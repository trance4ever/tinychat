#include<string.h>
#include "reactor.h"
#include "log.h"

namespace trance {
    static thread_local Reactor* cur_reactor_ptr = nullptr;
    static int g_max_epoll_wait_time = 5000;
    static int g_max_epoll_event = 10;

    Reactor* Reactor::getCurReactor() {
        if(cur_reactor_ptr) {
            return cur_reactor_ptr;
        }
        else {
            ERROR_LOG("have no reactor in this thread")
        }
    }

    void Reactor::setCurReactor() {
        cur_reactor_ptr = this;
    }

    Reactor::Reactor(bool needTimer) {
        m_epoll_fd = epoll_create(10);
        if(m_epoll_fd < 0) {
            ERROR_LOG("failed to create epoll")
            return;
        }
        if(needTimer) {
            m_timer = new Timer();
        }
    }
    Reactor::~Reactor() {
        m_isStop = true;
        if(m_timer) {
            delete m_timer;
            m_timer = nullptr;
        }
        m_listen_fds.clear();
        {
            ScopedLock<Spinlock> lock(m_lock);
            while(!m_tasks.empty()) {
                m_tasks.pop();
            }
        }
        if(m_epoll_fd >= 0) {
            close(m_epoll_fd);
            m_epoll_fd = -1;
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
            FMT_ERROR_LOG("failed to del epoll event, fd = %d, errno = %d, error = %s", fe->getFd(), errno, strerror(errno))
        }
        m_listen_fds.erase(fd);
    }

    void Reactor::delEpollEvent(int fd) {
        int rt = epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, fd, NULL);
        if(rt == -1) {
            FMT_ERROR_LOG("failed to del epoll event, fd = %d, errno = %d, error = %s", fd, errno, strerror(errno))
        }
        m_listen_fds.erase(fd);
    }

    void Reactor::addTask(std::function<void()> cb) {
        if(!cb) {
            ERROR_LOG("failed to add task: null call back")
            return;
        }
        m_tasks.push(cb);
    }

    void Reactor::loop() {
        while(!m_isStop) {
            if(m_timer) {
                std::vector<std::function<void()>> timerTasks = m_timer->getTimeOutTasks();
                for(auto t : timerTasks) {
                    m_tasks.push(t);
                }
            }
            if(m_tasks.size()) {
                // ScopedLock<Spinlock> lock(m_lock);
                std::queue<std::function<void()>> tasks;
                // lock.unlock();
                m_tasks.swap(tasks);
                while(!tasks.empty()) {
                    std::function<void()> cb = tasks.front();
                    tasks.pop();
                    if(cb) {
                        cb();
                    }
                }
            }
            int wait_time = g_max_epoll_wait_time;
            if(m_timer) {
                int period = m_timer->getNextEventTime();
                if(period > 0) {
                    wait_time = std::min(wait_time, period);
                }
            }
            epoll_event events[g_max_epoll_event];
            int rt = epoll_wait(m_epoll_fd, events, g_max_epoll_event, wait_time);
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