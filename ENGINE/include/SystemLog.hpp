#ifndef _SYSTEMLOG_HPP_
#define _SYSTEMLOG_HPP_

#include "System.hpp"

#include <vector>
#include <fstream>

#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>

namespace ENGINE
{

typedef boost::thread::id ThreadID;
typedef std::pair<std::string, ThreadID> LogInfo;

class SystemLog : public System, public Singleton<SystemLog>
{
friend class Singleton<SystemLog>;
public:
    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *msg);

    void createLog(std::string filename = "log.html");
    void addLogType(std::string type, std::string style);
    void log(std::string message, std::string type = "log");
    void enter(std::string message, std::string type = "log");
    void exit(std::string message, std::string type = "log");
    void close();

private:
    enum State
    {
        STATE_STARTED,
        STATE_CREATED,
        STATE_INITED,
        STATE_CLOSED
    };

    SystemLog();
    SystemLog(const SystemLog &) : System("SystemLog") {}
    ~SystemLog();

    std::vector<std::pair<LogInfo, std::string> > mLogBuffer;
    boost::mutex mLogBufferMutex;

    std::map<ThreadID, int> mLogLevel;
    std::vector<ThreadID> mThreads;

    std::fstream mLogFile;

    State mState;

    ThreadID getThreadID();

};

};

#endif
