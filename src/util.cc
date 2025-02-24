#include "util.h"

namespace trance {

    static int g_pid = 0;

    pid_t getPid() {
        if(g_pid == 0) {
            g_pid = getpid();
        }
        return g_pid;
    }
    
    pid_t getThreadId() {
        return syscall(SYS_gettid);
    }

    std::string getDateTime() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::tm* now_tm = std::localtime(&now_c);
        std::stringstream ss;
        auto f = [](int num) {
            std::string tmp = std::to_string(num);
            if(num <= 9) {
                return "0" + tmp;
            }
            else {
                return tmp;
            }
        };
        std::string zero = "0";
        ss << std::to_string(now_tm->tm_year + 1900) << "-"
            << f(now_tm->tm_mon + 1) << "-"
            << f(now_tm->tm_mday) << " "
            << f(now_tm->tm_hour) << ":"
            << f(now_tm->tm_min) << ":" 
            << f(now_tm->tm_sec);
        return ss.str();
    }
    std::string getDate() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::tm* now_tm = std::localtime(&now_c);
        std::stringstream ss;
        ss << std::to_string(now_tm->tm_year + 1900) << "-"
            << std::to_string(now_tm->tm_mon + 1) << "-"
            << std::to_string(now_tm->tm_mday);
        return ss.str();
    }

    uint64_t getMillis() {
        auto now = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    }

    void sleep(int mills) {
        usleep(mills * 1000);
    }
}