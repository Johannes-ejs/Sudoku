import socket
import os
from threading import Thread


SERVER: str = '127.0.0.1'
PORT: int = 5050
ADDR: tuple[str, int]= (SERVER, PORT)
FORMAT = 'utf-8'
ESCAPE_TOKEN = '\n'
TEMP_DIR = '.client'


def get_username()->str:
    with open(os.path.join('.client','nickname.txt'), 'r') as f:
        nickname = f.readline()
        return nickname


def get_points()->str:
    with open(os.path.join('.client','transfer.txt'), 'r') as f:
        for line in f:
            if line.startswith('<POINTS>'):
                s = line.split('/*')
                return s[1]


def write_to_file(mensagem: str)->None:
    with open(os.path.join('.client','transfer.txt'), 'a') as f:
        f.write(mensagem)


def main():
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        client.connect(ADDR)
    except:
        return print('\nNão foi possível conectar ao servidor...')

    print('Conectado...')

    thread1 = Thread(target=receive_message, args=[client])
    thread2 = Thread(target=send_message, args=[client])

    thread1.start()
    thread2.start()


def receive_message(client: socket.socket):
    # MENSAGENS RECEBIDAS PELO SERVIDOR COM AS SEGUINTES FLAGS:
    # <CONFIG>
    # <BEGIN>
    # <RANK>
    # <ENDGAME>
    
    while True:
        try:
            msg = client.recv(1024).decode(FORMAT)
            write_to_file(msg)
        except:
            print('Não foi possível continuar conectado...')
            print('Pressione <enter> para continuar...')
            client.close()
            break


def send_message(client: socket.socket):
    # MENSAGENS ENVIADAS AO SERVIDOR COM AS SEGUINTES FLAGS:
    # <STOP>
    # <NEXT>
    # <POINTS>
    # <NICKNAME>

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
            client.close()
            return


if __name__ == '__main__':
    main()