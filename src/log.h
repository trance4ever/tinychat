#ifndef __TINYCHAT_LOG_H__
#define __TINYCHAT_LOG_H__
#include "util.h"
#include<iostream>
#include<memory>
#include<vector>
#include<stdarg.h>
#include<string>


namespace trance {
    #define FMT_UNKOWN_LOG(fmt, ...) \
        Logger::getGlobalLogger()->pushLog(LogEvent(UNKOWN, __FILE__, __LINE__), fmt, __VA_ARGS__); \

    #define FMT_INFO_LOG(fmt, ...) \
        Logger::getGlobalLogger()->pushLog(LogEvent(INFO, __FILE__, __LINE__), fmt, __VA_ARGS__); \

    #define FMT_WARN_LOG(fmt, ...) \
        Logger::getGlobalLogger()->pushLog(LogEvent(WARN, __FILE__, __LINE__), fmt, __VA_ARGS__); \

    #define FMT_ERROR_LOG(fmt, ...) \
        Logger::getGlobalLogger()->pushLog(LogEvent(ERROR, __FILE__, __LINE__), fmt, __VA_ARGS__); \

    #define FMT_FATAL_LOG(fmt, ...) \
        Logger::getGlobalLogger()->pushLog(LogEvent(FATAL, __FILE__, __LINE__), fmt, __VA_ARGS__); \

    #define UNKOWN_LOG(str) \
        Logger::getGlobalLogger()->pushLog(LogEvent(UNKOWN, __FILE__, __LINE__), str); \

    #define INFO_LOG(str) \
        Logger::getGlobalLogger()->pushLog(LogEvent(INFO, __FILE__, __LINE__), str); \

    #define WARN_LOG(str) \
        Logger::getGlobalLogger()->pushLog(LogEvent(WARN, __FILE__, __LINE__), str); \

    #define ERROR_LOG(str) \
        Logger::getGlobalLogger()->pushLog(LogEvent(ERROR, __FILE__, __LINE__), str); \

    #define FATAL_LOG(str) \
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
        // 默认为INFO级别
        Logger(LogLevel level = INFO);

        typedef std::shared_ptr<Logger> ptr;
        // 输出格式化日志到所有输出地
        void pushLog(LogEvent l, const char* format, ...);
        // 输出日志到所有输出地
        void pushLog(LogEvent l, std::string str);
        // 得到日志器
        static std::shared_ptr<Logger> getGlobalLogger();
        // 初始化操作
        void init();
        // 设置日志器级别
        void setLogLevel(LogLevel l);
        // 测试
        LogLevel getlevel() const { return m_level;}
    private:
        // 日志器级别，输出级别不小于该级别的日志
        LogLevel m_level;
    };

    //事件类
    class LogEvent {
    public:
        // 事件构造器，自动获取当前时间与进程与线程号
        LogEvent(LogLevel l, std::string file_name, int file_line,
            int pid = getPid(), int thread_id = getThreadId(), std::string dateTime = getDateTime()) : 
                m_level(l),
                m_file_name(file_name),
                m_file_line(std::to_string(file_line)),
                m_pid(pid),
                m_thread_id(thread_id),
                m_dateTime(dateTime) { }
        // 将日志时间转为字符串
        std::string toString();
        // 将日志级别转化为字符串
        static std::string stringFromLogLevel(LogLevel level);
        // 从字符串得到日志级别
        static LogLevel logLevelFromString(std::string level);
        // 推送日志
        void pushLog(LogEvent l, const char* format, ...);
        // 得到事件级别
        LogLevel getLevel() const { return m_level;}
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
