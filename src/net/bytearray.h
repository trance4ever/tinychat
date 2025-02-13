#ifndef __BYTEARRAY_H__
#define __BYTEARRAY_H__
#include<memory>

namespace trance {

    // 数据缓冲区结构体
    struct Buffer {
        // 创建指定大小的缓冲区
        Buffer(uint32_t size);
        // 创建默认大小的缓冲区
        Buffer();
        ~Buffer();
        // 当前指针
        char* ptr;
        // 当前缓冲区大小
        uint32_t size;
        Buffer* next;
    };
    // 转换字节序
    template<typename T>
    class byteswap {
    public:
        void operator()(T& val) {
            uint8_t size = sizeof(val);
            if(size == 2) {
                val = ((val >> 8) & 0x00FF) | ((val << 8) & 0xFF00);
            }
            else if(size == 4) {
                val = ((val >> 24) & 0x000000FF) | 
                      ((val >> 8)  & 0x0000FF00) | 
                      ((val << 8)  & 0x00FF0000) | 
                      ((val << 24) & 0xFF000000);
            }
            else if(size == 8) {
                val = ((val >> 56) & 0x00000000000000FFULL) | 
                      ((val >> 40) & 0x000000000000FF00ULL) |
                      ((val >> 24) & 0x0000000000FF0000ULL) |
                      ((val >> 8)  & 0x00000000FF000000ULL) | 
                      ((val << 8)  & 0x000000FF00000000ULL) |
                      ((val << 24) & 0x0000FF0000000000ULL) |
                      ((val << 40) & 0x00FF000000000000ULL) |
                      ((val << 56) & 0xFF00000000000000ULL);
            }
        }
    };
    // 字节序类，与缓冲区交互数据
    class ByteArray {
    public:
        typedef std::shared_ptr<ByteArray> ptr;
        ByteArray();
        ~ByteArray();
        // 向数据缓冲区写入数据
        void write(const void* data, uint32_t size);
        // 从数据缓冲区读取数据
        void read(void* buf, uint32_t size);
        // 从数据缓冲区仅读数据
        void readOnly(void* buf, uint32_t size);
        // 写入基本类型数据
        void writeInt8(int8_t val);
        void writeUint8(uint8_t val);
        void writeInt16(int16_t val);
        void writeUint16(uint16_t val);
        void writeInt32(int32_t val);
        void writeUint32(uint32_t val);
        void writeInt64(int64_t val);
        void writeUint64(uint64_t val);
        void writeFlaot(float val);
        void writeDouble(double val);
        // 读取基本类型数据
        int8_t   readInt8();
        uint8_t  readUint8();
        int16_t  readInt16();
        uint16_t readUint16();
        int32_t  readInt32();
        uint32_t readUint32();
        int64_t  readInt64();
        uint64_t readUint64();
        float readFloat();
        double readDouble();
        // 获取可读大小
        size_t getReadable() { return m_readable;}
    private:
        // 写指针
        uint32_t m_write_idx;
        // 读指针
        uint32_t m_read_idx;
        // 可读数据大小
        size_t m_readable;
        // 可写数据大小
        size_t m_writeable;
        // 数据链表根节点
        Buffer* m_root;
        // 数据链表当前节点
        Buffer* m_cur;
    };

}

#endif