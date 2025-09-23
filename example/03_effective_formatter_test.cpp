#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

#include "logger/formatter/effective_formatter.h"
#include "effective_msg.pb.h" // 根据生成位置调整 include 路径

void printStandardTime(int64_t timestamp_ms)
{
    // 1. 将毫秒转换为秒
    std::time_t t = timestamp_ms / 1000;

    // 2. 将时间戳转换为本地时间结构
    std::tm tm_time;
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&tm_time, &t); // Windows
#else
    localtime_r(&t, &tm_time); // Linux / POSIX
#endif

    // 3. 格式化输出
    std::cout << std::put_time(&tm_time, "%Y-%m-%d %H:%M:%S")
              << "." << (timestamp_ms % 1000) // 毫秒部分
              << std::endl;
}

std::string formatTimestamp(int64_t timestamp_ms)
{
    // 1. 秒和毫秒
    std::time_t t = timestamp_ms / 1000;
    int ms = timestamp_ms % 1000;

    // 2. 本地时间结构
    std::tm tm_time;
    localtime_r(&t, &tm_time); // Linux/Ubuntu POSIX

    // 3. 字符串流格式化
    std::ostringstream oss;
    oss << std::put_time(&tm_time, "%Y-%m-%d %H:%M:%S")
        << "." << std::setw(3) << std::setfill('0') << ms; // 毫秒补零
    return oss.str();
}

int main()
{
    // 1. 构造日志消息
    logger::SourceLocation SL(__FILE__, __LINE__, __func__);
    logger::LogMsg log_msg(SL, logger::LogLevel::kInfo, "Hello, EffectiveFormatter!");

    // 2. 创建缓冲区
    logger::MemoryBuf buf;

    // 3. 格式化日志
    logger::EffectiveFormatter formatter;
    formatter.Format(log_msg, &buf);

    std::cout << "Serialized log size: " << buf.size() << " bytes" << std::endl;

    // 4. 解析回 Protobuf 对象（模拟接收端）
    EffectiveMsg parsed_msg;
    if (parsed_msg.ParseFromArray(buf.data(), buf.size()))
    {
        std::cout << "Parsed log info: " << parsed_msg.log_info() << std::endl;
        std::cout << "File: " << parsed_msg.file_name()
                  << ", Line: " << parsed_msg.line()
                  << ", Function: " << parsed_msg.func_name() << std::endl;
        std::cout << "Level: " << parsed_msg.level()
                  << ", PID: " << parsed_msg.pid()
                  << ", TID: " << parsed_msg.tid()
                  << ", Timestamp: " << formatTimestamp(parsed_msg.timestamp()) << std::endl;
    }
    else
    {
        std::cerr << "Failed to parse serialized log!" << std::endl;
    }

    return 0;
}