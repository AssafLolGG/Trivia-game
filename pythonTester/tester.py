import socket

SERVER_PORT = 9999


def getString(string_name):
    print(f"Enter {string_name}: ")
    return input()


def logIn(userName, password):
    return f'{{"username": "{userName}", "password": "{password}"}}'.replace(" ", "")


def signUp(userName, password, email, address, phone, birthdate):
    return f'{{"username": "{userName}", "password": "{password}", "email": "{email}", "address": "{address}' \
           f'", "phone": "{phone}", "birthdate": "{birthdate}"}}'.replace(" ", "")


def getPlayersInRoom(room_id):
    return f'{{"roomID": "{room_id}"}}'.replace(" ", "")


def joinRoom(room_id):
    return f'{{"roomID": "{room_id}"}}'.replace(" ", "")


# define ROOM_ID "roomID"
# define ROOM_NAME "roomName"
# define MAX_USERS_IN_ROOM "maximumUsers"
# define QUESTION_COUNT "questionCount"
# define TIME_TO_ANSWER "timeToAnswer"
def createRoom(room_name, maximumUsers, questionCount, timeToAnswer):
    return f'{{"roomName": "{room_name}", "maximumUsers": "{maximumUsers}",' \
           f' "questionCount": "{questionCount}", "timeToAnswer": "{timeToAnswer}"}}'.replace(" ", "")


def makeRequest(auto=0):
    message = ""
    message_code = ""
    if auto == 0:
        # define ERROR_CODE 0
        # define LOGIN_CODE 1
        # define SIGNUP_CODE 2
        # define LOGOUT_CODE 3
        # define GET_ROOMS_CODE 4
        # define GET_PLAYERS_IN_ROOM_CODE 5
        # define GET_JOIN_ROOMS_CODE 6
        # define GET_CREATE_ROOMS_CODE 7
        # define GET_STATISTICS_CODE 8
        print("1 - logIn\n2 - signUp\n3 - logOut\n4 - get rooms\n5 - get players in room\n6 - join room"
              "\n7 - create room\n8 - get statistics")
        choice = input()
        if choice == "1":
            message_code = "\1"
            message = logIn(getString("userName"), getString("password"))
        elif choice == "2":
            message_code = "\2"
            # message = signUp("user1", "Aa123456", "email@gmail.com", "street,2,city", "054-123456", "00.00.0000")
            message = signUp(getString("userName"), getString("password"), getString("email"), getString("address"),
                             getString("phone"), getString("birthdate"))
        elif choice == "3":  # logout
            message_code = "\3"
        elif choice == "4":  # get rooms
            message_code = "\4"
        elif choice == "5":  # get players in room
            message_code = "\5"
            message = getPlayersInRoom(getString("room id"))
        elif choice == "6":  # join room
            message_code = "\6"
            message = joinRoom(getString("room id"))
        elif choice == "7":  # create room
            message_code = "\7"
            createRoom(getString("room name"), getString("max users"), getString("question count"),
                       getString("time to answer"))
        elif choice == "8":  # get statistics
            message_code = "\8"
        else:
            print("exiting the program")
            exit(0)

    else:
        if auto == 1:
            message = logIn("userName", "Password")
            message_code = "\2"
        elif auto == 2:
            message = signUp("user1", "Aa123456", "street,1,city", "", "", "00.00.0000")
            message_code = "\3"
    lenOfMsg = len(message)
    print(lenOfMsg)
    nums = [0, 0, 0, 0]
    nums[3] = lenOfMsg % 256
    lenOfMsg = lenOfMsg // 256
    nums[2] = lenOfMsg % 256
    lenOfMsg = lenOfMsg // 256
    nums[1] = lenOfMsg % 256
    lenOfMsg = lenOfMsg // 256
    nums[0] = lenOfMsg % 256

    completeMsg = message_code.encode()
    completeMsg += nums[0].to_bytes(1, "big")
    completeMsg += nums[1].to_bytes(1, "big")
    completeMsg += nums[2].to_bytes(1, "big")
    completeMsg += nums[3].to_bytes(1, "big")
    completeMsg += message.encode()
    return completeMsg


def connect():
    client_socket = client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        client_socket.connect(('localhost', SERVER_PORT))
    except Exception:
        print("connection to server failed")
        exit(1)
    print("connection established")
    return client_socket


def connectAndMessage():
    client_message = b"hello"
    client_socket = connect()
    # try:
    while True:
        client_socket.send(client_message)
        print(f"client message: {client_message}")
        server_message = client_socket.recv(1024)  # receive response from server
        print(f"server response: {server_message.decode()}")
        client_message = makeRequest()  # make client message
    # except Exception:
    # print(f"connection broke")


def main():
    # print(makeRequest())
    # print(makeRequest())
    connectAndMessage()


if __name__ == '__main__':
    main()
