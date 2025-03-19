#ifndef __TRANCE_PROTOCOL_H__
#define __TRANCE_PROTOCOL_H__
#include<stddef.h>
#include<string>
#include<string.h>
#include<stdint.h>

namespace trance {

    enum Status {
        SUCCESS = 1,
        FAILED = 2
    };

    class Request {
    public:
        // 构造函数，反序列化
        Request() = delete;
        Request(void* data);
        Request(uint8_t func, std::string r_data, std::string message);
        ~Request();
        // 序列化
        void serialization(void* dest);
        // 消息体大小
        size_t size() { return length;}
    public:
        // 请求体总长
        uint16_t length;
        // 状态码
        Status status;
        // 请求调用函数号
        uint8_t fun_code;
        // 参数长度
        uint8_t paras_len;
        // 请求函数参数
        std::string res_data;
        // 信息长度
        uint8_t mess_len {0};
        // 信息
        std::string message;
        // 标志位
        uint32_t icon {0xffffffff};
    };

    class Response {
    public:
        // 构造函数，反序列化
        Response() = delete;
        Response(void* data);
        Response(uint8_t type, std::string r_data, std::string mess);
        ~Response();
        // 序列化
        void serialization(void* dest);
        // 消息体大小
        size_t size() { return length;}
    public:
        // 响应体总长
        uint16_t length;
        // 状态码
        Status status;
        // 响应参数类型
        uint8_t para_type;
        // 响应参数长度
        uint16_t para_len;
        // 响应数据
        std::string rsp_data;
        // 消息长度
        uint8_t mess_len;
        // 信息
        std::string message;
        // 标志位
        uint32_t icon {0xffffffff};
    };
}

#endif