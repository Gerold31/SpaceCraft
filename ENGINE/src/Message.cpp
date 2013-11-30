#include "Message.hpp"

#include "NetworkMessage.hpp"

#include <stdio.h>

using namespace ENGINE;

NetworkMessage *Message::toNetworkMessage()
{
    return nullptr;
}

int Message::calcID(std::string name)
{
    int id = 0;
    for(int i=0; i<name.size(); i++)
    {
        id += name[i] << (i % 38);
    }
    printf("Message %#.8x: \"%s\"\n", id, name.c_str());
    return id;
}