#include "thirdparty/catch_amalgamated.hpp"

#include "types/string.hpp"
#include "utf8.hpp"

TEST_CASE("hello, world") {
    String string{"foo"};

    auto cursor = string.cbegin();
    auto end = string.cend();

    auto result = utf8::next_char(cursor, end);
    REQUIRE(result.has_value());
    REQUIRE(*result == U'f');

    result = utf8::next_char(cursor, end);
    REQUIRE(result.has_value());
    REQUIRE(*result == U'o');

    result = utf8::next_char(cursor, end);
    REQUIRE(result.has_value());
    REQUIRE(*result == U'o');
}
