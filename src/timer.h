#ifndef __TIMER_H__
#define __TIMER_H__
#include<set>
#include<memory>
#include<chrono>
#include<iostream>
#include<functional>
#include<vector>
#include "util.h"
#include "thread.h"

namespace trance {

    // 定时事件
    class TimerEvent {
    public:
        typedef std::shared_ptr<TimerEvent> ptr;
        // 构造函数
        TimerEvent(std::function<void()> callback, uint64_t period, bool loop = false);
        // 获得目标时间
        uint64_t getSetTime() { return m_setTime;}
        // 设定目标时间
        void setTime(uint64_t time) { m_setTime = time;}
        // 查看是否是循环定时任务
        bool isLoop() { return m_isLoop;}
        // 得到定时任务
        std::function<void()> getCallBack() { return m_callback;}
        // 得到间隔时间
        uint64_t getPeriod() { return m_period;}
        // 比较方法
        struct comparator {
            bool operator()(const TimerEvent::ptr& t1, const TimerEvent::ptr& t2) const;
        };
    private:
        // 目标时间
        uint64_t m_setTime;
        // 间隔时间
        uint64_t m_period;
        // 定时任务
        std::function<void()> m_callback;
        // 循环定时器标志位
        bool m_isLoop;
    };

    // 定时器
    class Timer {
    public:
        // 得到所有期满定时任务
        std::vector<std::function<void()>> getTimeOutTasks();
        // 添加一个定时任务
        void addTimerEvent(TimerEvent::ptr event);
        // 添加一个定时任务
        void addTimerEvent(uint64_t period, std::function<void()> callback, bool loop = false);
        // 删除一个定时任务
        void delTimerEvent(TimerEvent::ptr event);
        // 得到最近定时任务时间间隔
        uint64_t getNextEventTime();
    private:
        // 锁资源
        Spinlock m_lock;
        // 存放定时任务
        std::set<TimerEvent::ptr, TimerEvent::comparator> m_onTimerTasks;
    };

}

#endif