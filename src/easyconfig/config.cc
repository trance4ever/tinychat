#include "config.h"

namespace trance {
    static Config::ptr m_global_config = nullptr;

    Config::ptr Config::getGlobalConfig() {
        if(m_global_config == nullptr) {
            m_global_config = std::make_shared<Config>();
        }
        return m_global_config;
    }
    void Config::insertConfig(std::string&& key, std::string&& val) {
        m_configs.insert({key, val});
    }
}