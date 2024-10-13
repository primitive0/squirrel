#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>

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
        return false;
    }
};

int main() {
    return 0;
}
