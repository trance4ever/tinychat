#include "util.h"

namespace trance {

    static int g_pid = 0;

	static thread_local int g_thread_id = 0;

    pid_t getPid() {
        if(g_pid == 0) {
            g_pid = getpid();
        }
        return g_pid;
    }
    
    pthread_t getThreadId() {
        if(g_thread_id == 0) {
            g_thread_id = gettid();
        }
        return g_thread_id;
    }

    std::string getDateTime() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::tm* now_tm = std::localtime(&now_c);
        std::stringstream ss;
        ss << std::to_string(now_tm->tm_year + 1900) << "-"
            << std::to_string(now_tm->tm_mon + 1) << "-"
            << std::to_string(now_tm->tm_mday) << " "
            << std::to_string(now_tm->tm_hour) << ":"
            << std::to_string(now_tm->tm_min) << ":" 
            << std::to_string(now_tm->tm_sec);
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
}