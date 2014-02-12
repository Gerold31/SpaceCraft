#include "SystemLog.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>

#include <iostream>

#define IGNORE_BUFFER

using namespace ENGINE;

SystemLog::SystemLog() :
    System("SystemLog")
{
    mState = STATE_STARTED;
}

SystemLog::~SystemLog()
{
    if(mState != STATE_CLOSED)
        close();
}

void SystemLog::init()
{
    LOG_IN("system");
    if(mState == STATE_STARTED)
        createLog();

    mLogFile << "</style><body>Log<table border=\"1\" cellspacing=\"0\" cellpadding=\"0\" frame=\"box\">" << std::endl;

    mState = STATE_INITED;
    LOG_OUT("system");
}

void SystemLog::update(float elapsedTime)
{
    mLogBufferMutex.lock();
    for(auto i=mLogBuffer.begin(); i!=mLogBuffer.end(); ++i)
    {
        boost::posix_time::ptime t = boost::posix_time::microsec_clock::local_time();
        std::string out = "<tr><td><pre>" + boost::posix_time::to_simple_string(t) + "</pre></td>";
        ThreadID id = (*i).first.second;
        for(auto j=mThreads.begin(); j!=mThreads.end(); ++j)
        {
            if(*j == id)
                break;
            out += "<td></td>";
        }
        out += "<td class=\"" + (*i).first.first + "\"><pre>" + (*i).second + "</pre></td>";
        mLogFile << out << std::endl;
    }
    mLogBuffer.clear();
    mLogBufferMutex.unlock();
}

void SystemLog::receiveMessage(Message *msg)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void SystemLog::createLog(std::string filename)
{
    LOG_IN("system");
    if(mState != STATE_STARTED)
        throw "cannot create Logfile";

    mLogFile.open(filename, std::ios_base::out);
    mLogFile << "<html><head><title>Log</title><style type=\"text/css\">" << std::endl;
    mLogFile << "tr{text-align: left; vertical-align: middle; border-width:2px;}" << std::endl;
    mLogFile << "td{text-align: left; vertical-align: middle; border-width:2px;}" << std::endl;

    mState = STATE_CREATED;

    addLogType("error"    , "color: #FFFF00;background-color: #FF0000");
    addLogType("warning"  , "color: #000000;background-color: #FF7F00");
    addLogType("log"      , "color: #000000;background-color: #FFFFFF");
    addLogType("engine"   , "color: #7F0000;background-color: #FFFFFF");
    addLogType("system"   , "color: #007F00;background-color: #FFFFFF");
    addLogType("object"   , "color: #00007F;background-color: #FFFFFF");
    addLogType("component", "color: #007F7F;background-color: #FFFFFF");
    addLogType("message"  , "color: #7F7F00;background-color: #FFFFFF");
    addLogType("frame"    , "color: #444444;background-color: #FFFFFF");

    LOG_OUT("system");
}

void SystemLog::addLogType(std::string type, std::string style)
{
    LOG_IN("system");
    if(mState != STATE_CREATED)
        throw "cannot add Logtype";

    mLogFile << "td." << type << "{text-align: left; vertical-align: middle; border-width:2px;" << style << "}" << std::endl;

    LOG("Log type: \"" + type + "\"", type);

    LOG_OUT("system");
}

void SystemLog::log(std::string message, std::string type)
{
    ThreadID id = getThreadID();
    int level = mLogLevel[id];
    std::string prefix = "";
    for(; level>0; level--)
        prefix += "\t";

    mLogBufferMutex.lock();
    mLogBuffer.push_back(std::pair<LogInfo, std::string>(LogInfo(type, id), prefix + message));
    mLogBufferMutex.unlock();
#ifdef IGNORE_BUFFER
    if(mState == STATE_INITED)
        update(0);
#endif

    //std::cout << prefix + message << std::endl;
}

void SystemLog::enter(std::string message, std::string type)
{
    ThreadID id = getThreadID();
    log("E: " + message, type);
    mLogLevel[id]++;
}

void SystemLog::exit(std::string message, std::string type)
{
    ThreadID id = getThreadID();
    mLogLevel[id]--;
    log("X: " + message, type);
}

void SystemLog::close()
{
    LOG_IN("system");
    if(mState == STATE_INITED)
    {
        // empty buffer
        update(0);

        mLogFile << "<thead><tr><th style=\"border-width:2px;\">Time</th>";
        for(auto j=mThreads.begin(); j!=mThreads.end(); ++j)
        {
            mLogFile << "<th style=\"border-width:2px;\">" << *j << "</th>";
        }
        mLogFile << "</tr></thead>" << std::endl;
        mLogFile << "</table></body></html>" << std::endl;

        mState = STATE_CLOSED;
    }
    LOG_OUT("system");
}

ThreadID SystemLog::getThreadID()
{
    ThreadID id = boost::this_thread::get_id();
    if(mLogLevel.count(id) == 0)
    {
        mLogLevel[id] = 0;
        mThreads.push_back(id);
    }
    return id;
}
