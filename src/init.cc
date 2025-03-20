#include "init.h"
#include "log.h"
using namespace easyconfig;
namespace trance {
    static Initializer initialization;

    Initializer::Initializer() {
        Config::ptr g_config_ptr = Config::getGlobalConfig();
        __GETNODE__("../src/easyconfig/config.txt", nodes)
        std::map<std::string, std::string> configs = g_config_ptr->getMap();
        for(auto list : nodes) {
            for(auto pair : list) {
                g_config_ptr->insertConfig(move(pair.first), move(pair.second));
            }
        }
        // 设置日志器级别
        Logger::getGlobalLogger()->setLogLevel(g_config_ptr->getConfig<LogLevel>("Logger::m_level"));
        // 添加std日志输出地
        Logger::getGlobalLogger()->addLoggerAppend(std::make_shared<StdLoggerAppend>());
        // 添加文件日志输出地
        std::string path = g_config_ptr->getConfig<std::string>("Logger::m_fileLogAppend");
        Logger::getGlobalLogger()->addLoggerAppend(std::make_shared<FileLoggerAppend>(path));
        // function中添加函数指针
        // getMap()[Functoin::__Myadd] = Myadd;
    }
}