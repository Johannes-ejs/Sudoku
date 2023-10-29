import socket
import sys
import threading
import pathlib
import time

SERVER = sys.argv[1]
PORT = 5050
ADDR = (SERVER, PORT)
HEADER = 64 #!!!!!!!!!!!!!! TO CHANGE
FORMAT = 'utf-8'
TEMP_FILE_COM = ".transfer"
TEMP_FILE_ENDGAME = ".endgame"
TEMP_DIR = ".client"
DISCONNECT_MESSAGE = "!DISCONNECT"

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client:
        client.connect(ADDR)
        loop(client)

    
def loop(client: socket.socket):
    connected = True
    while connected:
        msg = get_msg()
        if msg == DISCONNECT_MESSAGE:
            connected = False
        send(client, msg)
    pass

def send(client: socket.socket, msg: str):
    message = msg.encode(FORMAT)
    msg_length = len(message)
    send_length = str(msg_length).encode(FORMAT)
    send_length += b' '*(HEADER - len(send_length))
    client.send(send_length)
    client.send(message)
    handle(client.recv(2048).decode(FORMAT))

def get_msg():
    if not pathlib.Path(TEMP_DIR).exists():
        pathlib.Path(TEMP_DIR).mkdir()
    while not (path := (pathlib.Path(TEMP_DIR) / TEMP_FILE_COM)).exists():
       time.sleep(0.1)
    with path.open("r") as file:
        msg = file.readline().strip()
    path.unlink() 
    return msg

def handle(msg: str):
    print(msg)
    # if msg == "ENDGAME":
    #     pathlib.Path(TEMP_DIR).mkdir()
    #     with (pathlib.Path(TEMP_DIR) / TEMP_FILE_ENDGAME).open("w") as file:
    #         file.write("ENDGAME")
    #     sys.exit(0)

if __name__ == "__main__":
    main()