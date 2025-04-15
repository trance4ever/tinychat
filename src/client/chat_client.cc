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

    Response ChatClient::recvResponse() {
        // 接收响应体
        int rt = m_session->read(m_buffer, 2);
        if(rt == 0) {
            ERROR_LOG("server disconnected ...")
            return Response(Status::FAILED, "ERROR", "server disconnected");
        }
        uint16_t length;
        m_buffer->readOnly(&length, 2);
        byteswap<uint16_t>()(length);
        rt = m_session->read(m_buffer, length - 2);
        if(rt == 0) {
            ERROR_LOG("server disconnected ...")
            return Response(Status::FAILED, "ERROR", "server disconnected");
        }
        char buf[length];
        m_buffer->read(buf, length);
        Response res(buf);
        return res;
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
        Response res = std::move(recvResponse());
        if(res.rsp_data == "SUCCESS") {
            std::cout << "login success" << std::endl;
        }
        else {
            std::cout << "login failed" << std::endl;
            return;
        }
        m_username = username;
        sync();
        // 处理请求
        while(!m_requests.empty()) {
            system("clear");
            handleRequest(m_requests.front());
            m_requests.pop();
        }
        m_startSem.notify();
        m_interface = 2;
        interface2();
    }

    void ChatClient::quiryUI(std::string& friendname) {
        std::cout << "-----------------------------" << std::endl;
        std::cout << "|    FRIEND   REQUEST       |" << std::endl;
        std::cout << "-----------------------------" << std::endl;
        std::cout << "[" << friendname << "]" << " want to be your friend" << std::endl;
        std::cout << "[Y]:agree       [N]:disagress" << std::endl;
    }

    void ChatClient::handleRequest(std::string& data) {
        int split_idx = data.find_first_of('?');
        char operation = data[0];
        switch(operation) {
            case 'A':
            {
                std::string friendname = data.substr(split_idx + 1);
                while(1) {
                    system("clear");
                    quiryUI(friendname);
                    std::string choice;
                    std::cin >> choice;
                    if(choice == "Y") {
                        // 发送同意添加好友请求
                        std::string data = m_username + "/" + friendname;
                        std::string message = "agree";
                        sendRequest(Function::__Agree, data, message);
                        m_friends.push_back(friendname);
                        break;
                    }
                    else if(choice == "N") {
                        // 发送拒绝添加好友请求
                        std::string data = m_username + "/" + friendname;
                        std::string message = "refuse";
                        sendRequest(Function::__Refuse, data, message);
                        break;
                    }
                    else {
                        std::cout << "invalid choice, press any key to try again" << std::endl;
                        std::string pause;
                        std::cin >> pause;
                    }
                }
                break;
            }
            case 'P':
            {
                std::string friendname = data.substr(split_idx + 1);
                m_friends.push_back(friendname);
                std::string info = "[" + friendname + "]" + " agree to be your friend";
                printInfo(info);
                break;
            }
            case 'R':
            {
                std::string friendname = data.substr(split_idx + 1);
                std::string info = "[" + friendname + "]" + " reject to be your friend";
                printInfo(info);
                break;
            }
            default:
            {
                break;
            }
        }
    }

    void ChatClient::printInfo(std::string& data) {
        system("clear");
        std::cout << "-----------------------------" << std::endl;
        std::cout << "|           INFO             |" << std::endl;
        std::cout << "-----------------------------" << std::endl;
        std::cout << data << std::endl;
        std::cout << "press any key to continue" << std::endl;
        std::string pause;
        std::cin >> pause;
    }

    void ChatClient::sync() {
        // 同步数据
        std::string info2 = "sync";
        sendRequest(Function::__Sync, m_username, info2);
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
        char buf2[length];
        m_buffer->read(buf2, length);
        Response res(buf2);
        // 解析数据
        // 数据格式: 好友1|好友2|....好友n|/好友x?聊天消息|好友y?聊天消息|/操作X?信息|操作?信息|
        int frist_split_idx = res.rsp_data.find_first_of('/'),
                second_split_idx = res.rsp_data.find_last_of('/');
        std::string friendsInfo = res.rsp_data.substr(0, frist_split_idx),
                messagesInfo = res.rsp_data.substr(frist_split_idx + 1, second_split_idx - frist_split_idx - 1),
                operationsInfo = res.rsp_data.substr(second_split_idx + 1);
        // 同步好友数据
        if(friendsInfo.size()) {
            int idx = 0;
            while(idx < friendsInfo.size()) {
                int split_idx = friendsInfo.find_first_of('|', idx);
                m_friends.push_back(friendsInfo.substr(idx, split_idx - idx));
                idx = split_idx + 1;
            }
        }
        // 同步聊天数据
        if(messagesInfo.size()) {
            int idx = 0;
            while(idx < messagesInfo.size()) {
                int split_idx = messagesInfo.find_first_of('|', idx);
                std::string infos = messagesInfo.substr(idx, split_idx - idx);
                int seperate_idx = infos.find_first_of('?');
                std::string friendname = infos.substr(0, seperate_idx);
                std::string message = infos.substr(seperate_idx + 1);
                m_messages[friendname].push_back(message);
                idx = split_idx + 1;
            }
        }
        // 同步请求数据
        if(operationsInfo.size()) {
            int idx = 0;
            while(idx < operationsInfo.size()) {
                int split_idx = operationsInfo.find_first_of('|', idx);
                m_requests.push(operationsInfo.substr(idx, split_idx - idx));
                idx = split_idx + 1;
            }
        }
    }

    void ChatClient::circularRecv() {
        system("clear");
        friendsUI();
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
            ScopedLock<Spinlock> lock(m_lock);
            m_requests.push(res.rsp_data);
            lock.unlock();
            if(res.rsp_data[0] == 'I') {

            }
            system("clear");
            friendsUI();
            std::cout << res.rsp_data << std::endl;
            if(res.rsp_data[0] != 'I') {
                std::cout << "new request arrive, press any anthor key to handle" << std::endl;
            }
        }
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
    }

    void ChatClient::friendsUI() {
        std::cout << "     [" + m_username + "]" << std::endl;
        std::cout << "-----------------------------" << std::endl;
        std::cout << "|       friends list        |" << std::endl;
        std::cout << "| A:add            D:delete |" << std::endl;
        std::cout << "-----------------------------" << std::endl;
        for(int i = 0; i < m_friends.size(); ++i) {
            std::cout << i + 1 << ". " << m_friends[i]
                << "[" << m_messages[m_friends[i]].size() << "]" << std::endl;
        }
    }

    void ChatClient::interface2() {
        std::string choice, username;
        while(1) {
            ScopedLock<Spinlock> lock(m_lock);
            while(!m_requests.empty()) {
                std::string data = m_requests.front();
                m_requests.pop();
                lock.unlock();
                handleRequest(data);
            }
            lock.unlock();
            std::cin >> choice;
            if(choice == "A") {
                std::cout << "input username: ";
                std::cin >> username;
                // 发送添加好友RPC请求
                std::string data = m_username + "/" + username;
                std::string message = "addfriend";
                sendRequest(Function::__AddFriend, data, message);
            }
            else if(choice == "D") {

            }
            else {
            }
        }
    }

    void ChatClient::handleResponse(std::string& data) {
        std::cout << data << std::endl;
    }
}