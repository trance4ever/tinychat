#ifndef __TINYCHAT_UTIL_H__
#define __TINYCHAT_UTIL_H__
#include<unistd.h>
#include<string>
#include<sys/types.h>
#include<chrono>
#include<ctime>
#include<sstream>
#include<sys/syscall.h>

namespace trance {
    // 得到进程号
    pid_t getPid();
    // 得到线程号
    pid_t getThreadId();
    // 得到日期时间
    std::string getDateTime();
    // 得到日期
    std::string getDate();
    // 得到毫秒数
    uint64_t getMillis();
    // 睡眠，单位毫秒
    void sleep(int mills);
}

#endif