#include "log.h"


namespace trance {
    
    static Logger::ptr g_logger = nullptr;

    Logger::Logger(LogLevel level) : m_level(level) {
        init();
    }

    void Logger::init() {

    }

    void Logger::pushLog(LogEvent l, std::string str) {
        if(l.getLevel() >= m_level) {
            std::cout << l.toString() << "\t" << str << std::endl; 
        }
    }

    void Logger::setLogLevel(LogLevel l) {
        this->m_level = l;
    }

    std::shared_ptr<Logger> Logger::getGlobalLogger() {
        if(g_logger == nullptr) {
            g_logger = std::make_shared<Logger>();
        }
        return g_logger;
    }

    std::string LogEvent::stringFromLogLevel(LogLevel level) {
        switch(level) {
            case INFO:
            return "INFO";
            case ERROR:
            return "ERROR";
            case WARN:
            return "WARN";
            case FATAL:
            return "FATAL";
            case UNKOWN:
            return "UNKOWN";
        }
        return "UNKOWN";
    }

    LogLevel LogEvent::logLevelFromString(std::string level) {
        if(level == "INFO") { return LogLevel::INFO;}
        else if(level == "ERROR") { return LogLevel::ERROR;}
        else if(level == "WARN") { return LogLevel::WARN;}
        else if(level == "FATAL") { return LogLevel::FATAL;}
        return LogLevel::UNKOWN;
    }

    std::string LogEvent::toString() {
        std::stringstream ss;
        ss << "[" << stringFromLogLevel(m_level) << "]" << " "
            << "[" << m_dateTime << "]" << " " 
            << "[" << m_pid << ":"
            << m_thread_id << "]" << " "
            << m_file_name << ":" 
            << m_file_line;
        return ss.str();
    }
}

