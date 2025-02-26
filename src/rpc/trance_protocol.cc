#include "trance_protocol.h"
#include "../net/bytearray.h"

namespace trance {

    Request::~Request() {
        // if(parameter) {
        //     free(parameter);
        // }
    }

    Request::Request(void* data) {
        uint32_t idx = 0;
        uint16_t t1;
        memcpy(&t1, data + idx, sizeof(t1));
        idx += sizeof(t1);
        byteswap<uint16_t>()(t1);
        length = t1;
        uint8_t t2;
        memcpy(&t2, data + idx, sizeof(t2));
        idx += sizeof(t2);
        status = (Status)t2;
        memcpy(&fun_code, data + idx, sizeof(fun_code));
        idx += sizeof(fun_code);
        memcpy(&paras_len, data + idx, sizeof(paras_len));
        idx += sizeof(paras_len);
        if(paras_len) {
            char tmpbuf[paras_len];
            parameter = tmpbuf;
            memcpy(parameter, data + idx, paras_len);
            idx += paras_len;
        }
        memcpy(&mess_len, data + idx, sizeof(mess_len));
        idx += sizeof(mess_len);
        if(mess_len) {
            char buf[mess_len];
            memcpy(buf, data + idx, mess_len);
            idx += mess_len;
            message.assign(buf);
        }
        memcpy(&icon, data + idx, sizeof(icon));
    }

    Request::Request(uint8_t func, uint8_t parasLen, void* para, std::string mess) {
        status = SUCCESS;
        fun_code = func;
        paras_len = parasLen;
        parameter = para;
        uint8_t len = mess.size();
        if(len) {
            mess_len = len + 1;
            message = mess;
        }
        length = 10 + paras_len + mess_len;
    }

    void Request::serialization(void* dest) {
        uint16_t t1 = length;
        byteswap<uint16_t>()(t1);
        uint32_t idx = 0;
        memcpy(dest + idx, &t1, sizeof(t1));
        idx += sizeof(t1);
        uint8_t t2 = status;
        memcpy(dest + idx, &t2, sizeof(t2));
        idx += sizeof(t2);
        memcpy(dest + idx, &fun_code, sizeof(fun_code));
        idx += sizeof(fun_code);
        memcpy(dest + idx, &paras_len, sizeof(paras_len));
        idx += sizeof(paras_len);
        memcpy(dest + idx, parameter, paras_len);
        idx += paras_len;
        memcpy(dest + idx, &mess_len, sizeof(mess_len));
        idx += sizeof(mess_len);
        if(mess_len) {
            memcpy(dest + idx, message.c_str(), mess_len);
            idx += (mess_len);
        }
        memcpy(dest + idx, &icon, sizeof(icon));
    }

    Response::~Response() {
        // if(parameter) {
        //     free(parameter);
        // }
    }

    Response::Response(void* data) {
        uint32_t idx = 0;
        uint16_t t1;
        memcpy(&t1, data + idx, sizeof(t1));
        idx += sizeof(t1);
        byteswap<uint16_t>()(t1);
        length = t1;
        uint8_t t2;
        memcpy(&t2, data + idx, sizeof(t2));
        idx += sizeof(t2);
        status = (Status)t2;
        memcpy(&para_type, data + idx, sizeof(para_type));
        idx += sizeof(para_type);
        uint16_t t3;
        memcpy(&t3, data + idx, sizeof(t3));
        idx += sizeof(t3);
        byteswap<uint16_t>()(t3);
        para_len = t3;
        if(para_len) {
            char tmpbuf[para_len];
            parameter = tmpbuf;
            memcpy(parameter, data + idx, para_len);
            idx += para_len;
        }
        memcpy(&mess_len, data + idx, sizeof(mess_len));
        idx += sizeof(mess_len);
        if(mess_len) {
            char buf[mess_len];
            memcpy(buf, data + idx, mess_len);
            idx += mess_len;
            message.assign(buf);
        }
        memcpy(&icon, data + idx, sizeof(icon));
    }

    Response::Response(uint8_t type, uint16_t len, 
            void* para, std::string mess) {
        status = SUCCESS;
        para_type = type;
        para_len = len;
        parameter = para;
        uint16_t ml = mess.size();
        if(ml) {
            mess_len = ml + 1;
            message = mess;
        }
        length = 11 + para_len + mess_len;
    }

    void Response::serialization(void* dest) {
        uint16_t t1 = length;
        byteswap<uint16_t>()(t1);
        uint32_t idx = 0;
        memcpy(dest + idx, &t1, sizeof(t1));
        idx += sizeof(t1);
        uint8_t t2 = status;
        memcpy(dest + idx, &t2, sizeof(t2));
        idx += sizeof(t2);
        memcpy(dest + idx, &para_type, sizeof(para_type));
        idx += sizeof(para_type);
        uint16_t t3 = para_len;
        byteswap<uint16_t>()(t3);
        memcpy(dest + idx, &t3, sizeof(t3));
        idx += sizeof(t3);
        memcpy(dest + idx, parameter, para_len);
        idx += para_len;
        memcpy(dest + idx, &mess_len, sizeof(mess_len));
        idx += sizeof(mess_len);
        if(mess_len) {
            memcpy(dest + idx, message.c_str(), mess_len);
            idx += (mess_len);
        }
        memcpy(dest + idx, &icon, sizeof(icon));
    }
}