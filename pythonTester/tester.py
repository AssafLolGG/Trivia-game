import socket

SERVER_PORT = 6969


def getString(string_name):
    print(f"Enter {string_name}: ")
    return input()


def logIn(userName, password):
    return f'{{username": "{userName}", password: "{password}"}}'.replace(" ", "")


def signUp(userName, password, email):
    return f'{{username": "{userName}", password: "{password}", email: {email}}}'.replace(" ", "")


def makeRequest():
    message = ""
    message_code = ""
    print("1 - logIn\n2 - signUp")
    if (input() == "1"):
        message_code = "1"
        message = logIn(getString("userName"), getString("password"))
    else:
        message_code = "2"
        message = signUp(getString("userName"), getString("password"), getString("email"))

    return message_code + format(len(message), '04d') + message


def main():
    message = logIn("user", "password")
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
    print(client_socket.recv(1024))


if __name__ == '__main__':
    main()
