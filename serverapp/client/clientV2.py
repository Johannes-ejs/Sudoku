import socket
import os
from threading import Thread


SERVER: str = '127.0.0.1'
PORT: int = 5050
ADDR: tuple[str, int]= (SERVER, PORT)
FORMAT = 'utf-8'
ESCAPE_TOKEN = '/*'
TEMP_DIR = '.client'


def get_username()->str:
    with open(os.path.join('.client','nickname.txt'), 'r') as f:
        nickname = f.readline()
        return nickname


def get_points()->str:
    pass


def main():
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        client.connect(ADDR)
    except:
        raise Exception('\nNão foi possível conectar ao servidor...')
    print('Conectado...')

    thread1 = Thread(target=receive_message, args=[client])
    thread2 = Thread(target=send_message, args=[client])

    thread1.start()
    thread2.start()


def receive_message(client):
    while True:
        try:
            msg = client.recv(1024).decode(FORMAT)
            print(msg)
            msgs = msg.split(ESCAPE_TOKEN)
            match(msgs[0]):
                case '<BEGIN>':
                    pass
                case '<RANK>':
                    pass
                case '<ENDGAME>':
                    pass
        except:
            print('Não foi possível continuar conectado...')
            client.close()
            break


def send_message(client):
    while True:
        try:
            msg = input('\n')
            msgs = msg.split(ESCAPE_TOKEN)
            match(msgs[0]):
                case '<STOP>':
                    client.send(f'{msgs[0]}{ESCAPE_TOKEN}'.encode(FORMAT))
                case '<NEXT>':
                    client.send(f'{msgs[0]}{ESCAPE_TOKEN}'.encode(FORMAT))
                case '<POINTS>':
                    client.send(f'{msgs[0]}{ESCAPE_TOKEN}{get_points()}'.encode(FORMAT))
                case '<NICKNAME>':
                    client.send(f'{msgs[0]}{ESCAPE_TOKEN}{get_username()}'.encode(FORMAT))
        except:
            return


if __name__ == '__main__':
    main()