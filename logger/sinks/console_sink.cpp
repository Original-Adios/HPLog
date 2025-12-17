#include "sinks/console_sink.h"

namespace logger
{
    constexpr char kLogLevelStr[] = "TDIWEF";
    void ConsoleSink::Log(const LogMsg &msg)
    {
        std::cout << "["
                  << kLogLevelStr[static_cast<int>(msg.level)] << "] "
                  << msg.location.file_name << ":"
                  << msg.location.func_name << ":" << msg.location.line << " "
                  << msg.message << "\n";
    }

    void ConsoleSink::SetFormatter(std::unique_ptr<Formatter> formatter)
    {
        // 如果你需要自定义格式，可以保存 formatter_
        // 目前空实现即可，避免抽象类
    }

    void ConsoleSink::Flush()
    {
        std::cout << std::flush;
    }
}