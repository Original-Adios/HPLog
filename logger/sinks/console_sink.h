#include <iostream>
#include <string>

#include "sinks/sink.h"

namespace logger
{
    class ConsoleSink : public LogSink
    {
    public:
        ConsoleSink() = default;

        ~ConsoleSink() override = default;

        void Log(const LogMsg &msg) override;
    };
}