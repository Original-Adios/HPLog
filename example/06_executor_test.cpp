// #include <iostream>
// #include <sstream>
// #include <thread>
// #include <vector>
// #include <mutex>
// #include "logger/context/executor.h"

// using namespace logger::ctx;

// class SimpleLogger
// {
// public:
//     SimpleLogger()
//     {
//         executor_ = std::make_unique<Executor>();
//         writer_tag_ = executor_->AddTaskRunner(1); // 单线程 TaskRunner
//         // 启动定时 flush
//         executor_->PostRepeatedTask(writer_tag_, [this]()
//                                     { Flush(); }, std::chrono::seconds(1), UINT64_MAX);
//     }

//     void Log(const std::string &msg)
//     {
//         // 产生日志任务，异步执行
//         executor_->PostTask(writer_tag_, [this, msg]()
//                             {
//             std::lock_guard<std::mutex> lock(buffer_mutex_);
//             buffer_ << msg << "\n"; });
//     }

// private:
//     void Flush()
//     {
//         std::lock_guard<std::mutex> lock(buffer_mutex_);
//         std::string data = buffer_.str();
//         buffer_.str(""); // 清空缓冲区
//         buffer_.clear();
//         if (!data.empty())
//         {
//             std::cout << "[FLUSH]\n"
//                       << data;
//         }
//     }

//     std::unique_ptr<Executor> executor_;
//     TaskRunnerTag writer_tag_;
//     std::ostringstream buffer_;
//     std::mutex buffer_mutex_;
// };

// int main()
// {
//     SimpleLogger logger;
//     std::cout << "!data.empty()" << std::endl;
//     // 启动多个线程模拟产生日志
//     std::vector<std::thread> threads;
//     for (int t = 0; t < 3; ++t)
//     {
//         threads.emplace_back([&logger, t]()
//                              {
//             for (int i = 0; i < 5; ++i) {
//                 logger.Log("Thread " + std::to_string(t) +
//                            " message " + std::to_string(i));
//                 std::this_thread::sleep_for(std::chrono::milliseconds(100));
//             } });
//     }

//     for (auto &th : threads)
//         th.join();

//     // 等待 2 秒，确保 flush 执行
//     std::this_thread::sleep_for(std::chrono::seconds(2));
// }

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include "context/executor.h"

using namespace logger::ctx;

// 一个简单的日志缓冲区
class LogBuffer
{
public:
    void Append(const std::string &msg)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        buffer_ += msg + "\n";
    }

    void FlushToFile(const std::string &filename)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (buffer_.empty())
            return;
        std::ofstream ofs(filename, std::ios::app);
        ofs << buffer_;
        buffer_.clear();
    }

private:
    std::string buffer_;
    std::mutex mutex_;
};

int main()
{
    Executor executor;

    // 创建一个专门用于日志写入的 TaskRunner（单线程，保证日志顺序）
    TaskRunnerTag log_runner = executor.AddTaskRunner(1);

    LogBuffer log_buffer;

    // 周期性任务：每 1 秒刷新一次日志文件
    executor.PostRepeatedTask(
        log_runner,
        [&log_buffer]()
        {
            log_buffer.FlushToFile("app.log");
            std::cout << "[LogSystem] Flush done in thread "
                      << std::this_thread::get_id() << std::endl;
        },
        std::chrono::seconds(1),
        /*repeat_num=*/1000); // 可视作无限次

    // 模拟主线程业务：生成日志
    for (int i = 0; i < 10; ++i)
    {
        // 异步提交日志任务，不阻塞主线程
        executor.PostTask(log_runner, [&, i]()
                          {
            log_buffer.Append("Log message #" + std::to_string(i));
            std::cout << "[LogSystem] Append log in thread "
                      << std::this_thread::get_id() << std::endl; });

        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

    // 等待一会儿，保证定时器线程能执行几次
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "Main thread exiting...\n";
    return 0;
}
