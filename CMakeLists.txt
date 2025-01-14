cmake_minimum_required(VERSION 3.28.3)
project(tinychat)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Wno-unused-function -Wno-builtin-macro-redefined")

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
add_executable(test_thread test/test_thread.cc)
add_dependencies(test_thread tinychat)
target_link_libraries(test_thread ${LIB_LIB})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

