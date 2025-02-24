#ifndef __SOCKET_STREAM_H__
#define __SOCKET_STREAM_H__
#include "socket.h"
#include "bytearray.h"

namespace trance {

    class SocketStream {
    public:
        typedef std::shared_ptr<SocketStream> ptr;
        // 构造函数
        SocketStream(Socket::ptr socket);
        SocketStream() = delete;
        // 析构函数
        ~SocketStream();
        // 将数据读入缓冲区
        int read(void* buf, size_t length);
        int read(ByteArray::ptr ba, size_t length);
        // 将数据从缓冲区发送
        int write(const void* data, size_t length);
        int write(ByteArray::ptr ba, size_t length);
        Socket::ptr getSocket() { return m_socket;}
    private:
        // 拥有的套接字对象
        Socket::ptr m_socket;
    };

}

#endif