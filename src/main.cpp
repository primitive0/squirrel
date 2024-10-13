#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <string_view>

#include "utf8.hpp"

using String = std::string;
using StringView = std::string_view;

enum class TokenKind : uint32_t {
    Ident,

    ParenOpen,
    ParenClose,

    KeywordSelect,
    KeywordFrom,
    KeywordWhere,
    KeywordAnd,
    KeywordOr,
    KeywordOrder,
    KeywordBy,
    KeywordInsert,
    KeywordTrue,
    KeywordFalse,

    LiteralString,
    LiteralInteger
};

struct Token {
    TokenKind kind{};
    StringView span{};

    explicit Token() : kind{TokenKind::Ident} {}

    explicit Token(TokenKind _kind) : kind{_kind} {}

    explicit Token(TokenKind _kind, StringView _span) : kind{_kind}, span{_span} {}
};

struct Lexer {
private:
    StringView expression{};
    size_t pos{};

public:
    explicit Lexer(StringView _expression) : expression{_expression}, pos{0} {}

    bool next(Token& token) {
        if (this->pos == this->expression.size()) {
            return false;
        }

        while (this->expression[this->pos] == ' ') {
            this->pos++;
            if (this->pos == this->expression.size()) {
                return false;
            }
        }

        // this->expression.begin()

        // => this->pos < this->expr.size()

        return false;
    }

private:
    // bool match_ident_or_keyword(Token& token) {
    //     if (!is_ascii_letter(this->expr[this->pos])) {
    //         return false;
    //     }

    //     size_t i = this->pos + 1;
    //     while (i < this->expr.size()) {
    //         if (!is_ascii_letter(this->expr[i])) {
    //             break;
    //         }
    //         i++;
    //     }

    //     StringRef value = this->expr.substr(this->pos, i - this->pos);
    //     token = Ident{value};

    //     this->pos = i;
    //     this->is_previous_token_operand = false;

    //     return true;
    // }
};

int main() {
    char32_t rune{};
    const char* str = "а中🌞";

    const char* begin = str;
    const char* end = begin + std::strlen(str);
    ptrdiff_t read = utf8::next_rune(begin, end, rune);
    while (read != 0) {
        if (read != utf8::ERR_EOF) {
            std::cout << static_cast<int32_t>(rune) << " ";
        } else {
            std::cout << "xxx\n";
            return 0;
        }
        begin += read;
        read = utf8::next_rune(begin, end, rune);
    }
    std::cout << "\n";
    return 0;
}
