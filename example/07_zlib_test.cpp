// #include <iostream>
// #include <string>
// #include <vector>
// #include <cassert>
// #include "zlib.h"

// // 压缩字符串 data，返回压缩后的二进制数据
// std::vector<unsigned char> compress_string(const std::string &data, int level = Z_BEST_COMPRESSION)
// {
//     uLong src_len = data.size();
//     const unsigned char *src = reinterpret_cast<const unsigned char *>(data.data());

//     // 估算压缩后的最大空间（参考 zlib 文档，使用 compressBound）
//     uLong bound = compressBound(src_len);
//     std::vector<unsigned char> out(bound);

//     int ret = compress2(out.data(), &bound, src, src_len, level);
//     if (ret != Z_OK)
//     {
//         std::cerr << "compress2 failed, error = " << ret << std::endl;
//         return {};
//     }

//     out.resize(bound);
//     return out;
// }

// // 解压缩二进制数据 comp，期望恢复成原始字符串
// std::string decompress_string(const std::vector<unsigned char> &comp, size_t original_size_hint = 0)
// {
//     uLong comp_len = comp.size();
//     const unsigned char *cdata = comp.data();

//     // 如果给了 original_size_hint，可以用这个作为初步大小，否则估一个初始大小
//     uLong dest_len = original_size_hint > 0 ? original_size_hint : comp_len * 3;
//     std::vector<unsigned char> out;
//     while (true)
//     {
//         out.resize(dest_len);
//         int ret = uncompress(out.data(), &dest_len, cdata, comp_len);
//         if (ret == Z_OK)
//         {
//             // 成功
//             out.resize(dest_len);
//             return std::string(reinterpret_cast<char *>(out.data()), out.size());
//         }
//         else if (ret == Z_BUF_ERROR)
//         {
//             // 缓冲区不够，扩大再试
//             dest_len *= 2;
//             continue;
//         }
//         else
//         {
//             std::cerr << "uncompress failed, error = " << ret << std::endl;
//             return {};
//         }
//     }
// }

// int main()
// {
//     std::string s = "Hello, zlib! This is a test string. Hello, zlib! This is a test string.";
//     std::cout << "原始字符串: " << s << std::endl;

//     std::cout << "压缩前长度: " << s.length() << std::endl;

//     auto comp = compress_string(s);
//     std::cout << "压缩后长度: " << comp.size() << std::endl;

//     auto decomp = decompress_string(comp, s.size());
//     std::cout << "解压后: " << decomp << std::endl;

//     assert(decomp == s);
//     std::cout << "断言成功，解压结果和原始一致。" << std::endl;
//     return 0;
// }

#include <iostream>
#include <string>
#include <vector>
#include <zlib.h>

/// 压缩日志字符串
std::string compress_log(const std::string &log)
{
    z_stream stream{};
    if (deflateInit(&stream, Z_BEST_COMPRESSION) != Z_OK)
    {
        throw std::runtime_error("deflateInit failed");
    }

    std::string compressed;
    compressed.resize(compressBound(log.size())); // 预分配最大可能压缩后大小

    stream.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(log.data()));
    stream.avail_in = log.size();

    stream.next_out = reinterpret_cast<Bytef *>(&compressed[0]);
    stream.avail_out = compressed.size();

    int ret = deflate(&stream, Z_FINISH);
    if (ret != Z_STREAM_END)
    {
        deflateEnd(&stream);
        throw std::runtime_error("deflate failed");
    }

    compressed.resize(stream.total_out); // 截断到实际压缩长度
    deflateEnd(&stream);
    return compressed;
}

/// 解压日志字符串
std::string decompress_log(const std::string &compressed)
{
    z_stream stream{};
    if (inflateInit(&stream) != Z_OK)
    {
        throw std::runtime_error("inflateInit failed");
    }

    std::string decompressed;
    decompressed.resize(compressed.size() * 3); // 先预估一个大小，可以动态扩展

    stream.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(compressed.data()));
    stream.avail_in = compressed.size();

    stream.next_out = reinterpret_cast<Bytef *>(&decompressed[0]);
    stream.avail_out = decompressed.size();

    int ret = inflate(&stream, Z_FINISH);
    if (ret != Z_STREAM_END)
    {
        inflateEnd(&stream);
        throw std::runtime_error("inflate failed (maybe buffer too small)");
    }

    decompressed.resize(stream.total_out);
    inflateEnd(&stream);
    return decompressed;
}

int main()
{
    std::string log = "[INFO] 2025-09-28 10:00:00: 用户登录成功，UserID=123456";

    std::cout << "原始日志: " << log << "\n";

    // 压缩日志
    std::string compressed = compress_log(log);
    std::cout << "压缩后大小: " << compressed.size() << " bytes\n";

    // 解压日志
    std::string decompressed = decompress_log(compressed);
    std::cout << "解压还原: " << decompressed << "\n";

    if (log == decompressed)
    {
        std::cout << "✅ 压缩/解压验证成功\n";
    }
    else
    {
        std::cout << "❌ 解压结果不一致\n";
    }

    return 0;
}
