#ifndef __TINYCHAT_LOG_H__
#define __TINYCHAT_LOG_H__
#include "util.h"
#include<iostream>
#include<memory>
#include<vector>
#include<string>


namespace trance {
    #define __UNKOWN_LOG__(str) \
        Logger::getGlobalLogger()->pushLog(LogEvent(UNKOWN, __FILE__, __LINE__), str); \

    #define __INFO_LOG__(str) \
        Logger::getGlobalLogger()->pushLog(LogEvent(INFO, __FILE__, __LINE__), str); \

    #define __WAEN_LOG__(str) \
        Logger::getGlobalLogger()->pushLog(LogEvent(WARN, __FILE__, __LINE__), str); \

    #define __ERROR_LOG__(str) \
        Logger::getGlobalLogger()->pushLog(LogEvent(ERROR, __FILE__, __LINE__), str); \

    #define __FATAL_LOG__(str) \
        Logger::getGlobalLogger()->pushLog(LogEvent(FATAL, __FILE__, __LINE__), str); \

    //日志级别
    enum LogLevel {
        UNKOWN,
        INFO,
        WARN,
        ERROR,
        FATAL
    };
    class LogEvent;
    //日志器
    class Logger {
    public:
        Logger(LogLevel level = INFO);

        typedef std::shared_ptr<Logger> ptr;

        void pushLog(LogEvent l, std::string str);

        static std::shared_ptr<Logger> getGlobalLogger();

        void init();
    private:
        LogLevel m_level;
    };

    //事件类
    class LogEvent {
    public:
        LogEvent(LogLevel l, std::string file_name, int file_line,
            int pid = getPid(), int thread_id = getThreadId(), std::string dateTime = getDateTime()) : 
                m_level(l),
                m_file_name(file_name),
                m_file_line(std::to_string(file_line)),
                m_pid(pid),
                m_thread_id(thread_id),
                m_dateTime(dateTime) { }

        std::string toString();

        std::string stringFromLogLevel();

        void pushLog(LogEvent l, std::string str);
    private:	
		LogLevel m_level; //日志级别
        std::string m_file_name; //文件名
		std::string m_file_line; //行号
		int m_pid; //进程号
		int m_thread_id; //线程号
        std::string m_dateTime; //时间		
    };
}

#endif
