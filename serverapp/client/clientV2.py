import socket
import os
from threading import Thread
import pathlib
import time
import shutil


SERVER: str = '192.168.0.137'
PORT: int = 5050
ADDR: tuple[str, int]= (SERVER, PORT)
FORMAT = 'utf-8'
ESCAPE_TOKEN = '\n'
TEMP_DIR = '.client'
TEMP_FILE_COM = 'transfer.txt'


def get_nickname()->str:
    try:
        with open(os.path.join('.client','transfer.txt'), 'r') as f:
            texto = f.read()
            lista = texto.splitlines()
            if lista[0] == '<NICKNAME>':
                return lista[1]
            else:
                print('Problemas ao abrir arquivo transfer.txt')
    except:
        print('Problemas ao abrir arquivo nickname.')


def get_points()->str:
    try:
        with open(os.path.join(TEMP_DIR,'transfer.txt'), 'r') as f:
            texto = f.read()
            lista = texto.splitlines()
            if lista[0] == '<POINTS>':
                return lista[1]
            else:
                print('Problemas ao abrir arquivo transfer.txt')
    except:
        print('Problemas ao abrir arquivo transfer.txt')


def mkdir_client():
    #Criar
    if not (aux := pathlib.Path(TEMP_DIR)).exists():
        aux.mkdir()


def write_to_file(mensagem: str)->None:
    with open(os.path.join('.client','transfer.txt'), 'w') as f:
        f.write(mensagem)


def main():
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        client.connect(ADDR)
    except:
        return print('\nNão foi possível conectar ao servidor...')

    print(f'Conectado a {SERVER}...')
    mkdir_client()

    thread1 = Thread(target=receive_message, args=[client])
    thread2 = Thread(target=send_message, args=[client])
    
    threads = [thread1, thread2]

    thread1.start()
    thread2.start()
    
    while True:
        time.sleep(1)
        if all(not thread.is_alive() for thread in threads):
            print('Jogo encerrado. Fechando tudo...')
            break
    shutil.rmtree(pathlib.Path(TEMP_DIR))
    


def receive_message(client: socket.socket):
    # MENSAGENS RECEBIDAS PELO SERVIDOR COM AS SEGUINTES FLAGS:
    # <CONFIG>
    # <BEGIN>
    # <RANK>
    # <ENDGAME>
    
    while True:
        try:
            msg = client.recv(1024).decode(FORMAT)
            print(f'\nServidor -> Cliente: {msg}')
            write_to_file(msg)
            # msgs = msg.split(ESCAPE_TOKEN)
            # match(msgs[0]):
            #     case '<CONFIG>':
            #         write_to_file(msg)
            #     case '<BEGIN>':
            #         write_to_file(msg)
            #     case '<RANK>':
            #         write_to_file(msg)
            #     case '<ENDGAME>':
            #         client.send(f'<POINTS>{ESCAPE_TOKEN}{get_points()}'.encode(FORMAT))
        except:
            print('Não foi possível continuar conectado...')
            input('Pressione <enter> para continuar...')
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
            if client:
                msg = ''
                if (pathlib.Path(TEMP_DIR)/TEMP_FILE_COM).exists():
                    with (pathlib.Path(TEMP_DIR)/TEMP_FILE_COM).open() as file:
                        msg = file.read()
                        msgs = msg.split(ESCAPE_TOKEN)
                        match(msgs[0]):
                            case '<STOP>':
                                client.send(f'{msgs[0]}{ESCAPE_TOKEN}'.encode(FORMAT))
                            case '<NEXT>':
                                client.send(f'{msgs[0]}{ESCAPE_TOKEN}'.encode(FORMAT))
                            case '<POINTS>':
                                client.send(f'{msgs[0]}{ESCAPE_TOKEN}{get_points()}'.encode(FORMAT))
                            case '<NICKNAME>':
                                client.send(f'{msgs[0]}{ESCAPE_TOKEN}{get_nickname()}'.encode(FORMAT))
                    (pathlib.Path(TEMP_DIR)/TEMP_FILE_COM).unlink()
                else:
                    continue
            else:
                client.close()
                return
        except:
            client.close()
            return


if __name__ == '__main__':
    main()
    

#<NICKNAME>
#BRUCE_LEE

#<POINTS>
#125