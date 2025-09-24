#include "logger/mmap/mmap_aux.h"
#include <iostream>
#include <string>
#include <cstring>

int main()
{
    // 1. 创建 MMapAux 对象，映射一个文件
    logger::MMapAux mmap_file("mmap_test.bin");
    std::cout << "初始文件大小: " << mmap_file.Size() << " 字节" << std::endl;

    // 2. 向文件写入数据
    const char *msg = "Hello mmap! This is a test string.";
    mmap_file.Push(msg, strlen(msg));
    std::cout << "写入后文件逻辑大小: " << mmap_file.Size() << " 字节" << std::endl;

    // 3. 打印 Data() 中的内容
    std::cout << "内存中数据: " << std::string((char *)mmap_file.Data(), mmap_file.Size()) << std::endl;

    // 4. 模拟重新读取（重新 new 一个对象，映射同一个文件）
    {
        logger::MMapAux mmap_file2("mmap_test.bin");
        std::cout << "重新映射后文件大小: " << mmap_file2.Size() << " 字节" << std::endl;
        std::cout << "重新读取的数据: "
                  << std::string((char *)mmap_file2.Data(), mmap_file2.Size())
                  << std::endl;
    }

    return 0;
}
