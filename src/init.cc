#include "init.h"
#include "log.h"
using namespace easyconfig;
namespace trance {
    static Initializer initialization;

    Initializer::Initializer() {
        config::ptr g_config_ptr = config::getGlobalConfig();
        __GETNODE__("../src/easyconfig/config.txt", nodes)
        std::map<std::string, std::string> configs = g_config_ptr->getMap();
        for(auto list : nodes) {
            for(auto pair : list) {
                g_config_ptr->insertConfig(move(pair.first), move(pair.second));
            }
        }
        // 设置日志器级别
        Logger::getGlobalLogger()->setLogLevel(g_config_ptr->getConfig<LogLevel>("Logger::m_level"));
    }
}