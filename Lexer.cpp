#include "Lexer.hpp"
#include <cctype>
#include <iostream>

Lexer::Lexer(const std::string& source) : source(source), currentPosition(0) {}

Token Lexer::nextToken() {
    while (currentPosition < source.length()) {
        char currentChar = source[currentPosition];

        // Ignorar espaços em branco
        if (std::isspace(currentChar)) {
            currentPosition++;
            continue;
        }

        // Ignorar comentários de linha //
        if (currentChar == '/' && currentPosition + 1 < source.length() && source[currentPosition + 1] == '/') {
            currentPosition += 2; // pula os dois '/'
            while (currentPosition < source.length() && source[currentPosition] != '\n') {
                currentPosition++;
            }
            continue; // volta pro while
        }

        // Identificadores ou palavras-chave
        if (std::isalpha(currentChar)) return readIdentifierOrKeyword();

        // Números
        if (std::isdigit(currentChar)) return readNumber();

        // Strings
        if (currentChar == '"') return readString();

        // Operadores e símbolos
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
    currentPosition++; // pula a aspa inicial

    while (currentPosition < source.length() && source[currentPosition] != '"') {
        value += source[currentPosition++];
    }

    if (currentPosition < source.length()) currentPosition++; // pula a aspa final

    return { TokenType::String, value };
}

Token Lexer::readOperatorOrSymbol() {
    char currentChar = source[currentPosition++];
    if (currentChar == '+' || currentChar == '-' || currentChar == '=' || currentChar == ';') {
        return { TokenType::Operator, std::string(1, currentChar) };
    } else if (currentChar == '{' || currentChar == '}') {
        return { TokenType::Symbol, std::string(1, currentChar) };
    } else {
        return { TokenType::Invalid, std::string(1, currentChar) };
    }
}
