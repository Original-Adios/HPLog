// #include <iostream>
// #include <string>
// #include "logger/crypt/aes_crypt.h"
// #include "logger/crypt/crypt.h"

// using namespace logger::crypt;

// int main()
// {
//     // 1. 生成 AES 密钥
//     std::string aes_key = AESCrypt::GenerateKey();
//     std::cout << "[INFO] 生成 AES Key(hex): " << aes_key << "\n";

//     // 2. 创建 AES 加密对象
//     AESCrypt aes(aes_key);

//     // 3. 模拟日志内容
//     std::string log = "[INFO] 用户登录成功，用户名=alice, IP=192.168.1.100";
//     std::cout << "[INFO] 原日志: " << log << "\n";

//     // 4. 加密日志
//     std::string encrypted;
//     aes.Encrypt(log.data(), log.size(), encrypted);
//     std::cout << "[INFO] 加密后的日志长度: " << encrypted.size() << "\n";

//     // 5. 解密日志
//     std::string decrypted = aes.Decrypt(encrypted.data(), encrypted.size());
//     std::cout << "[INFO] 解密后日志: " << decrypted << "\n";

//     // 6. 验证
//     if (decrypted == log)
//     {
//         std::cout << "[INFO] 加解密成功 ✅\n";
//     }
//     else
//     {
//         std::cout << "[ERROR] 加解密失败 ❌\n";
//     }

//     return 0;
// }

#include <iostream>
#include <string>
#include "crypt/aes_crypt.h"
#include "crypt/crypt.h"

int main()
{
    using namespace logger::crypt;

    // ==============================
    // 1️⃣ 生成客户端和服务端的 ECDH 密钥对
    auto [client_pri, client_pub] = GenECDHKey();
    auto [server_pri, server_pub] = GenECDHKey();

    std::cout << "Client Public Key (hex): " << BinaryKeyToHex(client_pub) << std::endl;
    std::cout << "Server Public Key (hex): " << BinaryKeyToHex(server_pub) << std::endl;

    // ==============================
    // 2️⃣ 双方协商共享密钥
    std::string client_shared_key = GenECDHSharedSecret(client_pri, server_pub);
    std::string server_shared_key = GenECDHSharedSecret(server_pri, client_pub);

    std::cout << "Client Shared Key (hex): " << BinaryKeyToHex(client_shared_key) << std::endl;
    std::cout << "Server Shared Key (hex): " << BinaryKeyToHex(server_shared_key) << std::endl;

    // 验证共享密钥是否一致
    if (client_shared_key != server_shared_key)
    {
        std::cerr << "Error: Shared keys do not match!" << std::endl;
        return -1;
    }

    // ==============================
    // 3️⃣ 使用 AES 对日志内容加密
    AESCrypt aes(client_shared_key); // 使用协商的共享密钥
    std::string plaintext = "This is a sensitive log message.";
    std::string ciphertext;

    aes.Encrypt(plaintext.data(), plaintext.size(), ciphertext);

    std::cout << "Encrypted Log (hex): " << BinaryKeyToHex(ciphertext) << std::endl;

    // ==============================
    // 4️⃣ 使用 AES 解密
    std::string decrypted = aes.Decrypt(ciphertext.data(), ciphertext.size());
    std::cout << "Decrypted Log: " << decrypted << std::endl;

    return 0;
}
