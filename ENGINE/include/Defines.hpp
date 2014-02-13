#ifndef _DEFINES_HPP_
#define _DEFINES_HPP_

#include <string>
#include <vector>
#include <map>

#include <boost/any.hpp>
#include <boost/current_function.hpp>

//#define LOG_EVERY_FRAME
//#define LOG_EVERY_MSG

#define LOG_IN(type) SystemLog::getSingleton()->enter(BOOST_CURRENT_FUNCTION, type)
#define LOG_OUT(type) SystemLog::getSingleton()->exit(BOOST_CURRENT_FUNCTION, type)
#define LOG(msg, type) SystemLog::getSingleton()->log(msg, type)

#ifdef LOG_EVERY_FRAME
#define LOG_IN_FRAME SystemLog::getSingleton()->enter(BOOST_CURRENT_FUNCTION, "frame")
#define LOG_OUT_FRAME SystemLog::getSingleton()->exit(BOOST_CURRENT_FUNCTION, "frame")
#define LOG_FRAME(msg) SystemLog::getSingleton()->log(msg, "frame")
#else
#define LOG_IN_FRAME (void*)0
#define LOG_OUT_FRAME (void*)0
#define LOG_FRAME(x) (void*)0
#endif

#ifdef LOG_EVERY_MSG
#define LOG_IN_MSG SystemLog::getSingleton()->enter(BOOST_CURRENT_FUNCTION, "message")
#define LOG_OUT_MSG SystemLog::getSingleton()->exit(BOOST_CURRENT_FUNCTION, "message")
#define LOG_MSG(msg) SystemLog::getSingleton()->log(msg, "message")
#else
#define LOG_IN_MSG (void*)0
#define LOG_OUT_MSG (void*)0
#define LOG_MSG(x) (void*)0
#endif

namespace ENGINE
{

class Object;
    
typedef std::map<std::string, boost::any> ParamMap;
typedef void* (*CreateInstance)(Object *object, ParamMap &params);

};

#endif
