#include "JsonRequestPacketDeserializer.h"

json JsonRequestPacketDeserializer::getJson(std::vector<uint8_t> buffer)
{
    int size = buffer[MESSAGE_SIZE_PLACE];

    // getting message size
    for (int i = MESSAGE_SIZE_PLACE + 1; i < MESSAGE_SIZE + MESSAGE_SIZE_PLACE; i++)
    {
        size = stringNumbers(size, buffer[i]);
    }

    std::vector<uint8_t> content = { buffer.begin() + MESSAGE_SIZE + MESSAGE_SIZE_PLACE, buffer.begin() + size + MESSAGE_SIZE + MESSAGE_SIZE_PLACE }; // getting message content

    return json::parse(content.begin(), content.end());
}

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(std::vector<uint8_t> buffer)
{
    json result;
    LoginRequest login;

    result = JsonRequestPacketDeserializer::getJson(buffer);

    login.username = result.value(USERNAME, "");
    login.password = result.value(PASSWORD, "");

    return login;
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(std::vector<uint8_t> buffer)
{
    json result;
    SignupRequest sign;

    result = JsonRequestPacketDeserializer::getJson(buffer);

    sign.username = result.value(USERNAME, "");
    sign.password = result.value(PASSWORD, "");
    sign.email = result.value(EMAIL, "");

    return sign;
}
