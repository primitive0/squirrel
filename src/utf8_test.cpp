#include "thirdparty/catch_amalgamated.hpp"

#include "types/string.hpp"
#include "utf8.hpp"

TEST_CASE("utf8::next_char works") {
    // $ - 1 byte, ñ - 2, 漢 - 3, 𝄞 - 4 in UTF-8
    String string = "$ñ漢𝄞";

    auto cursor = string.cbegin();
    auto end = string.cend();

    auto result = utf8::next_char(cursor, end);
    REQUIRE(result.has_value());
    REQUIRE(*result == U'$');

    result = utf8::next_char(cursor, end);
    REQUIRE(result.has_value());
    REQUIRE(*result == U'ñ');

    result = utf8::next_char(cursor, end);
    REQUIRE(result.has_value());
    REQUIRE(*result == U'漢');

    result = utf8::next_char(cursor, end);
    REQUIRE(result.has_value());
    REQUIRE(*result == U'𝄞');

    result = utf8::next_char(cursor, end);
    REQUIRE(!result.has_value());
    REQUIRE(result.error() == utf8::DecodingError::Eof);
}

// todo: add more tests
