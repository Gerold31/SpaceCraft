#ifndef _MESSAGEQUIT_HPP_
#define _MESSAGEQUIT_HPP_

#include "Message.hpp"

#include <OIS/OIS.h>

namespace ENGINE
{

class MessageQuit : public Message
{
public:
    MessageQuit() : Message() {}
};

};

#endif
