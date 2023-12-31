import socket
import os
from threading import Thread
import pathlib
import time
import shutil
import sys


PORT: int = 5050
FORMAT = 'utf-8'
ESCAPE_TOKEN = '\n'
TEMP_DIR = '.client'
TEMP_FILE_PYTOCPP = 'python_to_cpp.txt'
TEMP_FILE_CPPTOPY = 'cpp_to_python.txt'


def mkdir_client():
    '''Cria a pasta .client/ para amarzenar os arquivos python_to_cpp.txt e cpp_to_python.cpp'''
    if not (aux := pathlib.Path(TEMP_DIR)).exists():
        aux.mkdir()


def rmdir_client(threads: list):
    '''remove a pasta .client/ que armazenar os arquivos python_to_cpp.txt e cpp_to_python.cpp'''
    while True:
        time.sleep(1)
        if all(not thread.is_alive() for thread in threads):
            print('Jogo encerrado. Fechando tudo...')
            break


def write_to_file(mensagem: str)->None:
    '''Função que recebe uma mensagem do servidor e escreve no arquito python_to_cpp.txt'''
    with open(os.path.join(TEMP_DIR, TEMP_FILE_PYTOCPP), 'w') as f:
        f.write(mensagem)


def main():
    '''Função principal que roda na aplicação do lado do cliente.'''
    ip_server = sys.argv[1]
    nickname = sys.argv[2]
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        client.connect((ip_server, PORT))
    except:
        return print('\nNão foi possível conectar ao servidor...')

    print(f'{nickname} conectado a {ip_server}...')
    mkdir_client()

    thread1 = Thread(target=receive_message, args=[client])
    thread2 = Thread(target=send_message, args=[client])
    
    threads = [thread1, thread2]

    thread1.start()
    thread2.start()
    
    rmdir_client(threads)
    shutil.rmtree(pathlib.Path(TEMP_DIR))
    print('Saindo do jogo. Adeus...')
    


def receive_message(client: socket.socket):
    '''MENSAGENS RECEBIDAS PELO SERVIDOR COM A SEGUINTE FLAG:
    <CONFIG>. Esta flag sinaliza a aplicação que a mensagem contém a matriz do quebra-cabeça a ser resolvido
    e a matriz da solução do quebra-cabeça.'''
    
    while True:
        try:
            msg = client.recv(1024).decode(FORMAT)
            print(f'\nServidor -> Cliente: {msg}')
            write_to_file(msg)
            msgs = msg.split(ESCAPE_TOKEN)
            match(msgs[0]):
                case '<CONFIG>':
                    client.close()
                    break
        except:
            print('Não foi possível continuar conectado...')
            input('Pressione <enter> para continuar...')
            client.close()
            break


def send_message(client: socket.socket):
    '''Envia mensagens para o servidor. Mensagem enviada é:
    <STOP>. Esta flag sinaliza ao servidor que o jogo foi finalizado pelo cliente.'''

    while True:
        try:
            if (pathlib.Path(TEMP_DIR)/TEMP_FILE_CPPTOPY).exists():
                with (pathlib.Path(TEMP_DIR)/TEMP_FILE_CPPTOPY).open() as file:
                    msg = file.read()
                    client.send(f'{msg}'.encode(FORMAT))
                (pathlib.Path(TEMP_DIR)/TEMP_FILE_CPPTOPY).unlink()
            else:
                continue
        except:
            client.close()
            break


if __name__ == '__main__':
    main()
