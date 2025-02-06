#include "timer.h"
#include "log.h"

namespace trance {

    TimerEvent::TimerEvent(std::function<void()> callback, uint64_t period, bool loop) : m_callback(callback),
            m_setTime(getMillis() + period),
            m_isLoop(loop),
            m_period(period) {
        
    }

    bool TimerEvent::comparator::operator()(const TimerEvent::ptr& t1, const TimerEvent::ptr& t2) const {
        if(t1 == nullptr || t2 == nullptr) {
            return false;
        }
        if(t1->getSetTime() < t2->getSetTime()) {
            return true;
        }
        return false;
    }

    void Timer::addTimerEvent(TimerEvent::ptr event) {
        m_onTimerTasks.insert(event);
    }

    void Timer::addTimerEvent(uint64_t period, std::function<void()> callback, bool loop) {
        addTimerEvent(std::make_shared<TimerEvent>(callback, period, loop));
    }

    void Timer::delTimerEvent(TimerEvent::ptr event) {
        auto it = m_onTimerTasks.find(event);
        if(it != m_onTimerTasks.end()) {
            m_onTimerTasks.erase(it);
        }
    }

    std::vector<std::function<void()>> Timer::getTimeOutTasks() {
        std::vector<std::function<void()>> tasks;
        uint64_t now = getMillis();
        auto it = m_onTimerTasks.begin();
        std::vector<TimerEvent::ptr> loopEvent;
        while(it != m_onTimerTasks.end() && (*it)->getSetTime() <= now) {
            if((*it)->isLoop()) {
                loopEvent.emplace_back((*it));
            }
            tasks.emplace_back((*it)->getCallBack());
            it = m_onTimerTasks.erase(it);
        }
        now = getMillis();
        for(auto te : loopEvent) {
            uint64_t period = te->getPeriod();
            te->setTime(now + period);
            m_onTimerTasks.insert(te);
        }
        return tasks;
    }

    uint64_t Timer::getNextEventTime() {
        if(m_onTimerTasks.size()) {
            auto it = m_onTimerTasks.begin();
            uint64_t period = (*it)->getSetTime() - getMillis();
            if(period >= 0) {
                return period;
            }
        }
        return -1;
    }
}