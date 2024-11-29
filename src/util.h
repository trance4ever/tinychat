#ifndef __TINYCHAT_UTIL_H__
#define __TINYCHAT_UTIL_H__
#include<unistd.h>
#include<string>
#include<sys/types.h>
#include<chrono>
#include<ctime>
#include<sstream>

namespace trance {

    pid_t getPid();

    pthread_t getThreadId();

    std::string getDateTime();

}

#endif