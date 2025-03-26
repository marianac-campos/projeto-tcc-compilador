#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

enum class TokenType {
    Keyword, Identifier, Number, Operator, Symbol, EndOfFile, Invalid
};

struct Token {
    TokenType type;
    std::string value;
};

#endif // TOKEN_HPP
