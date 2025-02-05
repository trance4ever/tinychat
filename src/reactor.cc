#include<string.h>
#include "reactor.h"
#include "log.h"

namespace trance {

    static int g_max_epoll_wait_time = 10000;
    static int g_max_epoll_event = 10;

    Reactor::Reactor() {
        m_epoll_fd = epoll_create(10);
        if(m_epoll_fd < 0) {
            ERROR_LOG("failed to create epoll")
            return;
        }
    }
    
    void Reactor::addEpollEvent(FdEvent* fe) {
        int op = EPOLL_CTL_ADD, fd = fe->getFd();
        if(m_listen_fds.count(fd)) {
            op = EPOLL_CTL_MOD;
        }
        epoll_event tmp = fe->getEpollEvent();
        int rt = epoll_ctl(m_epoll_fd, op, fd, &tmp);
        if(rt == -1) {
            FMT_ERROR_LOG("failed to add epoll event, fd = %d, errno = %d, error = %s", fe->getFd(), errno, strerror(errno))
        }
        m_listen_fds.insert(fd);
    }

    void Reactor::delEpollEvent(FdEvent* fe) {
        int fd = fe->getFd();
        epoll_event tmp = fe->getEpollEvent();
        int rt = epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, fd, &tmp);
        if(rt == -1) {
            FMT_ERROR_LOG("failed to add epoll event, fd = %d, errno = %d, error = %s", fe->getFd(), errno, strerror(errno))
        }
        m_listen_fds.erase(fd);
    }

    void Reactor::addTask(std::function<void()> cb) {
        m_tasks.push(cb);
    }

    void Reactor::loop() {
        while(!m_isStop) {
            ScopedLock<Spinlock> lock(m_lock);
            std::queue<std::function<void()>> tasks;
            lock.unlock();
            m_tasks.swap(tasks);
            while(!tasks.empty()) {
                std::function<void()> cb = tasks.front();
                tasks.pop();
                if(cb) {
                    cb();
                }
            }
            epoll_event events[g_max_epoll_event];
            int rt = epoll_wait(m_epoll_fd, events, g_max_epoll_event, g_max_epoll_wait_time);
            if(rt > 0) {
                for(int i = 0; i < rt; ++i) {
                    epoll_event e = events[i];
                    FdEvent* fe = static_cast<FdEvent*>(e.data.ptr);
                    if(fe == nullptr) {
                        continue;
                    }
                    if(e.events & EPOLLIN) {
                        addTask(fe->handle(EPOLLIN));
                    }
                    if(e.events & EPOLLOUT) {
                        addTask(fe->handle(EPOLLOUT));
                    }
                }
            }
        }
    }
}