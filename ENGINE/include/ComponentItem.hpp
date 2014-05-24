#ifndef _COMPONENTITEM_HPP_
#define _COMPONENTITEM_HPP_

#include "Component.hpp"

namespace ENGINE
{

class ComponentItem : public Component
{
public:
    ComponentItem(Object *object, ParamMap &params);
    ~ComponentItem();

    static void *createInstance(Object *object, ParamMap &params);
    
    bool init();
    void update(float elapsedTime);
    void receiveMessage(Message *message);

    static TypeInfo *getType() {return mType;}

    std::string getName() { return mName; }
    void setName(std::string name) { mName = name; }
    unsigned int getCount() { return mCount; }
    void setCount(unsigned int count) { mCount = count; }
    
	bool isEmpty() const;
	bool compare(ComponentItem *other) const;
	void move(ComponentItem *other);

private:
    static TypeInfo *mType;

	std::string mName;
	unsigned int mCount;
};

};

#endif
