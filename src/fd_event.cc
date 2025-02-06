#include<iostream>
#include<string.h>
#include "fd_event.h"
#include "log.h"

namespace trance {
    
    FdEvent::FdEvent(int fd) : m_fd(fd) {
        memset(&m_event, 0, sizeof(m_event));
        m_event.events |= EPOLLET;
        setNonBlock();
    }

    FdEvent::FdEvent() {
        memset(&m_event, 0, sizeof(m_event));
        m_event.events |= EPOLLET;
    }

    void FdEvent::listen(std::function<void()> callback, int type) {
        if(type == EPOLLIN) {
            m_event.events |= EPOLLIN;
            m_read_callback = callback;
        }
        else if(type == EPOLLOUT) {
            m_event.events |= EPOLLOUT;
            m_write_callback = callback;
        }
        else {
            FMT_ERROR_LOG("event type error, type: %d", type);
        }
        m_event.data.ptr = this;
    }

    std::function<void()> FdEvent::handle(int type) {
        if(type == EPOLLIN) {
            return m_read_callback;
        }
        else if(type == EPOLLOUT) {
            return m_write_callback;
        }
        auto f = [=]() {
            std::cout << "failed to get handle callback, cause event type is error: " << type << std::endl;
        };
        return f;
    }

    void FdEvent::cancleEvent(int type) {
        if(type == EPOLLIN) {
            m_event.events &= (~EPOLLIN);
        }
        else if(type == EPOLLOUT) {
            m_event.events &= (~EPOLLOUT);
        }
    }

    void FdEvent::setNonBlock() {
        int flag = fcntl(m_fd, F_GETFL, 0);
        if(flag & O_NONBLOCK) {
            return;
        }
        fcntl(m_fd, F_SETFL, flag | O_NONBLOCK);
    }

    void FdEvent::setFd(int fd) {
        m_fd = fd;
        setNonBlock();
    }
}