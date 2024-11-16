#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <expected>
#include <iostream>
#include <print>

#include "types/optional.hpp"
#include "types/string.hpp"
#include "utf8.hpp"

enum class TokenKind : uint32_t {
    Ident,

    ParenOpen,
    ParenClose,
    Star,

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
    LiteralInteger,

    OpEqual,
};

struct Token {
    TokenKind kind{};
    StringView span{};

    explicit Token() : kind{TokenKind::Ident} {}

    explicit Token(TokenKind kind_) : kind{kind_} {}

    explicit Token(TokenKind kind_, StringView span_) : kind{kind_}, span{span_} {}
};

// template<>
// struct std::formatter<Token> {
//     constexpr auto parse(std::format_parse_context& ctx) {
//         return ctx.begin();
//     }

//     auto format(const Token& id, std::format_context& ctx) const {
//         return std::format_to(ctx.out(), "{}", id.span);
//     }
// };

auto operator<<(std::ostream& os, const Token& token) -> std::ostream& {
    switch (token.kind) {
    case TokenKind::Ident:
        os << "Ident";
        break;
    case TokenKind::ParenOpen:
        os << "ParenOpen";
        break;
    case TokenKind::ParenClose:
        os << "ParenClose";
        break;
    case TokenKind::Star:
        os << "Star";
        break;
    case TokenKind::KeywordSelect:
        os << "KeywordSelect";
        break;
    case TokenKind::KeywordFrom:
        os << "KeywordFrom";
        break;
    case TokenKind::KeywordWhere:
        os << "KeywordWhere";
        break;
    case TokenKind::KeywordAnd:
        os << "KeywordAnd";
        break;
    case TokenKind::KeywordOr:
        os << "KeywordOr";
        break;
    case TokenKind::KeywordOrder:
        os << "KeywordOrder";
        break;
    case TokenKind::KeywordBy:
        os << "KeywordBy";
        break;
    case TokenKind::KeywordInsert:
        os << "KeywordInsert";
        break;
    case TokenKind::KeywordTrue:
        os << "KeywordTrue";
        break;
    case TokenKind::KeywordFalse:
        os << "KeywordFalse";
        break;
    case TokenKind::LiteralString:
        os << "LiteralString";
        break;
    case TokenKind::LiteralInteger:
        os << "LiteralInteger";
        break;
    case TokenKind::OpEqual:
        os << "OpEqual";
        break;
    default:
        std::abort();
    }
    os << "(\"" << token.span << "\")";
    return os;
}

// todo: better name
auto check_keyword(StringView span) -> Optional<TokenKind> {
    auto to_lower = [](char& ch) -> void {
        // todo: check implicit cast for soundness
        ch = std::tolower(ch);
    };

    String lowercase{span};
    std::for_each(lowercase.begin(), lowercase.end(), to_lower);

    if (lowercase == "select") {
        return TokenKind::KeywordSelect;
    }
    if (lowercase == "from") {
        return TokenKind::KeywordFrom;
    }
    if (lowercase == "where") {
        return TokenKind::KeywordWhere;
    }
    if (lowercase == "and") {
        return TokenKind::KeywordAnd;
    }
    if (lowercase == "or") {
        return TokenKind::KeywordOr;
    }
    if (lowercase == "order") {
        return TokenKind::KeywordOrder;
    }
    if (lowercase == "by") {
        return TokenKind::KeywordBy;
    }
    if (lowercase == "insert") {
        return TokenKind::KeywordInsert;
    }
    if (lowercase == "true") {
        return TokenKind::KeywordTrue;
    }
    if (lowercase == "false") {
        return TokenKind::KeywordFalse;
    }
    return None;
}

struct LexerException {};

struct Lexer {
private:
    const char* begin;
    const char* current;
    const char* end;
    char32_t ch;

public:
    explicit Lexer(StringView expression) {
        this->begin = expression.begin();
        this->current = begin;
        this->end = expression.end();
    }

    auto next() -> Optional<Token> {
        do {
            auto next = this->next_char();
            if (!next) {
                return None;
            }
            this->current = *next;

            if (this->ch != U' ') {
                break;
            }
            this->begin = current;
        } while (true);

        Token token{};

        if (this->match_ident_or_keyword(token)) {
            return token;
        }
        if (this->match_star(token)) {
            return token;
        }
        if (this->match_paren_open(token)) {
            return token;
        }
        if (this->match_paren_close(token)) {
            return token;
        }
        if (this->match_string_literal(token)) {
            return token;
        }
        if (this->match_op_equal(token)) {
            return token;
        }

        throw LexerException{};
    }

private:
    auto match_ident_or_keyword(Token& token) -> bool {
        if (!utf8::ascii::is_letter(this->ch)) {
            return false;
        }

        while (true) {
            auto next = this->next_char();
            if (!next) {
                break;
            }
            if (!utf8::ascii::is_letter(this->ch) && !utf8::ascii::is_digit(this->ch)) {
                break;
            }
            this->current = *next;
        }

        token.span = StringView(this->begin, this->current - this->begin);
        auto kind = check_keyword(token.span);
        token.kind = kind.has_value() ? *kind : TokenKind::Ident;

        this->begin = current; // todo: move to function

        return true;
    }

    auto match_star(Token& token) -> bool {
        return this->match_single_char_token(TokenKind::Star, U'*', token);
    }

    auto match_paren_open(Token& token) -> bool {
        return this->match_single_char_token(TokenKind::ParenOpen, U'(', token);
    }

    auto match_paren_close(Token& token) -> bool {
        return this->match_single_char_token(TokenKind::ParenClose, U')', token);
    }

    auto match_op_equal(Token& token) -> bool {
        return this->match_single_char_token(TokenKind::OpEqual, U'=', token);
    }

    auto match_single_char_token(TokenKind goal, char32_t needle, Token& token) -> bool {
        if (this->ch != needle) {
            return false;
        }

        token.span = StringView(this->begin, this->current - this->begin);
        token.kind = goal;

        this->begin = current;

        return true;
    }

    auto match_string_literal(Token& token) -> bool {
        if (this->ch != '\'') {
            return false;
        }

        do {
            auto next = this->next_char();
            if (!next) {
                throw LexerException{};
            }
            this->current = *next;
        } while (this->ch != U'\'');

        token.span = StringView(this->begin, this->current - this->begin);
        token.kind = TokenKind::LiteralString;

        this->begin = current;

        return true;
    }

    auto next_char() -> Optional<const char*> {
        const char* next = this->current;
        auto result = utf8::next_char(next, this->end);
        if (result) {
            this->ch = *result;
            return next;
        }
        if (result.error() == utf8::DecodingError::Eof) {
            return None;
        }
        throw LexerException{};
    }
};

auto main() -> int {
    String input{};

    while (true) {
        std::print(">>> ");
        std::getline(std::cin, input);

        Lexer lexer{input};

        Optional<Token> token = lexer.next();
        while (token) {
            std::cout << *token << "\n";
            token = lexer.next();
        }
    }

    return 0;
}
