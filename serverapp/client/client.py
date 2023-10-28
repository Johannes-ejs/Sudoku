import socket
import sys
import threading

SERVER = sys.argv[1]
PORT = 5050
ADDR = (SERVER, PORT)
HEADER = 64 #!!!!!!!!!!!!!! TO CHANGE
FORMAT = 'utf-8'
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
    print(client.recv(2048).decode(FORMAT))

def get_msg():
    return input()