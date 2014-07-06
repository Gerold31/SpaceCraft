#ifndef _COMPONENTPLAYERSTATE_HPP_
#define _COMPONENTPLAYERSTATE_HPP_

#include "Component.hpp"

using namespace ENGINE;

namespace SpaceCraft
{

class ComponentPlayerState : public Component
{
public:
    ComponentPlayerState(Object *object, ParamMap &params);
    ~ComponentPlayerState();

    static void *createInstance(Object *object, ParamMap &params);
    
    bool init();
    void update(float elapsedTime);

    static TypeInfo *getType() {return mType;}

private:
    void _receiveMessage(Message *message);
    static TypeInfo *mType;

    enum STATE{
        STATE_WALKING,
        STATE_INGAMEMENU,
		STATE_INVENTORY,
        STATE_BUILDING,
        STATE_COUNT
    };

    void pushState(STATE s);
    void popState();

    std::vector<STATE> mState;
    std::map<STATE, std::vector<Component *> > mActiveComponents;
};

};

#endif
