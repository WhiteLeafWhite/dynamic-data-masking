#include <sodium.h>
#include <iostream>
#include <string>
#include <cstring>

// 定义 Argon2id 推荐参数
#define OPS_LIMIT crypto_pwhash_OPSLIMIT_MODERATE
#define MEM_LIMIT crypto_pwhash_MEMLIMIT_MODERATE
#define HASH_LEN crypto_pwhash_BYTES // 推荐使用 32 字节哈希长度

std::string SALT = "viva odden";

// 生成密码哈希
std::string hashPassword(const std::string &password) {
    char hashedPassword[crypto_pwhash_STRBYTES];

    std::string passAndSalt = password + SALT;

    if (crypto_pwhash_str(hashedPassword, passAndSalt.c_str(), passAndSalt.size(),
                          OPS_LIMIT, MEM_LIMIT) != 0) {
        throw std::runtime_error("密码哈希失败!");
    }

    return std::string(hashedPassword);
}

// 验证密码
bool verifyPassword(const std::string &hashedPassword, const std::string &password) {
    return crypto_pwhash_str_verify(hashedPassword.c_str(), password.c_str(), password.size()) == 0;
}

int main() {
    // 初始化 libsodium
    if (sodium_init() < 0) {
        std::cerr << "libsodium 初始化失败!" << std::endl;
        return 1;
    }

    // 用户注册：输入密码并哈希存储
    std::string password;
    std::cout << "请输入密码: ";
    std::cin >> password;

    password = "15197283215";

    std::string hashedPassword = hashPassword(password);
    std::cout << "存储的哈希密码: " << hashedPassword << std::endl;

    // 用户登录：输入密码并验证
    std::string inputPassword;
    std::cout << "请再次输入密码进行验证: ";
    std::cin >> inputPassword;

    if (verifyPassword(hashedPassword, inputPassword)) {
        std::cout << "密码正确，登录成功!" << std::endl;
    } else {
        std::cout << "密码错误，登录失败!" << std::endl;
    }

    return 0;
}