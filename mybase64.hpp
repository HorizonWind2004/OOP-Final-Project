#ifndef _BASE64_H_
#define _BASE64_H_
#include <string>
#include <array>
#include <cctype>

static const std::string base64Chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::array<unsigned char, 4> transform(const std::array<unsigned char, 3>& char3) {
    return {
        static_cast<unsigned char>((char3[0] & 0xfc) >> 2),
        static_cast<unsigned char>(((char3[0] & 0x03) << 4) + ((char3[1] & 0xf0) >> 4)),
        static_cast<unsigned char>(((char3[1] & 0x0f) << 2) + ((char3[2] & 0xc0) >> 6)),
        static_cast<unsigned char>(char3[2] & 0x3f)
    };
}

std::string base64_encode(unsigned char const* bytesToEncode, unsigned int len) {
    std::string ret;
    std::array<unsigned char, 3> char3{};
    std::array<unsigned char, 4> char4{};
    int i = 0;
    while (len--) {
        char3[i++] = *(bytesToEncode++);
        if (i == 3) {
            char4 = transform(char3);
            for (auto ch : char4) {
                ret += base64Chars[ch];
            }
            i = 0;
        }
    }
    if (i) {
        for (int j = i; j < 3; j++) {
            char3[j] = '\0';
        }
        char4 = transform(char3);
        for (int j = 0; j < i + 1; j++) {
            ret += base64Chars[char4[j]];
        }
        while (i++ < 3) {
            ret += '=';
        }
    }
    return ret;
}

std::string base64_decode(std::string const& encodedString) {
    int len = encodedString.size();
    int i = 0;
    int j = 0;
    int in = 0;
    std::array<unsigned char, 4> char4{};
    std::array<unsigned char, 3> char3{};
    std::string ret;
    while (len-- && (encodedString[in] != '=') && is_base64(encodedString[in])) {
        char4[i++] = encodedString[in]; in++;
        if (i ==4) {
            for (int k = 0; k < 4; k++) {
                char4[k] = base64Chars.find(char4[k]);
            }
            char3[0] = (char4[0] << 2) + ((char4[1] & 0x30) >> 4);
            char3[1] = ((char4[1] & 0xf) << 4) + ((char4[2] & 0x3c) >> 2);
            char3[2] = ((char4[2] & 0x3) << 6) + char4[3];
            for (auto ch : char3) {
                ret += ch;
            }
            i = 0;
        }
    }
    if (i) {
        for (int k = i; k < 4; k++) {
            char4[k] = 0;
        }
        for (int k = 0; k < 4; k++) {
            char4[k] = base64Chars.find(char4[k]);
        }
        char3[0] = (char4[0] << 2) + ((char4[1] & 0x30) >> 4);
        char3[1] = ((char4[1] & 0xf) << 4) + ((char4[2] & 0x3c) >> 2);
        for (int k = 0; k < i - 1; k++) {
            ret += char3[k];
        }
    }
    return ret;
}
#endif
