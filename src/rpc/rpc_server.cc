#include "rpc_server.h"
#include "../log.h"
#include "../init.h"

namespace trance {
    // IO线程个数
    static int g_iothread_num = 7;

    RPCServer::~RPCServer() {
        for(int i = 0; i < m_iothreads.size(); ++i) {
            delete m_iothreads[i];
        }
        if(m_reactor) {
            delete m_reactor;
        }
    }

    RPCServer::RPCServer() {
        m_iothreads.resize(g_iothread_num);
        m_reactor = new Reactor();
        for(int i = 0; i < m_iothreads.size(); ++i) {
            m_iothreads[i] = new IOThread();
        }
        init();
    }

    void RPCServer::init() {
        m_server = Socket::createIPv4TCP();
        int port = Config::getGlobalConfig()->getConfig<int>("RPCServer::port");
        m_server->bind(port);
        m_server->listen();
    }

    void RPCServer::start() {
        FMT_INFO_LOG("server starting, iothread num: %d", g_iothread_num)
        // 监听必要的事件
        FdEvent* mainEvent = new FdEvent(m_server->getSock());
        mainEvent->listen(std::bind(&RPCServer::OnAccept, this), EPOLLIN);
        m_reactor->addEpollEvent(mainEvent);
        // 开启循环
        for(int i = 0; i < m_iothreads.size(); ++i) {
            m_iothreads[i]->start();
        }
        m_reactor->loop();
    }

    void RPCServer::stop() {
        for(int i = 0; i < m_iothreads.size(); ++i) {
            m_iothreads[i]->stop();
        }
    }

    void RPCServer::OnAccept() {
        Socket::ptr client = m_server->accept();
        int idx = threadIdx++ % m_iothreads.size();
        IOThread* thread = m_iothreads[idx];
        FdEvent* clientEvent = new FdEvent(client->getSock());
        SocketStream::ptr session = std::make_shared<SocketStream>(client);
        auto f = [=]() {
            OnRead(std::make_shared<ByteArray>(), std::make_shared<ByteArray>(), session);
        };

        clientEvent->listen(f, EPOLLIN);
        clientEvent->setEpollOneShot();
        thread->getReactor()->addEpollEvent(clientEvent);
        FMT_INFO_LOG("new connected %s", client->printInfo().c_str())
    }

    void RPCServer::OnRead(ByteArray::ptr ba, ByteArray::ptr sba, SocketStream::ptr session) {
        int rt = session->read(ba, 2);
        if(rt == 0) {
            FMT_INFO_LOG("client disconnect, info:%s", session->getSocket()->printInfo().c_str())
            Reactor::getCurReactor()->delEpollEvent(session->getSocket()->getSock());
            return;
        }
        uint16_t length;
        ba->readOnly(&length, 2);
        byteswap<uint16_t>()(length);
        rt = session->read(ba, length - 2);
        if(rt == 0) {
            FMT_INFO_LOG("client disconnect, info:%s", session->getSocket()->printInfo().c_str())
            Reactor::getCurReactor()->delEpollEvent(session->getSocket()->getSock());
            return;
        }
        char buf[length];
        ba->read(buf, length);
        Request r(buf);
        FMT_INFO_LOG("get client request, message: %s", r.message.c_str())
        /*
            调用rpc函数，计算响应体
        */
        Response res(1, 0, NULL, std::string("OK"));
        length = res.size();
        char buf2[length];
        res.serialization(buf2);
        sba->write(buf2, length);
        session->write(sba, length);
        //OnWrite(wba, wsession, res);
        // auto f = [&]() {
        //     OnWrite(ba, session, res);
        // };
        // Reactor::getCurReactor()->getTimer()->addTimerEvent(std::make_shared<TimerEvent>(f, 0));
    }

    void RPCServer::OnWrite(std::weak_ptr<ByteArray> wba, std::weak_ptr<SocketStream> wsession, Response& res) {
        // uint16_t length = res.size();
        // char buf[length];
        // res.serialization(buf);
        // ba->write(buf, length);
        // session->write(wba, length);
    }

}