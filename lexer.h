#pragma once
#pragma once
#include <string>
#include <vector>

enum class TokenType {
    IDENTIFIER,
    KEYWORD,
    NUMBER,
    STRING,
    OPERATOR,
    DELIMITER,
    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string value;
};

class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> tokenize();

private:
    std::string source;
    size_t position;

    char peek() const;
    char get();
    void skipWhitespace();
    Token nextToken();
};
