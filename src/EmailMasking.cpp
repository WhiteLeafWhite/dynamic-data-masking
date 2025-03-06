// copyright 2025 Yanghaoyang
#include "EmailMasking.h"

#include <string>

EmailMasking::EmailMasking(char d):delimiter(d){};

std::string EmailMasking::mask(int startPosition, int length,
    const std::string &information){
    std::string res = information;
    // 找到 @ 符号的位置
    size_t atPos = res.find(delimiter);
    // 如果找到了 @ 符号
    if (atPos != std::string::npos) {
    // 将 @ 符号前的所有字符替换为 *
    std::string maskedPart(atPos, '*');
    res.replace(0, atPos, maskedPart);
    }
    return res;
}