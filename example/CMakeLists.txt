add_executable(01_console_sink 01_console_sink.cpp)

target_link_libraries(01_console_sink
    PRIVATE
        logger
)
