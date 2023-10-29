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
TEMP_FILE_COM = ".transfer"
TEMP_FILE_ENDGAME = ".endgame"
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
    while i < MAX_NUM_PLAYERS:
        i+=1
        if start_game(): 
            BEGIN_GAME = True
            break
        conn, addr = server.accept()
        thread = threading.Thread(target=handle_client, args=(conn, addr))
        thread.start()
    
    wait_for_finish()

def start_game(): #! CHANGE WHEN GM PRESSES "START" BUTTON
    return False
    
def check_endgame(): #! CHECKS IF THE GAME HAS ENDED
    pass

def broadcast_endgame(conn: socket.socket, addr): #! SENDS ENDGAME MESSAGE TO ALL CLIENTS
    conn.send("ENDGAME".encode(FORMAT))
    pass

def wait_for_finish(): #! WAITS FOR THE ENDGAME
    pass

if __name__ == "__main__":
    main()