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

    // Check for specific type keywords first
    if (value == "int") return { TokenType::Integer, value };
    if (value == "float") return { TokenType::Float, value }; // Assuming 'float' is a keyword
    if (value == "bool") return { TokenType::Boolean, value }; // Assuming 'bool' is a keyword
    if (value == "char") return { TokenType::Char, value };   // Assuming 'char' is a keyword
    // Add other specific type keywords here if needed (e.g., void)

    // Check for other general keywords
    if (value == "return" || value == "string" || value == "for" || value == "print" || value == "input" || value == "var" || value == "func" /* Add other general keywords like if, else, while, etc. */) {
        return { TokenType::Keyword, value };
    }

    // If it's not a recognized keyword, it's an identifier
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
