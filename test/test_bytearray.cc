#include<iostream>
#include<cstdint>
#include "../src/net/bytearray.h"
#include "../src/init.h"
#include "../src/log.h"
using namespace std;
using namespace trance;

std::ostream& operator<<(std::ostream& os, __int128 value) {
    // 将 __int128 拆分为高位和低位（两个 64 位部分）
    const uint64_t lower = static_cast<uint64_t>(value);
    const uint64_t upper = static_cast<uint64_t>(value >> 64);
    return os << "0x" << std::hex << upper << std::hex << lower;
}

int main() {
    ByteArray::ptr ba = make_shared<ByteArray>();
    
#define WRITE(type, val) \
    cout << "write data type:" << #type << " type value: " << val << endl; \

    WRITE(int8_t, -100)
    ba->writeInt8(-100);
    WRITE(uint8_t, 200)
    ba->writeUint8(200);
    WRITE(int16_t, -30000)
    ba->writeInt16(-30000);
    WRITE(uint16_t, 60000)
    ba->writeUint16(60000);
    WRITE(int32_t, -2147483648)
    ba->writeInt32(-2147483648);
    WRITE(uint32_t, 4294967295)
    ba->writeUint32(4294967295);
    WRITE(int64_t, -9223372036854775808)
    ba->writeInt64(-9223372036854775808);
    WRITE(uint64_t, 18446744073709551615)
    ba->writeUint64(18446744073709551615);
    WRITE(float, 1.34)
    ba->writeFlaot(1.34);
    WRITE(double, 1.27273)
    ba->writeDouble(1.27273);
    char* s = "hello world!";
    ba->write(s, 13);
    cout << "write data type:char* type value: " << s << endl;

    cout << "--------------------------------" << endl;
#define READ(type, val) \
    cout << "read data type:" << #type << dec << " type value: " << val << endl; \

    int8_t val = ba->readInt8();
    cout << "read data type:int8_t type value: " << dec << static_cast<int>(val) << endl;
    uint8_t val2 = ba->readUint8();
    cout << "read data type:uint8_t type value: " << dec << static_cast<unsigned int>(val2) << endl;
    int16_t val3 = ba->readInt16();
    cout << "read data type:int16_t type value: " << val3 << endl;
    uint16_t val4 = ba->readUint16();
    READ(uint16_t, val4)
    int32_t val5 = ba->readInt32();
    READ(int32_t, val5)
    uint32_t val6 = ba->readUint32();
    READ(uint32_t, val6)
    int64_t val7 = ba->readInt64();
    READ(int64_t, val7)
    uint64_t val8 = ba->readUint64();
    READ(uint64_t, val8)
    float val9 = ba->readFloat();
    READ(float, val9)
    double val10 = ba->readDouble();
    READ(double, val10)
    char str[13];
    ba->read(str, 13);
    cout << "read data type:char* type value: " << str << endl;
    return 0;
}