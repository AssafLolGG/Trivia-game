#include "JsonRequestPacketDeserializer.h"

/* extracting json from buffer */
json JsonRequestPacketDeserializer::getJson(std::vector<uint8_t> buffer)
{
    int size = buffer[MESSAGE_SIZE_PLACE];
    std::vector<uint8_t> content;

    // getting message size
    for (int i = MESSAGE_SIZE_PLACE + 1; i < MESSAGE_SIZE + MESSAGE_SIZE_PLACE; i++)
    {
        size = stringNumbers(size, buffer[i]);
    }

    content = { buffer.begin() + MESSAGE_SIZE + MESSAGE_SIZE_PLACE, buffer.begin() + size + MESSAGE_SIZE + MESSAGE_SIZE_PLACE }; // getting message content

    return json::parse(content.begin(), content.end());
}

/* Deserialize login request */
LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(std::vector<uint8_t> buffer)
{
    json result;
    LoginRequest login;

    result = JsonRequestPacketDeserializer::getJson(buffer);

    login.username = result.value(USERNAME, "");
    login.password = result.value(PASSWORD, "");

    return login;
}

/* Deserialize signup request */
SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(std::vector<uint8_t> buffer)
{
    json result;
    SignupRequest sign;

    result = JsonRequestPacketDeserializer::getJson(buffer);

    sign.username = result.value(USERNAME, "");
    sign.password = result.value(PASSWORD, "");
    sign.email = result.value(EMAIL, "");
	sign.address = result.value(ADDRESS, "");
	sign.phone = result.value(PHONE, "");
	sign.birthdate = result.value(BIRTHDATE, "");
    return sign;
}

/* Deserialize players in room request */
GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(std::vector<uint8_t> buffer)
{
    json result;
    GetPlayersInRoomRequest players_in_room;

    result = JsonRequestPacketDeserializer::getJson(buffer);
    
    players_in_room.room_id = std::stoi(result.value(ROOM_ID, ""));

    return players_in_room;
}

/* Deserialize join room request */
JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(std::vector<uint8_t> buffer)
{
    json result;
    JoinRoomRequest join_room;

    result = JsonRequestPacketDeserializer::getJson(buffer);

    join_room.room_id = std::stoi(result.value(ROOM_ID, ""));

    return join_room;
}

/* Deserialize players create room request */
CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(std::vector<uint8_t> buffer)
{
    json result;
    CreateRoomRequest create_room;

    result = JsonRequestPacketDeserializer::getJson(buffer);

    create_room.room_name = result.value(ROOM_NAME, "");
    create_room.max_users = std::stoi(result.value(MAX_USERS_IN_ROOM, ""));
    create_room.question_count = std::stoi(result.value(QUESTION_COUNT, ""));
    create_room.answer_time_out = std::stoi(result.value(TIME_TO_ANSWER, ""));

    return create_room;
}
