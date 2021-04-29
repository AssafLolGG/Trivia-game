import socket

SERVER_PORT = 6969


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
        print("1 - logIn\n2 - signUp")
        if input() == "1":
            message_code = "\2"
            message = logIn(getString("userName"), getString("password"))
        else:
            message_code = "\3"
            message = signUp(getString("userName"), getString("password"), getString("email"))
    else:
        message = logIn("userName", "Password")
        message_code = "\2"

    return (message_code + str(len(message).to_bytes(4, "big")) + message).replace('b', '', 1).replace("'", "", 2).replace("\\x00", "\0")


def main():
    message = makeRequest()
    print(message)
    client_socket = client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        client_socket.connect(('localhost', SERVER_PORT))
    except Exception:
        print("connection to server failed")
        exit(1)
    try:
        client_socket.send(client_socket.recv(1024))
    except Exception:
        print("connection broke")
        exit(1)
    client_socket.send(message.encode())
    print(f"message received: {client_socket.recv(1024)}")

    while(True):
        pass


if __name__ == '__main__':
    main()
