#include "socket_stream.h"
#include "../log.h"


namespace trance {

    SocketStream::SocketStream(Socket::ptr socket) 
            : m_socket(socket) {
        
    }

    SocketStream::~SocketStream() {
        if(m_socket) {
            m_socket->close();
        }
    }

    int SocketStream::read(void* buf, size_t length) {
        return m_socket->recv(buf, length);
    }

    int SocketStream::read(ByteArray::ptr ba, size_t length) {
        char data[length];
        int len = 0;
        while(len < length) {
            int rt = m_socket->recv(data + len, length - len);
            if(rt <= 0) {
                FMT_ERROR_LOG("read error, need %d byte, accept %d byte", length - len, rt)
                return len;
            }
            len += rt;
        }
        // int len = m_socket->recv(data, length);
        ba->write(data, len);
        return len;
    }

    int SocketStream::write(const void* data, size_t length) {
        return m_socket->send(data, length);
    }

    int SocketStream::write(ByteArray::ptr ba, size_t length) {
        char buf[length];
        ba->read(buf, length);
        int len = m_socket->send(buf, length);
        return len;
    }
    
}