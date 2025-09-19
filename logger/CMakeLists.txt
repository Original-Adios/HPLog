file(GLOB LOGGER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sinks/*.cpp
)

add_library(logger STATIC ${LOGGER_SOURCES})

target_include_directories(logger
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/logger
        ${CMAKE_SOURCE_DIR}/third_party/fmt/include
)
