#include "JsonRequestPacketDeserializer.h"

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(std::vector<uint8_t> buffer)
{
    json result;
    long long int four_bytes = buffer[MESSAGE_SIZE_PLACE];
    int size = 0;
    LoginRequest login;
    
    // combining all four bytes together
    for (int i = MESSAGE_SIZE_PLACE + 1; i < MESSAGE_SIZE + MESSAGE_SIZE_PLACE; i++)
    {
        four_bytes = stringNumbers(four_bytes, buffer[i]);
    }

    size = binToDecimal(four_bytes); // converting binary to decimal
    size = size - buffer.size() + MESSAGE_SIZE + MESSAGE_SIZE_PLACE; // resizing to the total size from the end of the vector
    
    std::vector<uint8_t> content = { buffer.begin() + MESSAGE_SIZE + MESSAGE_SIZE_PLACE, buffer.end() - size };

    result = json::from_bson(content);
    login.username = result.value(USERNAME, "");
    login.password = result.value(PASSWORD, "");

    return login;
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(std::vector<uint8_t> buffer)
{
    json result;
    long long int four_bytes = buffer[MESSAGE_SIZE_PLACE];
    int size = 0;
    SignupRequest sign;

    // combining all four bytes together
    for (int i = MESSAGE_SIZE_PLACE + 1; i < MESSAGE_SIZE + MESSAGE_SIZE_PLACE; i++)
    {
        four_bytes = stringNumbers(four_bytes, buffer[i]);
    }

    size = binToDecimal(four_bytes); // converting binary to decimal
    size = size - buffer.size() + MESSAGE_SIZE + MESSAGE_SIZE_PLACE; // resizing to the total size from the end of the vector

    std::vector<uint8_t> content = { buffer.begin() + MESSAGE_SIZE + MESSAGE_SIZE_PLACE, buffer.end() - size };

    result = json::from_bson(content);
    sign.username = result.value(USERNAME, "");
    sign.password = result.value(PASSWORD, "");
    sign.email = result.value(EMAIL, "");

    return sign;
}
