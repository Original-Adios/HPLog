#include "sinks/console_sink.h"

namespace logger
{
    constexpr char kLogLevelStr[] = "TDIWEF";
    void ConsoleSink::Log(const LogMsg &msg)
    {
        std::cout << "["
                  << kLogLevelStr[static_cast<int>(msg.level)] << "] "
                  << msg.location.func_name << ":" << msg.location.line << " "
                  << msg.message << "\n";
    }
}