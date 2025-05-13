#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

enum class TokenType {
    Keyword, Identifier, Number, String, Operator, Symbol, EndOfFile, Invalid
};

struct Token {
    TokenType type;
    std::string value;
};

#endif
