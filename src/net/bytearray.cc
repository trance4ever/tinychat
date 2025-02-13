#include<string>
#include<string.h>
#include "bytearray.h"
#include "../log.h"

namespace trance {

    static uint32_t g_buffer_size = 4096;
    
    Buffer::Buffer(uint32_t s)
            :  size(s),
            ptr(new char[size]),
            next(nullptr) {

    }
    
    Buffer::Buffer() 
            : size(g_buffer_size),
            ptr(new char[size]),
            next(nullptr) {

    }

    Buffer::~Buffer() {
        if(ptr) {
            delete[] ptr;
        }
    }

    ByteArray::ByteArray() 
            : m_write_idx(0),
              m_read_idx(0),
              m_readable(0),
              m_writeable(g_buffer_size),
              m_root(new Buffer()),
              m_cur(m_root) {

    }

    ByteArray::~ByteArray() {
        Buffer* tmp = m_root;
        while(tmp) {
            m_cur = tmp;
            delete m_cur;
            tmp = tmp->next;
        }
    }

    void ByteArray::write(const void* data, uint32_t size) {
        if(m_writeable < size) {
            Buffer* last = m_cur;
            while(m_cur->next) {
                last = m_cur->next;
            }
            while(m_writeable < size) {
                Buffer* next = new Buffer();
                last->next = next;
                m_writeable += g_buffer_size;
                last = last->next;
            }
        }
        uint32_t remain = size, hadWrite = 0;
        while(remain) {
            uint32_t writeableOnCurBuffer = m_cur->size - m_write_idx;
            if(writeableOnCurBuffer >= remain) {
                memcpy(m_cur->ptr + m_write_idx, (const char*)data + hadWrite, remain);
                m_write_idx += remain;
                m_writeable -= remain;
                remain = 0;
            }
            else {
                memcpy(m_cur->ptr + m_write_idx, (const char*)data + hadWrite, writeableOnCurBuffer);
                remain -= writeableOnCurBuffer;
                m_cur = m_cur->next;
                m_write_idx = 0;
                hadWrite += writeableOnCurBuffer;
                m_writeable -= writeableOnCurBuffer;
            }
        }
        m_readable += size;
    }

    void ByteArray::read(void* buf, uint32_t size) {
        if(m_readable < size) {
            FMT_ERROR_LOG("read() error, need read date size: %d, readable date size: %d", size, m_readable)
            return;
        }
        uint32_t remain = size, hasRead = 0;
        while(remain) {
            uint32_t readableOnCurBuffer = m_root->size - m_read_idx;
            if(readableOnCurBuffer >= remain) {
                memcpy((char*)buf + hasRead, m_root->ptr + m_read_idx, remain);
                m_read_idx += remain;
                remain = 0;
                if(m_read_idx == g_buffer_size) {
                    m_read_idx = 0;
                    Buffer* tmp = m_root;
                    m_root = m_root->next;
                    delete tmp;
                }
            }
            else {
                memcpy((char*)buf + hasRead, m_root->ptr + m_read_idx, readableOnCurBuffer);
                remain -= readableOnCurBuffer;
                hasRead += readableOnCurBuffer;
                m_read_idx = 0;
                Buffer* tmp = m_root;
                m_root = m_root->next;
                delete tmp;
            }
        }
        m_readable -= size;
    }

    void ByteArray::readOnly(void* buf, uint32_t size) {
        if(m_readable < size) {
            FMT_ERROR_LOG("read() error, need read date size: %d, readable date size: %d", size, m_readable)
            return;
        }
        uint32_t remain = size, hasRead = 0, this_read_idx = m_read_idx;
        Buffer* root = m_root;
        while(remain) {
            uint32_t readableOnCurBuffer = root->size - this_read_idx;
            if(readableOnCurBuffer >= remain) {
                memcpy((char*)buf + hasRead, root->ptr + this_read_idx, remain);
                remain = 0;
            }
            else {
                memcpy((char*)buf + hasRead, root->ptr + this_read_idx, readableOnCurBuffer);
                remain -= readableOnCurBuffer;
                hasRead += readableOnCurBuffer;
                this_read_idx = 0;
                root = root->next;
            }
        }
    }

    void ByteArray::writeInt8(int8_t val) {
        write(&val, sizeof(val));
    }

    void ByteArray::writeUint8(uint8_t val) {
        write(&val, sizeof(val));
    }

    #define WRITE(type) \
        byteswap<type>()(val); \
        write(&val, sizeof(val)); \ 
    
    void ByteArray::writeInt16(int16_t val) {
        WRITE(int16_t)
    }

    void ByteArray::writeUint16(uint16_t val) {
        WRITE(uint16_t)
    }

    void ByteArray::writeInt32(int32_t val) {
        WRITE(int32_t)
    }

    void ByteArray::writeUint32(uint32_t val) {
        WRITE(uint32_t)
    }

    void ByteArray::writeInt64(int64_t val) {
        WRITE(int64_t)
    }

    void ByteArray::writeUint64(uint64_t val) {
        WRITE(uint64_t)
    }

    void ByteArray::writeFlaot(float val) {
        uint32_t tmp;
        memcpy(&tmp, &val, sizeof(val));
        writeUint32(tmp);
    }

    void ByteArray::writeDouble(double val) {
        uint64_t tmp;
        memcpy(&tmp, &val, sizeof(val));
        writeUint64(tmp);
    }

    int8_t ByteArray::readInt8() {
        int8_t res;
        read(&res, sizeof(res));
        return res;
    }

    uint8_t ByteArray::readUint8() {
        uint8_t res;
        read(&res, sizeof(res));
        return res;
    }

    #define READ(type) \
        type val; \
        read(&val, sizeof(val)); \
        byteswap<type>()(val); \
        return val; \

    int16_t ByteArray::readInt16() {
        int16_t val;
        read(&val, sizeof(val));
        byteswap<int16_t>()(val);
        return val;
    }

    uint16_t ByteArray::readUint16() {
        READ(uint16_t)
    }

    int32_t ByteArray::readInt32() {
        READ(int32_t)
    }

    uint32_t ByteArray::readUint32() {
        READ(uint32_t)
    }

    int64_t ByteArray::readInt64() {
        READ(int64_t)
    }

    uint64_t ByteArray::readUint64() {
        READ(uint64_t)
    }

    float ByteArray::readFloat() {
        uint32_t tmp = readUint32();
        float val;
        memcpy(&val, &tmp, sizeof(val));
        return val;
    }

    double ByteArray::readDouble() {
        uint64_t tmp = readUint64();
        double val;
        memcpy(&val, &tmp, sizeof(val));
        return val;
    }
}