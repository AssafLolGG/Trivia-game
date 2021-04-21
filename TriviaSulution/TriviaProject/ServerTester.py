import socket
SERVER_PORT = 6969
def main():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect(('localhost', SERVER_PORT))
    while True:
        try:
            print()
            client_socket.sendall(client_socket.recv(1024))
        except Exception:
            break
if __name__ == '__main__':
    main()