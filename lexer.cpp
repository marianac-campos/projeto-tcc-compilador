#include "lexer.h"
#include <cctype>

Lexer::Lexer(const std::string& src) : source(src), position(0) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (position < source.length()) {
        skipWhitespace();
        if (position >= source.length()) break;
        tokens.push_back(nextToken());
    }
    tokens.push_back({ TokenType::END_OF_FILE, "" });
    return tokens;
}

char Lexer::peek() const {
    return source[position];
}

char Lexer::get() {
    return source[position++];
}

void Lexer::skipWhitespace() {
    while (position < source.length() && isspace(peek())) {
        position++;
    }
}

Token Lexer::nextToken() {
    char current = get();
    if (isalpha(current)) {
        std::string identifier(1, current);
        while (position < source.length() && isalnum(peek())) {
            identifier += get();
        }
        return { TokenType::IDENTIFIER, identifier };
    }
    if (isdigit(current)) {
        std::string number(1, current);
        while (position < source.length() && isdigit(peek())) {
            number += get();
        }
        return { TokenType::NUMBER, number };
    }
    return { TokenType::DELIMITER, std::string(1, current) };
}
