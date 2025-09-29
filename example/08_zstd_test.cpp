#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "compress/zstd_compress.h"

using namespace logger::compress;

int main()
{
    // 1. 模拟生成日志内容
    std::string log_data;
    for (int i = 0; i < 1000; ++i)
    {
        log_data += "2025-09-28 [INFO] This is a log line number " + std::to_string(i) + "\n";
    }

    ZstdCompress compressor;

    // 2. 计算压缩后可能的最大长度
    size_t max_compressed_size = compressor.CompressedBound(log_data.size());
    std::vector<char> compressed(max_compressed_size);

    // 3. 压缩日志内容
    size_t compressed_size = compressor.Compress(
        log_data.data(),
        log_data.size(),
        compressed.data(),
        compressed.size());

    if (compressed_size == 0)
    {
        std::cerr << "Compression failed!" << std::endl;
        return 1;
    }

    // 4. 将压缩数据写入文件
    std::ofstream ofs("logs.zst", std::ios::binary);
    ofs.write(compressed.data(), compressed_size);
    ofs.close();
    std::cout << "Compressed log written to logs.zst (" << compressed_size << " bytes)" << std::endl;

    // 5. 模拟读取日志文件
    std::ifstream ifs("logs.zst", std::ios::binary);
    std::vector<char> file_data((std::istreambuf_iterator<char>(ifs)),
                                std::istreambuf_iterator<char>());
    ifs.close();

    // 6. 解压日志内容
    std::string decompressed = compressor.Decompress(file_data.data(), file_data.size());
    if (decompressed.empty())
    {
        std::cerr << "Decompression failed!" << std::endl;
        return 1;
    }

    // 7. 验证压缩前后是否一致
    if (decompressed == log_data)
    {
        std::cout << "Decompression OK! Original log restored." << std::endl;
    }
    else
    {
        std::cerr << "Decompression mismatch!" << std::endl;
    }

    // 8. 还可以把解压结果直接打印出来
    std::cout << "First 200 chars:\n"
              << decompressed.substr(0, 200) << "..." << std::endl;

    return 0;
}
