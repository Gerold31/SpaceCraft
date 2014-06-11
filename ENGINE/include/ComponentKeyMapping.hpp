#ifndef _COMPONENTKEYMAPPING_HPP_
#define _COMPONENTKEYMAPPING_HPP_

#include "Component.hpp"

#include "OIS/OIS.h"

namespace ENGINE
{

class ComponentKeyMapping : public Component
{
public:
    ComponentKeyMapping(Object *object, ParamMap &params);
    ~ComponentKeyMapping();

    static void *createInstance(Object *object, ParamMap &params);
    
    bool init();
    void update(float elapsedTime);

    static TypeInfo *getType() {return mType;}

private:
    void _receiveMessage(Message *message);
    static TypeInfo *mType;

    /**
        if a key doesn't has a LONG_PRESS-event, but a PRESS-event, a long press will also emit a PRESS-event, but not vice versa
    */
    enum KEY_EVENT_TYPE{
        DOWN,
        UP,
        PRESS,
        LONG_PRESS
    };

    std::map<std::pair<OIS::KeyCode, KEY_EVENT_TYPE>, int> mKeyMap;
    std::map<OIS::KeyCode, float> mKeyTime;

};

};

#endif
