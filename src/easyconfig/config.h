#ifndef __CONFIG_H__
#define __CONFIG_H__
#include<string>
#include<map>
#include<memory>
#include<vector>
#include<algorithm>
#include "boost/lexical_cast.hpp"
#include "../log.h"

namespace trance {
    // string -> T
    template<class T>
    class FromString {
    public:
        T operator()(std::string val) {
            return boost::lexical_cast<T>(val);
        }
    };
    // T -> string
    template<class T>
    class ToString {
    public:
        std::string operator()(T val) {
            return boost::lexical_cast<std::string>(val);
        }
    };
    template<>
    class FromString<LogLevel> {
     public:
        LogLevel operator()(std::string val) {
            return LogEvent::logLevelFromString(val);
        }
    };
    class config : public std::enable_shared_from_this<config> {
    public:
        typedef std::shared_ptr<config> ptr;
        // 获取配置信息
        template<class T>
        T getConfig(std::string name) const {
            auto it = m_configs.find(name);
            if(it != m_configs.end()) {
                return FromString<T>()(it->second);
            }
            else {
                FMT_INFO_LOG("not find such config : %s, mp size : %d", name.c_str(), m_configs.size())
            }
            return (T)0;
        }
        // 插入配置项
        void insertConfig(std::string&& key, std::string&& val);
        // 得到配置类容器
        std::map<std::string, std::string> getMap() { return m_configs;}
        // 得到全局配置实体
        static config::ptr getGlobalConfig();
    private:
        // 记录配置信息
        std::map<std::string, std::string> m_configs;
    };
}
#endif