#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>

#include "types/string.hpp"
#include "utf8.hpp"

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
    size_t pos = 0;

public:
    explicit Lexer(StringView _expression) : expression{_expression} {}

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
    std::string input{};
    std::getline(std::cin, input);

    auto cursor = input.cbegin();
    while (true) {
        char32_t ch{};
        auto result = utf8::next_char(cursor, input.cend());
        if (!result.has_value()) {
            if (result.error() == utf8::DecodingError::InvalidCharacter) {
                std::cout << "decoding error\n";
            }
            break;
        }
        ch = *result;

        std::cout << static_cast<int32_t>(ch) << "\n";
    }

    return 0;
}
