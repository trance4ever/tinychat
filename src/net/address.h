#ifndef __ADDRESS_H__
#define __ADDRESS_H__
#include<sys/socket.h>
#include<arpa/inet.h>
#include<memory>
#include<string>

namespace trance {

    class Address {
    public:
        typedef std::shared_ptr<Address> ptr;
        // 构造函数
        Address() {}
        // 析构函数
        ~Address() {}
        // 获取地址族
        virtual int getFamily() = 0;
        // 获取端口
        virtual int getPort() = 0;
        // 设置地址族
        virtual void setFamily(int family) = 0;
        // 设置端口
        virtual void setPort(int port) = 0;
        // 获得字符串
        virtual std::string toString() = 0;
        // 获取ip地址
        virtual std::string getIp() = 0;
        // 设置ip地址
        virtual void setIp(const std::string& ip) = 0;
        // 查看是否合法
        virtual bool isValid() = 0;
    };

    class IPv4Address : public Address {
    public:
        typedef std::shared_ptr<IPv4Address> ptr;
        // 构造函数
        IPv4Address() = delete;
        IPv4Address(const sockaddr_in addr);
        IPv4Address(const std::string& addr);
        IPv4Address(const std::string& ip, const int port);
        // 析构函数
        ~IPv4Address();
        // 根据结构体创建地址类对象
        static Address::ptr create(const sockaddr* addr, socklen_t len);
        // 根据字符串创建地址对象
        static Address::ptr create(const std::string& addr);
        // 根据ip地址和端口创建对象
        static Address::ptr create(const std::string& ip, const int port);
        // 获取地址族
        int getFamily() override;
        // 获取端口
        int getPort() override;
        // 获取ip地址
        std::string getIp() override;
        // 设置ip地址
        void setIp(const std::string& ip) override;
        // 设置地址族
        void setFamily(int family) override;
        // 设置端口
        void setPort(int port) override;
        // 获得字符串
        std::string toString() override;
        // 查看是否合法
        bool isValid() override;
        // 非法类型
        enum ilegel {
            IPILLEGEL = 1,
            PORTILEGEL = 2
        };
    private:
        // 地址类结构体
        sockaddr_in m_addr;
        // 合法标志位
        uint8_t m_valid = 0;
    };

}

#endif