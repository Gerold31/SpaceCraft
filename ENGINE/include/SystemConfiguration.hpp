#ifndef _SYSTEMCONFIGURATION_HPP_
#define _SYSTEMCONFIGURATION_HPP_

#include "System.hpp"
#include "Defines.hpp"

namespace ENGINE
{

class SystemConfiguration : public System, public Singleton<SystemConfiguration>
{
friend class Singleton<SystemConfiguration>;
public:
    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *msg);

    void loadFromFile(std::string filename);

    std::string getConfiguration(std::string name);

    bool isServer() {return mIsServer;}
    bool isClient() {return mIsClient;}

private:
    SystemConfiguration();
    SystemConfiguration(const SystemConfiguration &) : System("SystemConfiguration") {}
    ~SystemConfiguration();

    std::map<std::string, std::string> mConfiguration;

    bool mIsServer, mIsClient;

};

};

#endif
