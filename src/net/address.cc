#include<string.h>
#include "address.h"
#include "../easyconfig/config.h"
#include "../log.h"

namespace trance {

    IPv4Address::IPv4Address(const sockaddr_in addr)
            : m_addr(addr) {
    }

    IPv4Address::IPv4Address(const std::string& addr) {
        memset(&m_addr, 0, sizeof(m_addr));
        m_addr.sin_family = AF_INET;
        int idx = addr.find_first_of(":");
        if(idx == addr.size()) {
            ERROR_LOG("failed to create IPv4Address, port is null")
            m_valid |= PORTILEGEL;
        }
        int port = FromString<int>()(addr.substr(idx + 1));
        if(port > 65535) {
            ERROR_LOG("failed to create IPV4Address, port is illegal")
            m_valid |= PORTILEGEL;
        }
        m_addr.sin_port = htons(port);
        std::string ip = addr.substr(0, idx);
        in_addr_t m_ip = inet_addr(ip.c_str());
        if(m_ip == INADDR_NONE) {
            ERROR_LOG("failed to create IPV4Address, ip is illegal")
            m_valid |= IPILLEGEL;
        }
        m_addr.sin_addr.s_addr = m_ip;
    }

    IPv4Address::IPv4Address(const std::string& ip, const int port) {
        memset(&m_addr, 0, sizeof(m_addr));
        m_addr.sin_family = AF_INET;
        if(port > 65535 || port < 0) {
            ERROR_LOG("failed to create IPV4Address, port is illegal")
            m_valid |= PORTILEGEL;
        }
        m_addr.sin_port = htons(port);
        in_addr_t m_ip = inet_addr(ip.c_str());
        if(m_ip == INADDR_NONE) {
            ERROR_LOG("failed to create IPV4Address, ip is illegal")
            m_valid |= IPILLEGEL;
        }
        m_addr.sin_addr.s_addr = m_ip;
    }

    IPv4Address::~IPv4Address() {

    }

    Address::ptr IPv4Address::create(const sockaddr* addr, socklen_t len) {
        return std::make_shared<IPv4Address>(*(sockaddr_in*)addr); 
    }

    Address::ptr IPv4Address::create(const std::string& addr) {
        return std::make_shared<IPv4Address>(addr);
    }

    Address::ptr IPv4Address::create(const std::string& ip, const int port) {
        return std::make_shared<IPv4Address>(ip, port);
    }

    int IPv4Address::getFamily() {
        return m_addr.sin_family;
    }

    int IPv4Address::getPort() {
        return ntohs(m_addr.sin_port);
    }

    void IPv4Address::setFamily(int family) {
        m_addr.sin_family = family;
    }

    void IPv4Address::setPort(int port) {
        if(port >=0 && port <= 65535 && (m_valid & PORTILEGEL)) {
            m_valid &= (~PORTILEGEL);
        }
        else if(port < 0 || port > 65536 && ((m_valid & PORTILEGEL) == 0)) {
            ERROR_LOG("the setted port is ilegel")
            m_valid |= PORTILEGEL;
        }
        m_addr.sin_port = htons(port);
    }

    std::string IPv4Address::getIp() {
        char* ip = inet_ntoa(m_addr.sin_addr);
        if(ip == nullptr) {
            ERROR_LOG("failed to get ip, cause the ip is null")
        }
        return std::string(ip);
    }

    void IPv4Address::setIp(const std::string& ip) {
        in_addr_t m_ip = inet_addr(ip.c_str());
        if(m_ip == INADDR_NONE) {
            ERROR_LOG("the setted ip is illegal")
            if((m_valid & IPILLEGEL) == 0) {
                m_valid |= IPILLEGEL;
            }
        }
        else if(m_valid & IPILLEGEL) {
            m_valid &= (~IPILLEGEL);
        }
        m_addr.sin_addr.s_addr = m_ip;
    }

    std::string IPv4Address::toString() {
        return getIp() + ":" + ToString<int>()(getPort());
    }
    
    bool IPv4Address::isValid() {
        return m_valid == 0;
    }
}