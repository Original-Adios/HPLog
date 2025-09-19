#include "sinks/console_sink.h"
#include "log_handle.h"

int main()
{
    auto console_sink = std::make_shared<logger::ConsoleSink>();

    logger::LogHandle logger(console_sink);
    logger.SetLevel(logger::LogLevel::kDebug);
    logger.Log(logger::LogLevel::kDebug, {"example.cpp", 123, __func__}, "Hello, this is an Debug log.");

    return 0;
}
