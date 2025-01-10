#include "config.h"

namespace trance {
    static config::ptr m_global_config = nullptr;

    config::ptr config::getGlobalConfig() {
        if(m_global_config == nullptr) {
            m_global_config = std::make_shared<config>();
        }
        return m_global_config;
    }
    void config::insertConfig(std::string&& key, std::string&& val) {
        m_configs.insert({key, val});
    }
}