#include <array>
#include <cstdint>
#include <expected>
#include <iterator>

template<typename R, typename E>
using Expected = std::expected<R, E>;

namespace utf8 {

template<typename It>
concept OctetIterator = std::forward_iterator<It> && std::same_as<std::iter_value_t<It>, char>;

enum struct DecodingError {
    Eof,
    InvalidCharacter,
};

// todo: better name
template<OctetIterator It>
inline Expected<void, DecodingError> read_utf8_octet(It& it, It end, uint8_t& octet) {
    if (it == end) {
        return std::unexpected(DecodingError::Eof);
    }

    octet = *it;
    it = std::next(it);

    if ((octet & 0xC0) != 0x80) {
        return std::unexpected(DecodingError::InvalidCharacter);
    }

    return {};
}

template<OctetIterator It>
inline Expected<char32_t, DecodingError> next_char(It& it, It end) {
    if (it == end) {
        return std::unexpected(DecodingError::Eof);
    }

    It current = it;
    std::array<uint8_t, 4> octets{};

    octets[0] = *current;
    current = std::next(current);

    if ((octets[0] & 0x80) == 0) {
        it = current;
        return static_cast<char32_t>(octets[0]);
    } else if ((octets[0] & 0xE0) == 0xC0) {
        auto result = read_utf8_octet(current, end, octets[1]);
        if (!result) {
            return std::unexpected(result.error());
        }

        char32_t ch{};
        ch = octets[1] & 0x3F;
        ch |= (octets[0] & 0x1F) << 6;

        if (ch < 0x80) {
            return std::unexpected(DecodingError::InvalidCharacter);
        }

        it = current;
        return ch;
    } else if ((octets[0] & 0xF0) == 0xE0) {
        auto result = read_utf8_octet(current, end, octets[1]);
        if (!result) {
            return std::unexpected(result.error());
        }
        result = read_utf8_octet(current, end, octets[2]);
        if (!result) {
            return std::unexpected(result.error());
        }

        char32_t ch{};
        ch = octets[2] & 0x3F;
        ch |= (octets[1] & 0x3F) << 6;
        ch |= (octets[0] & 0x0F) << 12;

        if (ch < 0x800) {
            return std::unexpected(DecodingError::InvalidCharacter);
        }

        it = current;
        return ch;
    } else if ((octets[0] & 0xF8) == 0xF0) {
        auto result = read_utf8_octet(current, end, octets[1]);
        if (!result) {
            return std::unexpected(result.error());
        }
        result = read_utf8_octet(current, end, octets[2]);
        if (!result) {
            return std::unexpected(result.error());
        }
        result = read_utf8_octet(current, end, octets[3]);
        if (!result) {
            return std::unexpected(result.error());
        }

        char32_t ch{};
        ch = octets[3] & 0x3F;
        ch |= (octets[2] & 0x3F) << 6;
        ch |= (octets[1] & 0x3F) << 12;
        ch |= (octets[0] & 0x07) << 18;

        if (ch < 0x10000) {
            return std::unexpected(DecodingError::InvalidCharacter);
        }

        // invalid UTF-8 characters
        if (ch > 0x10FFFF) {
            return std::unexpected(DecodingError::InvalidCharacter);
        }

        it = current;
        return ch;
    } else {
        return std::unexpected(DecodingError::InvalidCharacter);
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
