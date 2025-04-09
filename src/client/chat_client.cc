#include "chat_client.h"

namespace trance {

    ChatClient::ChatClient() {
        // 构造即连接服务端，并完成初始化操作
        Socket::ptr socket = Socket::createIPv4TCP();
        socket->connect(IPv4Address::create("127.0.0.1:7400"));
        m_session = std::make_shared<SocketStream>(socket);
        m_buffer = std::make_shared<ByteArray>();
        int rt = pthread_create(&subThread, nullptr, &ChatClient::Main, this);
        if(rt) {
            FMT_ERROR_LOG("failed to create sub thread, rt = %d", rt);
            throw std::logic_error("pthread_create error");
        }
        m_sem.wait();
    }

    ChatClient::~ChatClient() {

    }

    void* ChatClient::Main(void* arg) {
        ChatClient* cc = (ChatClient*)arg;
        cc->m_sem.notify();
        cc->m_startSem.wait();
        cc->circularRecv();
        return 0;
    }

    void ChatClient::start() {
        // 登陆界面
        loginUI();
        std::string username, password;
        std::cin >> username >> password;
        // 发送登陆请求
        std::string data = username + "/" + password;
        std::string message = "login";
        // 发送请求体
        sendRequest(Function::__Login, data, message);
        // 接收响应体
        int rt = m_session->read(m_buffer, 2);
        if(rt == 0) {
            ERROR_LOG("server disconnected ...")
            return;
        }
        uint16_t length;
        m_buffer->readOnly(&length, 2);
        byteswap<uint16_t>()(length);
        rt = m_session->read(m_buffer, length - 2);
        if(rt == 0) {
            ERROR_LOG("server disconnected ...")
            return;
        }
        char buf[length];
        m_buffer->read(buf, length);
        Response res(buf);
        if(res.rsp_data == "SUCCESS") {
            std::cout << "login success" << std::endl;
        }
        else {
            std::cout << "login failed" << std::endl;
            return;
        }
        m_startSem.notify();
    }

    void ChatClient::circularRecv() {
        while(1) {
            // 接收响应体
            int rt = m_session->read(m_buffer, 2);
            if(rt == 0) {
                ERROR_LOG("server disconnected ...")
                return;
            }
            uint16_t length;
            m_buffer->readOnly(&length, 2);
            byteswap<uint16_t>()(length);
            rt = m_session->read(m_buffer, length - 2);
            if(rt == 0) {
                ERROR_LOG("server disconnected ...")
                return;
            }
            char buf[length];
            m_buffer->read(buf, length);
            Response res(buf);
            handleResponse(res.rsp_data);
        }
    }

    void ChatClient::handleResponse(std::string& data) {

    }
    
    void ChatClient::loginUI() {
        std::cout << "-----------------------------" << std::endl;
        std::cout << "|                           |" << std::endl;
        std::cout << "|                           |" << std::endl;
        std::cout << "|           LOGIN           |" << std::endl;
        std::cout << "|  INPUT USERNAME&PASSWORD  |" << std::endl;
        std::cout << "|                           |" << std::endl;
        std::cout << "|                           |" << std::endl;
        std::cout << "|                           |" << std::endl;
        std::cout << "|                           |" << std::endl;
        std::cout << "|                           |" << std::endl;
        std::cout << "|                           |" << std::endl;
        std::cout << "|                           |" << std::endl;
        std::cout << "-----------------------------" << std::endl;
    }

    void ChatClient::sendRequest(Function func, std::string& data, std::string& message) {
        // 发送请求体
        Request r(func, data, message);
        uint16_t length = r.size();
        char buf[length];
        r.serialization(buf);
        m_buffer->write(buf, length);
        m_session->write(m_buffer, length);
        FMT_INFO_LOG("send request, data length: %d", length);
    }
}