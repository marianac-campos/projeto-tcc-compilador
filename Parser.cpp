#include "Parser.hpp"
#include <stdexcept>
#include <iostream>

Parser::Parser(Lexer& lexer) : lexer(lexer) {
    advance();
}

void Parser::advance() {
    currentToken = lexer.nextToken();
}

void Parser::expect(TokenType type, const std::string& errorMessage) {
    if (currentToken.type != type) {
        throw std::runtime_error(errorMessage);
    }
    advance();
}

std::shared_ptr<ASTNode> Parser::parseProgram() {
    auto root = std::make_shared<ASTNode>("Program", "");
    while (currentToken.type != TokenType::EndOfFile) {
        root->children.push_back(parseStatement());
    }
    return root;
}

std::shared_ptr<ASTNode> Parser::parseStatement() {
    if (currentToken.value == "var") return parseDeclaration();
    if (currentToken.value == "if") return parseIf();
    if (currentToken.value == "while") return parseWhile();
    if (currentToken.value == "for") return parseFor();
    if (currentToken.value == "func") return parseFunction();
    if (currentToken.value == "{") return parseBlock();

    return parseExpression();
}

std::shared_ptr<ASTNode> Parser::parseBlock() {
    expect(TokenType::Symbol, "Expected '{'");
    auto node = std::make_shared<ASTNode>("Block", "");
    while (currentToken.value != "}") {
        node->children.push_back(parseStatement());
    }
    expect(TokenType::Symbol, "Expected '}'");
    return node;
}

std::shared_ptr<ASTNode> Parser::parseIf() {
    advance();
    auto node = std::make_shared<ASTNode>("If", "");
    node->children.push_back(parseExpression());
    node->children.push_back(parseBlock());      
    if (currentToken.value == "else") {
        advance();
        node->children.push_back(parseBlock()); 
    }
    return node;
}

std::shared_ptr<ASTNode> Parser::parseWhile() {
    advance();
    auto node = std::make_shared<ASTNode>("While", "");
    node->children.push_back(parseExpression());
    node->children.push_back(parseBlock());
    return node;
}

std::shared_ptr<ASTNode> Parser::parseFor() {
    advance();
    auto node = std::make_shared<ASTNode>("For", "");
    node->children.push_back(parseExpression()); 
    node->children.push_back(parseExpression()); 
    node->children.push_back(parseExpression()); 
    node->children.push_back(parseBlock());      
    return node;
}

std::shared_ptr<ASTNode> Parser::parseFunction() {
    advance();
    expect(TokenType::Identifier, "Expected function name");
    std::string funcName = currentToken.value;
    advance();

    auto node = std::make_shared<ASTNode>("Function", funcName);

    expect(TokenType::Symbol, "Expected '('");
    expect(TokenType::Symbol, "Expected ')'");

    expect(TokenType::Symbol, "Expected ':'");
    expect(TokenType::Identifier, "Expected return type");
    node->value += ":" + currentToken.value;
    advance();

    node->children.push_back(parseBlock());
    return node;
}

std::shared_ptr<ASTNode> Parser::parseDeclaration() {
    advance();
    expect(TokenType::Identifier, "Expected variable name");
    std::string varName = currentToken.value;
    advance();

    expect(TokenType::Symbol, "Expected ':'");
    expect(TokenType::Identifier, "Expected type");
    std::string varType = currentToken.value;
    advance();

    auto node = std::make_shared<ASTNode>("Declaration", varName + ":" + varType);

    if (currentToken.value == "=") {
        advance();
        node->children.push_back(parseExpression());
    }

    return node;
}

std::shared_ptr<ASTNode> Parser::parseExpression() {
    auto node = std::make_shared<ASTNode>("Expression", currentToken.value);
    advance();
    return node;
}
