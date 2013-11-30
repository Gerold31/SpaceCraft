#ifndef _NETWORKMESSAGE_HPP_
#define _NETWORKMESSAGE_HPP_

#include <map>

namespace ENGINE
{

typedef void* (*CreateMessage)(std::stringstream &stream);

class NetworkMessage
{
public:
    void deserialize();

    static void registerMessge(int id, CreateMessage createMessage)
    {
        mMessages[id] = createMessage;
    }

  private:
    size_t mBodyLength;

    static std::map<int, CreateMessage> mMessages;
};

};

#endif
