# 先找 protobuf
find_package(Protobuf REQUIRED)

# 生成 pb.cc / pb.h
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS
    ${CMAKE_CURRENT_SOURCE_DIR}/proto/effective_msg.proto
)

file(GLOB LOGGER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sinks/*.cpp
)

# 编译 logger 静态库
add_library(logger STATIC ${LOGGER_SOURCES} ${PROTO_SRCS})

target_include_directories(logger
    PUBLIC
        ${CMAKE_SOURCE_DIR}          # 让 example 能用 #include "logger/xxx.h"
        ${CMAKE_CURRENT_SOURCE_DIR}        # 让 logger 自己能找到 log_common.h
        ${CMAKE_SOURCE_DIR}/third_party/fmt/include
        ${CMAKE_CURRENT_BINARY_DIR}        # 公开proto生成的 pb.h 所在目录
)

target_link_libraries(logger
    PUBLIC
        protobuf::libprotobuf-lite
)