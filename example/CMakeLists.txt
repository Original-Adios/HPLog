# add_executable(01_console_sink 01_console_sink.cpp)

# target_link_libraries(01_console_sink
#     PRIVATE
#         logger
# )

# # 新增 02_proto_test
# add_executable(02_proto_test 02_proto_test.cpp)
# # 链接 logger 静态库
# # 由于用了 PUBLIC，任何链接 logger 的目标都会自动继承 protobuf::libprotobuf-lite，不需要 example 再单独链接
# target_link_libraries(02_proto_test
#     PRIVATE
#         logger
#         # protobuf::libprotobuf-lite   # 如果用 lite runtime
#         # protobuf::libprotobuf     # 如果不是 lite runtime，用这个
# )

# 定义创建可执行程序的函数
function(add_logger_example name source)
    add_executable(${name} ${source})
    target_link_libraries(${name} PRIVATE logger)
endfunction()

# 使用函数
add_logger_example(01_console_sink 01_console_sink.cpp)
add_logger_example(02_proto_test 02_proto_test.cpp)
add_logger_example(03_effective_formatter_test 03_effective_formatter_test.cpp)
add_logger_example(04_mmap_test 04_mmap_test.cpp)