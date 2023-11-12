import pathlib
import socket
import time
import sys
import threading
import shutil
from enum import Enum


class Flags(Enum):
    ENDGAME = "<ENDGAME>"
    BEGIN = "<BEGIN>"
    CONFIG = "<CONFIG>"
    STOP = "<STOP>"
    POINTS = "<POINTS>"
    NEXT = "<NEXT>"
    RANK = "<RANK>"
    NICKNAME = "<NICKNAME>"

def get_local_ip():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))
    local_ip = s.getsockname()[0]
    s.close()
    return local_ip


SERVER = get_local_ip()
PORT = 5050
MAX_NUM_PLAYERS = int(sys.argv[1])
ADDR = (SERVER, PORT)
HEADER = 64
FORMAT = 'utf-8'
TEMP_DIR = ".server"
TEMP_FILE_COM = "transfer.txt"
ROUND_OVER_MSG = "ENDROUND"
ENDGAME_MSG = "ENDGAME"
DISCONNECT_MESSAGE = "!DISCONNECT"
ESCAPE_TOKEN = "/*"
NICKNAMES = {}
NEXT_COUNT = 0
POINTS = []



def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server:
        server.bind(ADDR)
        accept_clients(server)

def accept_clients(server: socket.socket):
    server.listen()
    i = 0
    print(f"[LISTENING] Server is listening on {SERVER}") #? Checks connectivity
    recv_threads, send_threads, conns, addrs = [], [], [], []
    while i < MAX_NUM_PLAYERS:
        if forced_start():
            MAX_NUM_PLAYERS = i
            break
        conn, addr = server.accept()
        conns.append(conn)
        addrs.append(addr)
        thread1 = threading.Thread(target=hear_client, args=(conn, addr))
        thread2 = threading.Thread(target=broadcast, args=(conn, addr))
        recv_threads.append(thread1)
        send_threads.append(thread2)
        thread1.start()
        thread2.start()
        i+=1
    print("All clients connected")
    print("Game started")
    wait_endgame()

def forced_start():
    if not (aux:=pathlib.Path(TEMP_DIR)).exists():
        aux.mkdir()
    if (aux:=(pathlib.Path(TEMP_DIR)/TEMP_FILE_COM)).exists():
        return True

def operate_on_file(conn, addr):
    if (aux := (pathlib.Path(TEMP_DIR)/TEMP_FILE_COM)).exists():
        with aux.open() as file:
            content = file.read()
            match content.split(ESCAPE_TOKEN)[0]:
                case _:
                    raise ValueError("Unknown flag")

def endgame(content: str, conns: list[socket.socket], addr):
    ret = '1' in content

    return ret

def stop(content: str, conn: socket.socket, addr):
    pass

def begin(content: str, conns: list[socket.socket]):
    [conn.send(bytes(content)) for conn in conns]

def config(content: str, conns: list[socket.socket]):
    [conn.send(bytes(content)) for conn in conns]

def points(content: str, conn: socket.socket, addr):
    with threading.Lock() as lock:
        POINTS.append((content.split(ESCAPE_TOKEN)[1], addr))
    time.sleep(1)
    rank(content, conn, addr)
    
def next(content: str, conn: socket.socket, addr):
    global NEXT_COUNT
    with threading.Lock() as lock:
        NEXT_COUNT += 1
    while NEXT_COUNT != MAX_NUM_PLAYERS:
        time.sleep(0.1)
    ...

def rank(content: bytes, conn: socket.socket, addr):
    with threading.Lock() as lock:
        POINTS.sort()
        POINTS.reverse()
        content.replace(b'POINTS', b'RANK')
        conn.send(bytes(POINTS))
        POINTS.clear()


def nickname(content: str, conn: socket.socket, addr):
    with threading.Lock() as lock:
        NICKNAMES[addr] = content.split(ESCAPE_TOKEN)[1]
    pass

def hear_client(conn: socket.socket, addr):
    while True:
        content = conn.recv(HEADER).decode(FORMAT)
        match content.split(ESCAPE_TOKEN)[0]:
            case Flags.NEXT.value:
                next(content, conn, addr)
            case Flags.POINTS.value:
                points(content, conn, addr)
            case Flags.STOP.value:
                stop(content, conn, addr)
            case Flags.NICKNAME.value:
                nickname(content, conn, addr)

def broadcast():
    pass

def wait_endgame():
    while True:
        pass
    shutil.rmtree(pathlib.Path(TEMP_DIR))