#include <iostream>
#include <chrono>
#include <string>
#include <thread>
#include "logger/context/thread_pool.h" // 你的 ThreadPool 头文件

using namespace logger::ctx;

std::mutex cout_mutex;

void simple_task(int id)
{
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "[simple_task] id=" << id
              << " running in thread " << std::this_thread::get_id()
              << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

int add_task(int a, int b)
{
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "[add_task] a=" << a << " b=" << b
              << " running in thread " << std::this_thread::get_id()
              << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    return a + b;
}

int main()
{
    std::cout << "=== 创建线程池，线程数=3 ===\n";
    ThreadPool pool(3);
    pool.Start();

    std::cout << "\n=== 提交普通任务 ===\n";
    for (int i = 0; i < 5; ++i)
    {
        pool.RunTask(simple_task, i);
    }

    std::cout << "\n=== 提交带返回值的任务 ===\n";
    auto fut1 = pool.RunRetTask(add_task, 10, 20);
    auto fut2 = pool.RunRetTask(add_task, 30, 40);

    if (fut1.valid() && fut2.valid())
    {
        std::cout << "[main] fut1 result=" << fut1.get() << std::endl;
        std::cout << "[main] fut2 result=" << fut2.get() << std::endl;
    }

    std::cout << "\n=== 停止线程池 ===\n";
    pool.Stop();

    std::cout << "\n=== 尝试再次启动线程池，线程数=2 ===\n";
    // 重新创建一个线程池，指定不同的线程数量
    ThreadPool pool2(2);
    pool2.Start();

    std::cout << "\n=== 提交新的任务到新线程池 ===\n";
    for (int i = 100; i < 105; ++i)
    {
        pool2.RunTask(simple_task, i);
    }

    auto fut3 = pool2.RunRetTask(add_task, 5, 7);
    if (fut3.valid())
    {
        std::cout << "[main] fut3 result=" << fut3.get() << std::endl;
    }

    std::cout << "\n=== 停止第二个线程池 ===\n";
    pool2.Stop();

    std::cout << "\n=== 程序结束 ===\n";
    return 0;
}
