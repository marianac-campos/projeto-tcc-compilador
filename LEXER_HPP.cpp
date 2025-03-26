#ifndef LEXER_HPP
#define LEXER_HPP

#include "Token.hpp"
#include <string>
#include <vector>

class Lexer {
public:
    Lexer(const std::string& source);
    Token nextToken();

private:
    std::string source;
    size_t currentPosition;

    Token readIdentifierOrKeyword();
    Token readNumber();
    Token readOperator();
    Token readSymbol();
};

#endif // LEXER_HPP
