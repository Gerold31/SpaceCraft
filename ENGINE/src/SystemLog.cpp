#include "SystemLog.hpp"

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
    if(mState == STATE_STARTED)
        createLog();

    addLogType("log"  , "color: #000000;background-color: #FFFFFF");
    addLogType("error", "color: #FF0000;background-color: #FFFFFF");

    mLogFile << "</style><body>Log<table border=\"1\" cellspacing=\"0\" cellpadding=\"0\" frame=\"box\">" << std::endl;

    mState = STATE_INITED;
}

void SystemLog::update(float elapsedTime)
{
    mLogBufferMutex.lock();
    for(auto i=mLogBuffer.begin(); i!=mLogBuffer.end(); ++i)
    {
        std::string out = "<tr><td><pre>@todo add time</pre></td>";
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
}

void SystemLog::createLog(std::string filename)
{
    if(mState != STATE_STARTED)
        throw "cannot create Logfile";

    mLogFile.open(filename, std::ios_base::out);
    mLogFile << "<html><head><title>Log</title><style type=\"text/css\">" << std::endl;
    mLogFile << "tr{text-align: left; vertical-align: middle; border-width:2px;}" << std::endl;
    mLogFile << "td{text-align: left; vertical-align: middle; border-width:2px;}" << std::endl;

    mState = STATE_CREATED;
}

void SystemLog::addLogType(std::string type, std::string style)
{
    if(mState != STATE_CREATED)
        throw "cannot add Logtype";

    mLogFile << "td." << type << "{text-align: left; vertical-align: middle; border-width:2px;" << style << "}" << std::endl;
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
