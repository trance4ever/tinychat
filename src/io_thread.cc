#include "io_thread.h"
#include "log.h"

namespace trance {

    IOThread::IOThread() {
        int rt = pthread_create(&m_threadIcon, nullptr, &IOThread::Main, this);
        if(rt) {
            FMT_ERROR_LOG("failed to create thread, rt = %d", rt);
            throw std::logic_error("pthread_create error");
        }
        m_initSem.wait();
    }

    IOThread::~IOThread() {
        if(m_reactor) {
            stop();
            delete m_reactor;
        }
    }

    void* IOThread::Main(void* arg) {
        IOThread* curThread = (IOThread*)arg;
        curThread->m_reactor = new Reactor(true);
        curThread->m_reactor->setCurReactor();
        curThread->m_initSem.notify();
        curThread->m_startSem.wait();
        curThread->m_reactor->loop();
        return 0;
    }

    void IOThread::start() {
        m_startSem.notify();
    }

    void IOThread::stop() {
        m_reactor->stop();
        int rt = pthread_join(m_threadIcon, nullptr);
        if(rt) {
            FMT_ERROR_LOG("failed to destory thread, rt = %d", rt)
            throw std::logic_error("pthread_join error");
        }
        m_threadIcon = 0;
    }

}