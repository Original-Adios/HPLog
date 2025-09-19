#pragma once

#include <memory>

#include "log_common.h"
#include "log_msg.h"

namespace logger
{
    class LogSink
    {
    public:
        virtual ~LogSink() = default;

        virtual void Log(const LogMsg &msg) = 0;
    };
} // namespace logger
