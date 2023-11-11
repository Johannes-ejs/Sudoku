import pathlib
import socket, sys, threading

def get_local_ip():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8", 80)) 
    local_ip = s.getsockname()[0]
    s.close()
    return local_ip

SERVER = get_local_ip()
PORT = 5050
MAX_NUM_PLAYERS = int(sys.argv[1]) # - 1 ????
ADDR = (SERVER, PORT)
HEADER = 64 #!!!!!!!!!!!!!! TO CHANGE
FORMAT = 'utf-8'
TEMP_DIR = ".server"
TEMP_FILE_COM = ".transfer"
TEMP_FILE_ENDGAME = ".endgame"
TEMP_START_GAME = ".startgame"
ROUND_OVER_MSG = "ENDROUND"
ENDGAME_MSG = "ENDGAME"
DISCONNECT_MESSAGE = "!DISCONNECT"
BEGIN_GAME = False

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server:
        server.bind(ADDR)
        accept_clients(server)

def handle_client(conn: socket.socket, addr):
    connected = True
    global BEGIN_GAME
    while connected and BEGIN_GAME:
        msg_length = conn.recv(HEADER).decode(FORMAT)
        msg_length = int(msg_length)
        msg = conn.recv(msg_length).decode(FORMAT)
        if msg == DISCONNECT_MESSAGE:
            connected = False
            continue
        print(f"[{addr}] {msg}")

def accept_clients(server: socket.socket):
    server.listen()
    global BEGIN_GAME
    i = 0
    print(f"[LISTENING] Server is listening on {SERVER}") #? Checks connectivity
    threads, conns, addrs = [], [], []
    while i < MAX_NUM_PLAYERS:
        i+=1
        if start_game(): 
            BEGIN_GAME = True
            break
        conn, addr = server.accept()
        conns.append(conn)
        addrs.append(addr)
        thread = threading.Thread(target=handle_client, args=(conn, addr))
        threads.append(thread)
        thread.start()
    
    wait_for_finish(threads, conns, addrs)

def start_game(): #! DONE
    if (aux:=(pathlib.Path(TEMP_DIR)/TEMP_START_GAME)).exists():
        aux.unlink()
        return True
    return False
    
def check_endgame(): #! DONE
    return (pathlib.Path(TEMP_DIR) / TEMP_FILE_ENDGAME).exists()

def broadcast_endgame(conns: list[socket.socket], addrs) -> int: #! SENDS ENDGAME MESSAGE TO ALL CLIENTS
    with (pathlib.Path(TEMP_DIR) / TEMP_FILE_ENDGAME).open() as file:
        msg = file.read().strip()
        if msg == ROUND_OVER_MSG:
            return 1
        elif msg == ENDGAME_MSG:
            return 0

def wait_for_finish(threads: list, conns: list, addrs: list): #! WAITS FOR THE ENDGAME
    while True:
        if check_endgame(): 
            if broadcast_endgame() == 0:
                break
            else:
                pass

if __name__ == "__main__":
    main()