#include <cstdint>

#include "types/array.hpp"
#include "types/expected.hpp"
#include "types/iterator.hpp"

namespace utf8 {

template<typename It>
concept OctetIterator = ForwardIterator<It> && std::same_as<std::iter_value_t<It>, char>;

enum struct DecodingError {
    Eof,
    InvalidCharacter,
};

// todo: better name
template<OctetIterator It>
inline auto read_utf8_octet(It& it, It end, uint8_t& octet) -> Expected<void, DecodingError> {
    if (it == end) {
        return Unexpected(DecodingError::Eof);
    }

    octet = *it;
    it = std::next(it);

    if ((octet & 0xC0) != 0x80) {
        return Unexpected(DecodingError::InvalidCharacter);
    }

    return {};
}

template<OctetIterator It, SentinelFor<It> Sentinel>
inline auto next_char(It& it, Sentinel end) -> Expected<char32_t, DecodingError> {
    if (it == end) {
        return Unexpected(DecodingError::Eof);
    }

    It current = it;
    Array<uint8_t, 4> octets{};

    octets[0] = *current;
    current = std::next(current);

    if ((octets[0] & 0x80) == 0) {
        it = current;
        return static_cast<char32_t>(octets[0]);
    } else if ((octets[0] & 0xE0) == 0xC0) {
        auto result = read_utf8_octet(current, end, octets[1]);
        if (!result) {
            return Unexpected(result.error());
        }

        char32_t ch{};
        ch = octets[1] & 0x3F;
        ch |= (octets[0] & 0x1F) << 6;

        if (ch < 0x80) {
            return Unexpected(DecodingError::InvalidCharacter);
        }

        it = current;
        return ch;
    } else if ((octets[0] & 0xF0) == 0xE0) {
        auto result = read_utf8_octet(current, end, octets[1]);
        if (!result) {
            return Unexpected(result.error());
        }
        result = read_utf8_octet(current, end, octets[2]);
        if (!result) {
            return Unexpected(result.error());
        }

        char32_t ch{};
        ch = octets[2] & 0x3F;
        ch |= (octets[1] & 0x3F) << 6;
        ch |= (octets[0] & 0x0F) << 12;

        if (ch < 0x800) {
            return Unexpected(DecodingError::InvalidCharacter);
        }

        it = current;
        return ch;
    } else if ((octets[0] & 0xF8) == 0xF0) {
        auto result = read_utf8_octet(current, end, octets[1]);
        if (!result) {
            return Unexpected(result.error());
        }
        result = read_utf8_octet(current, end, octets[2]);
        if (!result) {
            return Unexpected(result.error());
        }
        result = read_utf8_octet(current, end, octets[3]);
        if (!result) {
            return Unexpected(result.error());
        }

        char32_t ch{};
        ch = octets[3] & 0x3F;
        ch |= (octets[2] & 0x3F) << 6;
        ch |= (octets[1] & 0x3F) << 12;
        ch |= (octets[0] & 0x07) << 18;

        if (ch < 0x10000) {
            return Unexpected(DecodingError::InvalidCharacter);
        }

        // invalid UTF-8 characters
        if (ch > 0x10FFFF) {
            return Unexpected(DecodingError::InvalidCharacter);
        }

        it = current;
        return ch;
    } else {
        return Unexpected(DecodingError::InvalidCharacter);
    }
}

namespace ascii {

inline auto is_lowercase(char32_t ch) -> bool {
    return ch >= U'a' && ch <= U'z';
}

inline auto is_uppercase(char32_t ch) -> bool {
    return ch >= U'A' && ch <= U'Z';
}

inline auto is_letter(char32_t ch) -> bool {
    return is_lowercase(ch) || is_uppercase(ch);
}

} // namespace ascii

} // namespace utf8
