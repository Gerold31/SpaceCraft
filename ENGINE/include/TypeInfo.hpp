#ifndef _TYPEINFO_HPP_
#define _TYPEINFO_HPP_

#include <map>
#include <string>

namespace ENGINE
{

class Object;

typedef void* (*CreateInstance)(Object *object, std::map<std::string, std::string> params);

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
