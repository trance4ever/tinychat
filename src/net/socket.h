#ifndef __SOCKET_H__
#define __SOCKET_H__
#include<memory>
#include<sys/socket.h>
#include <netinet/tcp.h>
#include "address.h"

namespace trance {
    // 套接字类
    class Socket {
    public:
        typedef std::shared_ptr<Socket> ptr;
        enum Type {
            TCP = SOCK_STREAM, 
            UDP = SOCK_DGRAM
        };

        enum Domain {
            IPv4 = AF_INET,
            IPv6 = AF_INET6
        };

        // 构造函数
        Socket(Domain d, Type t, int protocol = 0);
        // 析构函数
        ~Socket();
        // 创建IPv4套接字
        static Socket::ptr createIPv4TCP();
        static Socket::ptr createIPv4UDP(); 
        // 创建IPv6套接字
        static Socket::ptr createIPv6TCP();
        static Socket::ptr createIPv6UDP(); 
        // 设置连接状态
        void setConnect(bool flag) { isConnected = flag;}
        // 初始化，获取套接字描述符
        void init(int sock);
        // 绑定
        void bind(int port);
        // 设置监听
        void listen(int MAXCONNECTION = 4096);
        // 接收连接
        Socket::ptr accept();
        // 连接服务端
        void connect(const Address::ptr server);
        // 套接字设置
        void setOption();
        // 本地地址get set
        Address::ptr getLocalAddress() { return m_localAddress;}
        void setLocalAddress(Address::ptr addr) { m_localAddress = addr;}
        // 对端地址get set
        Address::ptr getRemoteAddress() { return m_remoteAddress;}
        void setRemoteAddress(Address::ptr addr) { m_remoteAddress = addr;}
        // 关闭文件描述符
        void close();
        // 发送数据
        int send(const void* data, size_t length, int flags = 0);
        int send2(const void* data, size_t length, const Address::ptr peer, int flags = 0);
        // 接收数据
        int recv(void* buffer, size_t length, int flags = 0);
        int recvFrom(void* buffer, size_t length, Address::ptr peer, int flags = 0);
        // 输出连接信息
        std::string printInfo();
        // 得到套接字文件描述符
        int getSock() const { return m_sock;}
    private:
        // 是否连接
        bool isConnected;
        // 文件描述符
        int m_sock;
        // 协议簇
        Domain m_domain;
        // 套接字类型
        Type m_type;
        // 协议
        int m_protocol;
        // 本地地址
        Address::ptr m_localAddress;
        // 对端地址
        Address::ptr m_remoteAddress;
    };

}

#endif