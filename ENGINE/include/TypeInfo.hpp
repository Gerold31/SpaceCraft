#ifndef _TYPEINFO_HPP_
#define _TYPEINFO_HPP_

#include "Defines.hpp"

namespace ENGINE
{

class Object;

class TypeInfo
{
public:
    TypeInfo(std::string name, CreateInstance _createInstance) :
        mName(name),
        createInstance(_createInstance)
    {
    }

    ~TypeInfo();

    CreateInstance createInstance;

    std::string getName() {return mName;}

private:
    const std::string mName;
};

};

#endif
