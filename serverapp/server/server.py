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
        if check_endgame(): 
            broadcast_endgame(conn, addr)

        msg_length = conn.recv(HEADER).decode(FORMAT)
        msg_length = int(msg_length)
        msg = conn.recv(HEADER).decode(FORMAT)
        if msg == DISCONNECT_MESSAGE:
            connected = False
            continue
        print(f"[{addr}] {msg}")

def accept_clients(server: socket.socket):
    server.listen()
    global BEGIN_GAME
    i = 0
    print(f"[LISTENING] Server is listening on {SERVER}")
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

def start_game(): #! CHANGE WHEN GM PRESSES "START" BUTTON
    if (aux:=(pathlib.Path(TEMP_DIR)/TEMP_START_GAME)).exists():
        aux.unlink()
        return True
    return False
    
def check_endgame(): #! CHECKS IF THE GAME HAS ENDED
    return (pathlib.Path(TEMP_DIR) / TEMP_FILE_ENDGAME).exists()

def broadcast_endgame(conns: list[socket.socket], addrs): #! SENDS ENDGAME MESSAGE TO ALL CLIENTS
    with (pathlib.Path(TEMP_DIR) / TEMP_FILE_ENDGAME).open() as file:
        msg = file.read()
        if msg == ROUND_OVER_MSG:
            pass
        elif msg == ENDGAME_MSG:
            pass
    pass

def wait_for_finish(threads: list, conns: list, addrs: list): #! WAITS FOR THE ENDGAME
    while True:
        if check_endgame(): 
            broadcast_endgame()
            break

if __name__ == "__main__":
    main()