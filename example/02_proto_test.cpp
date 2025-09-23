#include <iostream>
#include <fstream>
#include <chrono>
#include "effective_msg.pb.h" // 根据生成位置调整 include 路径
#include <netinet/in.h>

int main()
{
    // 初始化（对于 lite runtime 不需要 Google protobuf 的全局初始化；使用完整版可调用）
    EffectiveMsg msg;
    msg.set_level(2); // e.g. INFO
    msg.set_timestamp(std::chrono::duration_cast<std::chrono::milliseconds>(
                          std::chrono::system_clock::now().time_since_epoch())
                          .count());
    msg.set_pid(1234);
    msg.set_tid(5678);
    msg.set_line(42);
    msg.set_file_name("foo.cpp");
    msg.set_func_name("do_work");
    msg.set_log_info("this is a test log line");

    // 序列化为二进制字符串
    std::string out;
    if (!msg.SerializeToString(&out))
    {
        std::cerr << "serialize failed\n";
        return 1;
    }

    // 写入文件（示例：写长度前缀 + 数据，便于流式读取）
    std::ofstream ofs("log_record.bin", std::ios::binary | std::ios::app);
    uint32_t len = static_cast<uint32_t>(out.size());
    uint32_t netlen = htonl(len); // network byte order 更通用
    ofs.write(reinterpret_cast<const char *>(&netlen), sizeof(netlen));
    ofs.write(out.data(), out.size());
    ofs.close();

    // 读取示例（从同一文件读回第一个消息）
    std::ifstream ifs("log_record.bin", std::ios::binary);
    if (ifs)
    {
        uint32_t readlen;
        ifs.read(reinterpret_cast<char *>(&readlen), sizeof(readlen));
        readlen = ntohl(readlen);
        std::string inbuf(readlen, '\0');
        ifs.read(&inbuf[0], readlen);
        EffectiveMsg parsed;
        if (parsed.ParseFromString(inbuf))
        {
            std::cout << "Parsed level=" << parsed.level()
                      << " file=" << parsed.file_name()
                      << " msg=" << parsed.log_info() << "\n";
        }
    }

    return 0;
}
