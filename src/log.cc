#include "log.h"


namespace trance {
    
    static Logger::ptr g_logger = nullptr;

    Logger::Logger(LogLevel level) : m_level(level) {
        init();
    }

    void Logger::init() {

    }

    void Logger::addLoggerAppend(std::shared_ptr<LoggerAppend> append) {
        m_appends.push_back(append);
    }
    
    void Logger::delLoggerAppend(std::shared_ptr<LoggerAppend> append) {
        auto it = m_appends.begin();
        for(; it != m_appends.end(); ++it) {
            if(*it == append) {
                m_appends.erase(it);
            }
        }
    }

    void StdLoggerAppend::push(std::string& str) {
        std::cout << str << std::endl;
    }

    void FileLoggerAppend::push(std::string& str) {
        std::string fileName = getFileName();
        if(fileName != m_preFileName) {
            m_ofs.close();
            m_ofs.open(fileName, std::ios::app);
        }
        m_ofs << str << std::endl;
        m_preFileName = fileName;
    }

    std::string FileLoggerAppend::getFileName() {
        if(m_preFileName.size() != 0) {
            if(!m_ifs.is_open()) {
                m_ifs.open(m_preFileName);
            }
            // 将文件指针移动到文件末尾
            m_ifs.seekg(0, std::ios::end);
            // 获取文件大小
            std::streamsize fileSize = m_ifs.tellg();
            // 日志文件小于10Mb
            if(fileSize < MAX_LOG_FILE_SIZE) {
                return m_preFileName;
            }
        }
        // 日志文件过大，此时需要新建文件
        if(m_ifs.is_open()) { 
            m_ifs.close();
        }
        // 如果前文件不为零，说明前文件过大，此时需要写入新文件
        if(m_preFileName.size() != 0) {
            int idx = m_preFileName.size() - 1;
            for(; m_preFileName[idx] != '-'; --idx) { }
            std::string fileNamePrefix = m_preFileName.substr(0, idx + 1);
            std::string curIdx = m_preFileName.substr(idx + 1);
            int nextIdx = std::stoi(curIdx) + 1;
            std::string FileName = fileNamePrefix + std::to_string(nextIdx);
            return FileName;
        }
        // 如果前两段未执行，需要初始化
        return getInitFileName();
    }

    std::string FileLoggerAppend::getInitFileName() {
        // 初始化操作，得到第一个文件名称
        std::string initFileNamePrefix = getDate();
        initFileNamePrefix.push_back('-');
        int initIdx = 1;
        for(;;++initIdx) {
            m_ifs.open(initFileNamePrefix + std::to_string(initIdx));
            if(!m_ifs.is_open()) {
                break;
            }
            else {
                // 将文件指针移动到文件末尾
                m_ifs.seekg(0, std::ios::end);
                // 获取文件大小
                std::streamsize fileSize = m_ifs.tellg();
                // 日志文件小于10Mb
                if(fileSize < MAX_LOG_FILE_SIZE) {
                    return initFileNamePrefix + std::to_string(initIdx);
                }
                else {
                    m_ifs.close();
                }
            }
        }
        return m_filepath + initFileNamePrefix + std::to_string(initIdx);
    }

    void Logger::pushLog(LogEvent l, const char* format, ...) {
        if(l.getLevel() >= m_level) {
            std::stringstream ss;
            ss << l.toString() << "\t";
            va_list al;
            va_start(al, format);
            char* buf = nullptr;
            int len = vasprintf(&buf, format, al);
            if(len != -1) {
                ss << std::string(buf, len);
                free(buf);
            }
            va_end(al);
            std::string str = ss.str();
            for(auto append : m_appends) {
                append->push(str);
            }
        }
    }

    void Logger::pushLog(LogEvent l, std::string& str) {
        if(l.getLevel() >= m_level) {
            std::string str = l.toString().append("\t");
            for(auto append : m_appends) {
                append->push(str);
            }
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
            << "[" << m_file_name << ":" 
            << m_file_line << "]";
        return ss.str();
    }
}

