#include "Lexer.hpp"
#include <cctype>
#include <iostream>

Lexer::Lexer(const std::string& source) : source(source), currentPosition(0) {}

Token Lexer::nextToken() {
    while (currentPosition < source.length()) {
        char currentChar = source[currentPosition];

        if (std::isspace(currentChar)) {
            currentPosition++;
            continue;
        }

        if (currentChar == '/' && currentPosition + 1 < source.length() && source[currentPosition + 1] == '/') {
            currentPosition += 2; 
            while (currentPosition < source.length() && source[currentPosition] != '\n') {
                currentPosition++;
            }
            continue; 
        }

        if (std::isalpha(currentChar)) return readIdentifierOrKeyword();

        if (std::isdigit(currentChar)) return readNumber();

        if (currentChar == '"') return readString();

        return readOperatorOrSymbol();
    }

    return { TokenType::EndOfFile, "" };
}

Token Lexer::readIdentifierOrKeyword() {
    std::string value;
    while (currentPosition < source.length() && std::isalnum(source[currentPosition])) {
        value += source[currentPosition++];
    }

    if (value == "int" || value == "string" || value == "return") {
        return { TokenType::Keyword, value };
    }

    return { TokenType::Identifier, value };
}

Token Lexer::readNumber() {
    std::string value;
    while (currentPosition < source.length() && std::isdigit(source[currentPosition])) {
        value += source[currentPosition++];
    }
    return { TokenType::Number, value };
}

Token Lexer::readString() {
    std::string value;
    currentPosition++; 

    while (currentPosition < source.length() && source[currentPosition] != '"') {
        value += source[currentPosition++];
    }

    if (currentPosition < source.length()) currentPosition++; 

    return { TokenType::String, value };
}

Token Lexer::readOperatorOrSymbol() {
    char currentChar = source[currentPosition++];

    if (currentChar == '+' || currentChar == '-' || currentChar == '=' || currentChar == '*'
        || currentChar == '/' ) {
        return { TokenType::Operator, std::string(1, currentChar) };
    } 
    else if (currentChar == '{' || currentChar == '}' ||
             currentChar == '(' || currentChar == ')' ||
             currentChar == ':' || currentChar == ',' || currentChar == ';') {
        return { TokenType::Symbol, std::string(1, currentChar) };
    } 
    else {
        return { TokenType::Invalid, std::string(1, currentChar) };
    }
}
