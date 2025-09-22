file(GLOB LOGGER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sinks/*.cpp
)

add_library(logger STATIC ${LOGGER_SOURCES})

target_include_directories(logger
    PUBLIC
        ${CMAKE_SOURCE_DIR}          # 让 example 能用 #include "logger/xxx.h"
        ${CMAKE_CURRENT_SOURCE_DIR}        # 让 logger 自己能找到 log_common.h
        ${CMAKE_SOURCE_DIR}/third_party/fmt/include
)
