#ifndef _DEFINES_HPP_
#define _DEFINES_HPP_

#include <string>
#include <vector>
#include <map>
#include <boost/any.hpp>

namespace ENGINE
{

class Object;
    
typedef std::map<std::string, boost::any> ParamMap;
typedef void* (*CreateInstance)(Object *object, ParamMap &params);

};

#endif
