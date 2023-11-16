import pathlib
import socket
import time
import sys
import threading
import shutil
from enum import Enum
import select
import subprocess
import argparse

class Flags(Enum):
    ENDGAME = "<ENDGAME>"
    BEGIN = "<BEGIN>"
    CONFIG = "<CONFIG>"
    STOP = "<STOP>"
    POINTS = "<POINTS>"
    NEXT = "<NEXT>"
    RANK = "<RANK>"
    NICKNAME = "<NICKNAME>"
    DISCONNECT = "<DISCONNECT>"

def get_local_ip():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))
    local_ip = s.getsockname()[0]
    s.close()
    return local_ip

SERVER = get_local_ip()
PORT = 5050
MAX_NUM_PLAYERS = 1
ADDR = (SERVER, PORT)
HEADER = 64
FORMAT ='utf-8'
TEMP_DIR = ".server"
CPP_TO_PYTHON = "cpp_to_python.txt"
PYTHON_TO_CPP = "python_to_cpp.txt"
ROUND_OVER_MSG = "ENDROUND"
ENDGAME_MSG = "ENDGAME"
ESCAPE_TOKEN = "\n"
NICKNAMES = {}
NEXT_COUNT = 0
POINTS = []


def is_socket_closed(sock):
    try:
        error_code = sock.getsockopt(socket.SOL_SOCKET, socket.SO_ERROR)
        return False
    except socket.error as e:
        if e.errno == 10054:
            return True
        else:
            raise e


socket.socket.__bool__ = is_socket_closed

def parse_args():
    argparser = argparse.ArgumentParser()
    argparser.add_argument("-n", "--num_players", type=int, default=1)
    argparser.add_argument("-d", "--difficulty", type=int, default=1)
    return argparser.parse_args()

def main():
    global MAX_NUM_PLAYERS
    args = parse_args()
    MAX_NUM_PLAYERS = args.num_players
    diff = args.difficulty
    print(f"Difficulty: {diff}")
    subprocess.run("g++ sudoku.cpp -o sudoku", shell=True)
    out = subprocess.run(f".\\sudoku {diff}", shell=True, capture_output=True).stdout.decode(FORMAT)
    if not out:
        out = subprocess.run(f"./sudoku {diff}", shell=True, capture_output=True).stdout.decode(FORMAT)
    print(out)
   

    try:
        server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server.bind(ADDR)
        accept_clients(server, out)
    except KeyboardInterrupt:
        server.close()
    finally:
        server.close()

def accept_clients(server: socket.socket, out: str):
    global MAX_NUM_PLAYERS
    server.listen()
    i = 0
    print(f"[LISTENING] Server is listening on {SERVER}") #? Checks connectivity
    threads, conns, addrs = [], [], []
    while i < MAX_NUM_PLAYERS:

        ready_to_read, _, _ = select.select([server], [], [], 0)
        if ready_to_read:
            conn, addr = server.accept()
            print(conn, addr)
            conns.append(conn)
            addrs.append(addr)
            thread1 = threading.Thread(
                target=hear_client, args=(conn, addr, conns))
            threads.append(thread1)
            thread1.start()
            config(Flags.CONFIG.value + ESCAPE_TOKEN + out, conn)
            time.sleep(0.5)
            i += 1

        if forced_start(conns):  # Check if the forced_start condition is met
            MAX_NUM_PLAYERS = i
            break

    print("All clients connected")
    time.sleep(0.2)
    print("Game started")
    wait_endgame(threads, conns)

def forced_start(conns):
    global TEMP_DIR, CPP_TO_PYTHON
    if not (aux:=pathlib.Path(TEMP_DIR)).exists():
        aux.mkdir()
    if (aux:=(pathlib.Path(TEMP_DIR)/CPP_TO_PYTHON)).exists():
        with aux.open() as file:
            config(file.read(), conns)
        return True
    
def broadcast_players(conns: list[socket.socket]):
    global NICKNAMES, ESCAPE_TOKEN
    [conn.send(bytes("<LIST>" + ESCAPE_TOKEN, FORMAT)+ bytes(ESCAPE_TOKEN.join(NICKNAMES.values()), FORMAT)) for conn in conns]

def endgame(content: str, conns: list[socket.socket], addr):
    [conn.send(bytes(content, FORMAT)) for conn in conns]

def stop(content: str, conn: socket.socket, addr, conns: list[socket.socket]):
    kill_conns(conns)
    # endgame(content.replace("STOP", "ENDGAME"), conns, addr)

def begin(content: str, conns: list[socket.socket]):
    [conn.send(bytes(content, FORMAT)) for conn in conns]

def config(content: str, conn: socket.socket):
    conn.send(bytes(content, FORMAT))

def points(content: str, conn: socket.socket, addr, conns: list[socket.socket]):
    global POINTS, NICKNAMES, ESCAPE_TOKEN
    with threading.Lock() as lock:
        POINTS.append((content.split(ESCAPE_TOKEN)[1], NICKNAMES.get(addr[0], addr[0])))
    time.sleep(1)
    rank(content, conn, addr, conns)
    
def next(content: str, conn: socket.socket, addr, conns: list[socket.socket]):
    global NEXT_COUNT
    with threading.Lock() as lock:
        NEXT_COUNT += 1
    while NEXT_COUNT != MAX_NUM_PLAYERS:
        time.sleep(0.1)
    while True:
        if (aux :=(pathlib.Path(TEMP_DIR) / CPP_TO_PYTHON)).exists():
            with aux.open() as file:
                config(bytes(file.read(), FORMAT), conns)
            aux.unlink()
            break
        time.sleep(0.1)

def rank(content: str, conn: socket.socket, addr, conns: list[socket.socket]):
    global POINTS
    with threading.Lock() as lock:
        POINTS.sort()
        POINTS.reverse()
        conn.send(bytes(Flags.RANK.value, FORMAT) + bytes(ESCAPE_TOKEN, FORMAT) +
                  bytes(' '.join(' '.join(element) for element in POINTS), FORMAT))
        POINTS.clear()

def nickname(content: str, conn: socket.socket, addr, conns: list[socket.socket]):
    global NICKNAMES, ESCAPE_TOKEN
    with threading.Lock() as lock:
        NICKNAMES[addr[0]] = content.split(ESCAPE_TOKEN)[1]
        print(NICKNAMES)

def hear_client(conn: socket.socket, addr, conns: list[socket.socket]):
    global ESCAPE_TOKEN, HEADER, FORMAT
    while True:
        try:
            content = conn.recv(HEADER).decode(FORMAT)
        except OSError:
            conn.close()
            return
        match content.split(ESCAPE_TOKEN)[0]:
            case Flags.NEXT.value:
                next(content, conn, addr, conns)
            case Flags.POINTS.value:
                points(content, conn, addr, conns)
            case Flags.STOP.value:
                stop(content, conn, addr, conns)
            case Flags.NICKNAME.value:
                nickname(content, conn, addr, conns)
            case _:
                print(f"Unrecognized flag: {content}")

def kill_conns(conns: list[socket.socket]):
    [conn.close() for conn in conns]
    # [conn.send(bytes(Flags.DISCONNECT.value, FORMAT)) for conn in conns]

def wait_endgame(threads: list[threading.Thread], conns: list[socket.socket]):
    global TEMP_DIR
    while True:
        time.sleep(1)
        if all(not conn for conn in conns):
            break
    shutil.rmtree(pathlib.Path(TEMP_DIR))

if __name__ == "__main__":
    main()
