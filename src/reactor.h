#ifndef __REACTOR_H__
#define __REACTOR_H__
#include<functional>
#include<queue>
#include<memory>
#include<sys/epoll.h>
#include<unordered_set>
#include "thread.h"
#include "log.h"
#include "fd_event.h"
#include "timer.h"

namespace trance {
    class Reactor {
    public:
        typedef std::shared_ptr<Reactor> ptr;
        // 构造函数
        Reactor(bool needTimer = false);
        // 析构函数
        ~Reactor();
        // 循环处理事件
        void loop();
        // 添加事件
        void addEpollEvent(FdEvent* fe);
        // 删除事件
        void delEpollEvent(FdEvent* fe);
        // 添加任务
        void addTask(std::function<void()> cb);
        // 删除任务
        void delTask(std::function<void()> cb);
        // 得到定时器
        Timer* getTimer() {return m_timer;}
    private:
        // 锁资源
        // Spinlock m_lock;
        // epoll对象文件描述符
        int m_epoll_fd;
        // 延迟任务函数
        std::queue<std::function<void()>> m_tasks;
        // 停止标志位
        bool m_isStop = false;
        // 记录已经监听的文件描述符
        std::unordered_set<int> m_listen_fds;
        // 定时器
        Timer* m_timer = nullptr;
    };
}

#endif