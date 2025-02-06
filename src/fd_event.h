#ifndef __FDEVENT_H__
#define __FDEVENT_H__
#include<functional>
#include<sys/epoll.h>
#include<fcntl.h>
#include<memory>

namespace trance {
    class FdEvent{
    public:
        typedef std::shared_ptr<FdEvent> ptr;
        // 有参构造
        FdEvent(int fd);
        // 无参构造
        FdEvent();
        // 析构函数
        ~FdEvent() {}
        // 设置回调函数
        void listen(std::function<void()> callback, int type);
        // 根据触发事件类型获得处理函数
        std::function<void()> handle(int type);
        // 获取文件描述符
        int getFd() const { return m_fd;}
        // 设置文件描述符
        void setFd(int fd);
        // 取消事件
        void cancleEvent(int type);
        // 获得事件结构体
        epoll_event getEpollEvent() { return m_event;}
        // 设置非阻塞模式
        void setNonBlock();
    private:
        // 相关联的文件描述符
        int m_fd = -1;
        // epoll事件结构体
        epoll_event m_event;
        // 写事件回调函数
        std::function<void()> m_write_callback = nullptr;
        // 读事件回调函数
        std::function<void()> m_read_callback = nullptr;
    };

}

#endif