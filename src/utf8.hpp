#include <cstddef>
#include <cstdint>

namespace utf8 {

inline constinit ptrdiff_t ERR_EOF = 0;
inline constinit ptrdiff_t ERR_INVALID_UTF8 = -1;

inline ptrdiff_t next_rune(const char* begin, const char* end, char32_t& rune) {
    if ((*begin & 0b10000000) == 0b00000000) { // 0xxxxxxx
        if (begin >= end) {
            return ERR_EOF;
        }

        rune = *begin;

        return 1;
    } else if ((*begin & 0b11100000) == 0b11000000) { // 110xxxxx
        if (begin + 1 >= end) {
            return ERR_EOF;
        }
        int32_t first = begin[0];
        int32_t second = begin[1];

        if ((second & 0b11000000) != 0b10000000) {
            return ERR_INVALID_UTF8;
        }

        rune = second & 0b00111111;
        rune |= (first & 0b00011111) << 6;

        return 2;
    } else if ((*begin & 0b11110000) == 0b11100000) { // 1110xxxx
        if (begin + 2 >= end) {
            return ERR_EOF;
        }
        int32_t first = begin[0];
        int32_t second = begin[1];
        int32_t third = begin[2];

        if ((second & 0b11000000) != 0b10000000) {
            return ERR_INVALID_UTF8;
        }
        if ((third & 0b11000000) != 0b10000000) {
            return ERR_INVALID_UTF8;
        }

        rune = third & 0b00111111;
        rune |= (second & 0b00111111) << 6;
        rune |= (first & 0b00001111) << 12;

        return 3;
    } else if ((*begin & 0b11111000) == 0b11110000) { // 11110xxx
        if (begin + 3 >= end) {
            return ERR_EOF;
        }
        int32_t first = begin[0];
        int32_t second = begin[1];
        int32_t third = begin[2];
        int32_t forth = begin[3];

        if ((second & 0b11000000) != 0b10000000) {
            return ERR_INVALID_UTF8;
        }
        if ((third & 0b11000000) != 0b10000000) {
            return ERR_INVALID_UTF8;
        }
        if ((forth & 0b11000000) != 0b10000000) {
            return ERR_INVALID_UTF8;
        }

        rune = forth & 0b00111111;
        rune |= (third & 0b00111111) << 6;
        rune |= (second & 0b00111111) << 12;
        rune |= (first & 0b00000111) << 18;

        return 4;
    } else {
        return ERR_INVALID_UTF8;
    }
}

namespace ascii {

inline bool is_lowercase(char ch) {
    return ch >= 'a' && ch <= 'z';
}

inline bool is_uppercase(char ch) {
    return ch >= 'A' && ch <= 'Z';
}

} // namespace ascii

} // namespace utf8
