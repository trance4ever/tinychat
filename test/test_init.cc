#include "../src/init.h"
#include "../src/log.h"
using namespace trance;
// static Initializer initialization;
int main() {
    std::map<std::string, std::string> m_configs = config::getGlobalConfig()->getMap();
    for(auto it = m_configs.begin(); it != m_configs.end(); ++it) {
        std::cout << it->first << ":" << it->second << std::endl;
    }
    FMT_INFO_LOG("the loglevel of g_logger is : %s", LogEvent::stringFromLogLevel(Logger::getGlobalLogger()->getlevel()).c_str());
    return 0;
}