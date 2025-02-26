cmake_minimum_required(VERSION 3.28.3)
project(tinychat)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++14 -Wall -Wno-deprecated -Wno-unused-function -Wno-builtin-macro-redefined")

include_directories(.)
include_directories(/usr/local/include)

link_directories(/usr/local/lib)

set(LIB_SRC 
	src/init.cc
    src/log.cc
    src/util.cc
    src/easyconfig/config.cc
    src/easyconfig/parse.cc
    src/thread.cc
    src/fd_event.cc
    src/reactor.cc
    src/timer.cc
    src/io_thread.cc
    src/net/address.cc
    src/net/bytearray.cc
    src/net/socket.cc
    src/net/socket_stream.cc
    src/rpc/trance_protocol.cc
)

add_library(tinychat SHARED ${LIB_SRC})

set(LIB_LIB
    tinychat
	pthread
)
# 测试初始化模块
# add_executable(test_init test/test_init.cc)
# add_dependencies(test_init tinychat)
# target_link_libraries(test_init ${LIB_LIB})

# 测试日志模块
# add_executable(test_log test/test_log.cc)
# add_dependencies(test_log tinychat)
# target_link_libraries(test_log ${LIB_LIB})

# 测试解析配置文件模块
# add_executable(test_parse test/test_parse.cc)
# add_dependencies(test_parse tinychat)
# target_link_libraries(test_parse ${LIB_LIB})

# 测试配置模块
# add_executable(test_config test/test_config.cc)
# add_dependencies(test_config tinychat)
# target_link_libraries(test_config ${LIB_LIB})

# 测试线程模块
# add_executable(test_thread test/test_thread.cc)
# add_dependencies(test_thread tinychat)
# target_link_libraries(test_thread ${LIB_LIB})

# 测试反应堆模块
# add_executable(test_reactor test/test_reactor.cc)
# add_dependencies(test_reactor tinychat)
# target_link_libraries(test_reactor ${LIB_LIB})

# 测试定时器模块
# add_executable(test_timer test/test_timer.cc)
# add_dependencies(test_timer tinychat)
# target_link_libraries(test_timer ${LIB_LIB})

# 测试IO线程模块
# add_executable(test_iothread test/test_iothread.cc)
# add_dependencies(test_iothread tinychat)
# target_link_libraries(test_iothread ${LIB_LIB})

# 测试地址类
# add_executable(test_address test/test_address.cc)
# add_dependencies(test_address tinychat)
# target_link_libraries(test_address ${LIB_LIB})

# 测试字节流模块
# add_executable(test_bytearray test/test_bytearray.cc)
# add_dependencies(test_bytearray tinychat)
# target_link_libraries(test_bytearray ${LIB_LIB})

# 测试套接字模块
add_executable(test_socket test/test_socket.cc)
add_dependencies(test_socket tinychat)
target_link_libraries(test_socket ${LIB_LIB})

add_executable(test_socket_client test/test_socket_client.cc)
add_dependencies(test_socket_client tinychat)
target_link_libraries(test_socket_client ${LIB_LIB})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

