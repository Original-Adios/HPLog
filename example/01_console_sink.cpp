#include "logger/sinks/console_sink.h"
#include "logger/log_handle.h"
#include <iostream>

int main()
{
    auto console_sink = std::make_shared<logger::ConsoleSink>();

    logger::LogHandle logger(console_sink);
    logger.SetLevel(logger::LogLevel::kDebug);
    logger.Log(logger::LogLevel::kDebug, logger::SourceLocation(__FILE__, __LINE__, __func__), "Hello, this is an Debug log.");

    return 0;
}
