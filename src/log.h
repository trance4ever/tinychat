#ifndef __TINYCHAT_LOG_H__
#define __TINYCHAT_LOG_H__
#include<iostream>
#include<memory>
#include<vector>
#include<stdarg.h>
#include<string>
#include<fstream>
#include "util.h"

#define MAX_LOG_FILE_SIZE 1024 * 1024 * 10

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

    // 日志级别
    enum LogLevel {
        UNKOWN,
        INFO,
        WARN,
        ERROR,
        FATAL
    };
    class LogEvent;
    // 日志器
    class Logger {
    public:
        // 默认为INFO级别
        Logger(LogLevel level = INFO);

        typedef std::shared_ptr<Logger> ptr;
        // 输出格式化日志到所有输出地
        void pushLog(LogEvent l, const char* format, ...);
        // 输出日志到所有输出地
        void pushLog(LogEvent l, std::string& str);
        // 得到日志器
        static std::shared_ptr<Logger> getGlobalLogger();
        // 初始化操作
        void init();
        // 设置日志器级别
        void setLogLevel(LogLevel l);
        // 测试
        LogLevel getlevel() const { return m_level;}
        // 日志输出地
        class LoggerAppend {
        public:
            // 输出日志
            virtual void push(std::string& str) = 0;
        };
        // 添加日志输出地方
        void addLoggerAppend(std::shared_ptr<LoggerAppend> append);
        // 删除日志输出地方
        void delLoggerAppend(std::shared_ptr<LoggerAppend> append);
    private:
        // 日志器级别，输出级别不小于该级别的日志
        LogLevel m_level;
        // 日志输出地
        std::vector<std::shared_ptr<LoggerAppend>> m_appends;
        
    };
    // std日志输出地
    class StdLoggerAppend : public Logger::LoggerAppend {
    public:
        void push(std::string& str) override;
    };
    // 文件日志输出地
    class FileLoggerAppend : public Logger::LoggerAppend {
    public:
        FileLoggerAppend(std::string str) : m_filepath(str) { }
        // 输出日志到文件
        void push(std::string& str) override;
        // 获得一个合适的文件名
        std::string getFileName();
        // 获得初始化文件名
        std::string getInitFileName();
    private:
        std::string m_filepath;
        std::string m_preFileName;
        std::ofstream m_ofs;
        std::ifstream m_ifs;
    };
    // 事件类
    class LogEvent {
    public:
        // 事件构造器，自动获取当前时间与进程与线程号
        LogEvent(LogLevel l, std::string file_name, int file_line,
            pid_t pid = getPid(), pid_t thread_id = getThreadId(), std::string dateTime = getDateTime()) : 
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
		pid_t m_pid; //进程号
		pid_t m_thread_id; //线程号
        std::string m_dateTime; //时间		
    };
}

#endif
