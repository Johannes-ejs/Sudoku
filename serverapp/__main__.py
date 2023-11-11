import sys

def main():
    if sys.argv[1] == 'server':
        from server import server
        server.main()
    elif sys.argv[1] == 'client':
        from client import client
        client.main()

if __name__ == "__main__":
    main()