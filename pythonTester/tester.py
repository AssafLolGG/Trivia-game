import socket
import threading
import time

SERVER_PORT = 9999

mutex = threading.Lock()


def getString(string_name):
    print(f"Enter {string_name}: ")
    return input()


def logIn(userName, password):
    return f'{{"username": "{userName}", "password": "{password}"}}'.replace(" ", "")


def signUp(userName, password, email):
    return f'{{"username": "{userName}", "password": "{password}", "email": "{email}"}}'.replace(" ", "")


def makeRequest(auto=0):
    message = ""
    message_code = ""
    if auto == 0:
        print("1 - logIn\n2 - signUp\n3 - exit")
        choice = input()
        if choice == "1":
            message_code = "\2"
            message = logIn(getString("userName"), getString("password"))
        elif choice == "2":
            message_code = "\3"
            message = signUp(getString("userName"), getString("password"), getString("email"))
        else:
            print("exiting the program")
            exit(0)
    else:
        if auto == 1:
            message = logIn("userName", "Password")
            message_code = "\2"
        elif auto == 2:
            message = signUp("userName", "Password", "email")
            message_code = "\3"

    return (message_code + str(len(message).to_bytes(4, "big")) + message).replace('b', '', 1).replace("'", "",
                                                                                                       2).replace(
        "\\x00", "\0")


def connect():
    client_socket = client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        client_socket.connect(('localhost', SERVER_PORT))
    except Exception:
        print("connection to server failed")
        exit(1)
    return client_socket


def connectAndMessage():
    client_message = "hello"
    client_socket = connect()
    try:
        while True:
            client_socket.send(client_message.encode())  # send message to server
            server_message = client_socket.recv(1024)  # receive response from server
            print(server_message.decode())
            client_message = makeRequest()  # make client message
    except Exception:
        print(f"connection broke")


def main():
    connectAndMessage()


if __name__ == '__main__':
    main()
