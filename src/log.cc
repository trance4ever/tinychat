#include "log.h"


namespace trance {
    
    static Logger::ptr g_logger = nullptr;

    Logger::Logger(LogLevel level) : m_level(level) {
        init();
    }

    void Logger::init() {

    }

    void Logger::pushLog(LogEvent l, std::string str) {
        std::cout << l.toString() << "\t" << str << std::endl; 
    }

    std::shared_ptr<Logger> Logger::getGlobalLogger() {
        if(g_logger == nullptr) {
            g_logger = std::make_shared<Logger>();
        }
        return g_logger;
    }

    std::string LogEvent::stringFromLogLevel() {
        switch(m_level) {
            case INFO:
            return "INFO";
            case ERROR:
            return "ERROR";
            case WARN:
            return "WARN";
            case FATAL:
            return "FATAL";
        }
        return "UNKOWN";
    }

    std::string LogEvent::toString() {
        std::stringstream ss;
        ss << "[" << stringFromLogLevel() << "]" << " "
            << "[" << m_dateTime << "]" << " " 
            << "[" << m_pid << ":"
            << m_thread_id << "]" << " "
            << m_file_name << ":" 
            << m_file_line;
        return ss.str();
    }
}

