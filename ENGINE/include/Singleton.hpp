#ifndef _SINGLETON_HPP_
#define _SINGLETON_HPP_

namespace ENGINE
{

template<typename T>
class Singleton
{
public:
    static T *getSingleton() {if(!mSingleton) mSingleton = new T(); return mSingleton;}
    
protected:
    Singleton() {}
    Singleton(const Singleton &) {}
    virtual ~Singleton() {}

private:
    static T *mSingleton;
};

template <typename T> T *Singleton<T>::mSingleton = 0;

};

#endif