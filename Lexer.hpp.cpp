#include "Lexer.hpp"
#include <cctype>
#include <iostream>

Lexer::Lexer(const std::string& source) : source(source), currentPosition(0) {}

Token Lexer::nextToken() {
    while (currentPosition < source.length() && std::isspace(source[currentPosition])) {
        currentPosition++;
    }

    if (currentPosition >= source.length()) return { TokenType::EndOfFile, "" };

    char currentChar = source[currentPosition];

    if (std::isalpha(currentChar)) {
        return readIdentifierOrKeyword();
    }
    else if (std::isdigit(currentChar)) {
        return readNumber();
    }
    else if (currentChar == '+' || currentChar == '-' || currentChar == '*' ||
        currentChar == '/' || currentChar == '=' || currentChar == ';' ||
        currentChar == '<' || currentChar == '>') {
        return readOperator();
    }
    else if (currentChar == '(' || currentChar == ')' || currentChar == '{' ||
        currentChar == '}') {
        return readSymbol();
    }
    else {
        std::cerr << "Invalid character: " << currentChar << std::endl;
        currentPosition++;
        return { TokenType::Invalid, std::string(1, currentChar) };
    }
}

Token Lexer::readIdentifierOrKeyword() {
    std::string value;
    while (currentPosition < source.length() && std::isalnum(source[currentPosition])) {
        value += source[currentPosition++];
    }

    // Lista de palavras-chave simples
    if (value == "int" || value == "return") {
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

Token Lexer::readOperator() {
    std::string value;
    value += source[currentPosition++];

    // Checar operadores compostos (==, !=, >=, <=)
    if ((value == "=" || value == "!" || value == "<" || value == ">") &&
        currentPosition < source.length() && source[currentPosition] == '=') {
        value += source[currentPosition++];
    }

    return { TokenType::Operator, value };
}

Token Lexer::readSymbol() {
    return { TokenType::Symbol, std::string(1, source[currentPosition++]) };
}
