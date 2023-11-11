import pathlib
import socket
import sys
import threading


def get_local_ip():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))
    local_ip = s.getsockname()[0]
    s.close()
    return local_ip


SERVER = get_local_ip()
PORT = 5050
MAX_NUM_PLAYERS = int(sys.argv[1])  # - 1 ????
ADDR = (SERVER, PORT)
HEADER = 64
FORMAT = 'utf-8'
TEMP_DIR = ".server"
TEMP_FILE_COM = "transfer"
ROUND_OVER_MSG = "ENDROUND"
ENDGAME_MSG = "ENDGAME"
DISCONNECT_MESSAGE = "!DISCONNECT"
BEGIN_GAME = False

