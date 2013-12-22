#ifndef _COMPONENTHARDWAREKEYBOARD_HPP_
#define _COMPONENTHARDWAREKEYBOARD_HPP_

#include "ComponentHardware.hpp"

#include <queue>

#include "OIS/OIS.h"

namespace SpaceCraft
{

class ComponentHardwareKeyboard : public ComponentHardware
{
public:
    ComponentHardwareKeyboard(Object *object, ParamMap &params);
    ~ComponentHardwareKeyboard();

    static void *createInstance(Object *object, ParamMap &params);
    
    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *message);

    static TypeInfo *getType() {return mType;}

private:
	void interrupt();
	
    static TypeInfo *mType;
    
    std::queue<WORD > mKeyBuffer;
    WORD mInterruptMsg;
    
    OIS::KeyCode keyToCode(WORD key);
    WORD codeToKey(OIS::KeyCode code);
};

};

#endif
