#include "Parser.hpp"
#include <stdexcept>
#include <iostream>
#include <vector>
#include <memory>
#include <map>

Parser::Parser(Lexer& lexer) : lexer(lexer) {
    advance();
}

void Parser::advance() {
    currentToken = lexer.nextToken();
}

void Parser::expect(TokenType type, const std::string& errorMessage) {
    if (currentToken.type != type) {
        throw std::runtime_error(errorMessage + " | Token atual: " + currentToken.value + " (Tipo: " + std::to_string(static_cast<int>(currentToken.type)) + ")");
    }
    advance();
}

void Parser::expectSymbol(const std::string& symbol, const std::string& errorMessage) {
    if (currentToken.type != TokenType::Symbol || currentToken.value != symbol) {
        throw std::runtime_error(errorMessage + " | Token atual: " + currentToken.value);
    }
    advance();
}

void Parser::expectKeyword(const std::string& keyword, const std::string& errorMessage) {
    if (currentToken.type != TokenType::Keyword || currentToken.value != keyword) {
        throw std::runtime_error(errorMessage + " | Token atual: " + currentToken.value);
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
    if (currentToken.type == TokenType::Keyword) {
        if (currentToken.value == "var") return parseDeclaration();
        if (currentToken.value == "if") return parseIf();
        if (currentToken.value == "while") return parseWhile();
        if (currentToken.value == "for") return parseFor();
        if (currentToken.value == "func") return parseFunction();
        if (currentToken.value == "print") return parsePrint();
        if (currentToken.value == "input") return parseInput();
        if (currentToken.value == "return") return parseReturn();
    }
    if (currentToken.type == TokenType::Symbol && currentToken.value == "{") {
        return parseBlock();
    }

    auto statementNode = parseExpression();
    expectSymbol(";", "Expected ';' after expression statement");
    return statementNode;
}

std::shared_ptr<ASTNode> Parser::parseBlock() {
    expectSymbol("{", "Expected '{' to start a block");
    auto node = std::make_shared<ASTNode>("Block", "");
    while (!(currentToken.type == TokenType::Symbol && currentToken.value == "}")) {
        if (currentToken.type == TokenType::EndOfFile) {
            throw std::runtime_error("Unexpected end of file within block, missing '}'");
        }
        node->children.push_back(parseStatement());
    }
    expectSymbol("}", "Expected '}' to end a block");
    return node;
}

std::shared_ptr<ASTNode> Parser::parseIf() {
    expectKeyword("if", "Expected 'if' keyword");
    expectSymbol("(", "Expected '(' after 'if'");
    auto node = std::make_shared<ASTNode>("If", "");
    node->children.push_back(parseExpression());
    expectSymbol(")", "Expected ')' after if condition");
    node->children.push_back(parseBlock());
    if (currentToken.type == TokenType::Keyword && currentToken.value == "else") {
        advance();
        node->children.push_back(parseBlock());
    }
    return node;
}

std::shared_ptr<ASTNode> Parser::parseWhile() {
    expectKeyword("while", "Expected 'while' keyword");
    expectSymbol("(", "Expected '(' after 'while'");
    auto node = std::make_shared<ASTNode>("While", "");
    node->children.push_back(parseExpression());
    expectSymbol(")", "Expected ')' after while condition");
    node->children.push_back(parseBlock());
    return node;
}

std::shared_ptr<ASTNode> Parser::parseFor() {
    expectKeyword("for", "Expected 'for' keyword");
    auto node = std::make_shared<ASTNode>("For", "");
    expectSymbol("(", "Expected '(' after 'for'");

    if (currentToken.type == TokenType::Symbol && currentToken.value == ";") {
        advance();
        node->children.push_back(std::make_shared<ASTNode>("Empty", ""));
    } else {
        if (currentToken.type == TokenType::Keyword && currentToken.value == "var") {
            node->children.push_back(parseDeclaration());
        } else {
            node->children.push_back(parseExpression());
            expectSymbol(";", "Expected ';' after for initialization expression");
        }
    }

    if (currentToken.type == TokenType::Symbol && currentToken.value == ";") {
        advance();
        node->children.push_back(std::make_shared<ASTNode>("EmptyCondition", ""));
    } else {
        node->children.push_back(parseExpression());
        expectSymbol(";", "Expected ';' after for condition");
    }

    if (currentToken.type == TokenType::Symbol && currentToken.value == ")") {
        node->children.push_back(std::make_shared<ASTNode>("Empty", ""));
    } else {
        node->children.push_back(parseExpression());
    }

    expectSymbol(")", "Expected ')' after for parts");
    node->children.push_back(parseBlock());
    return node;
}

std::shared_ptr<ASTNode> Parser::parseFunction() {
    expectKeyword("func", "Expected 'func' keyword");
    if (currentToken.type != TokenType::Identifier) {
        throw std::runtime_error("Expected function name | Token atual: " + currentToken.value);
    }
    std::string funcName = currentToken.value;
    advance();
    auto node = std::make_shared<ASTNode>("Function", funcName);
    expectSymbol("(", "Expected '(' after function name");

    bool firstParam = true;
    while (!(currentToken.type == TokenType::Symbol && currentToken.value == ")")) {
        if (!firstParam) {
            expectSymbol(",", "Expected ',' between parameters");
        }
        firstParam = false;
        if (currentToken.type != TokenType::Identifier) {
            throw std::runtime_error("Expected parameter name | Token atual: " + currentToken.value);
        }
        std::string paramName = currentToken.value;
        advance();
        expectSymbol(":", "Expected ':' after parameter name");
        if (currentToken.type != TokenType::Identifier) {
            throw std::runtime_error("Expected parameter type | Token atual: " + currentToken.value);
        }
        std::string paramType = currentToken.value;
        advance();
        node->children.push_back(std::make_shared<ASTNode>("Param", paramName + ":" + paramType));
    }

    expectSymbol(")", "Expected ')' after parameters");
    expectSymbol(":", "Expected ':' before return type");

    if (currentToken.type != TokenType::Identifier) {
        throw std::runtime_error("Expected return type | Token atual: " + currentToken.value);
    }
    std::string returnType = currentToken.value;
    advance();
    node->value += ":" + returnType;
    node->children.push_back(parseBlock());
    return node;
}

std::shared_ptr<ASTNode> Parser::parseDeclaration() {
    expectKeyword("var", "Expected 'var' keyword");
    if (currentToken.type != TokenType::Identifier) {
        throw std::runtime_error("Expected variable name | Token atual: " + currentToken.value);
    }
    std::string varName = currentToken.value;
    advance();
    expectSymbol(":", "Expected ':' after variable name");
    if (currentToken.type != TokenType::Identifier) {
        throw std::runtime_error("Expected type | Token atual: " + currentToken.value);
    }
    std::string varType = currentToken.value;
    advance();
    auto node = std::make_shared<ASTNode>("Declaration", varName + ":" + varType);
    if (currentToken.type == TokenType::Operator && currentToken.value == "=") {
        advance();
        node->children.push_back(parseExpression());
    }
    expectSymbol(";", "Expected ';' after variable declaration");
    return node;
}

std::shared_ptr<ASTNode> Parser::parsePrint() {
    expectKeyword("print", "Expected 'print' keyword");
    expectSymbol("(", "Expected '(' after 'print'");
    auto node = std::make_shared<ASTNode>("Print", "");
    node->children.push_back(parseExpression());
    expectSymbol(")", "Expected ')' after print expression");
    expectSymbol(";", "Expected ';' after print statement");
    return node;
}

std::shared_ptr<ASTNode> Parser::parseInput() {
    expectKeyword("input", "Expected 'input' keyword");
    expectSymbol("(", "Expected '(' after 'input'");
    auto node = std::make_shared<ASTNode>("Input", "");
    if (currentToken.type != TokenType::Identifier) {
        throw std::runtime_error("Expected variable name for input | Token atual: " + currentToken.value);
    }
    node->children.push_back(std::make_shared<ASTNode>("Identifier", currentToken.value));
    advance();
    expectSymbol(")", "Expected ')' after input variable");
    expectSymbol(";", "Expected ';' after input statement");
    return node;
}

std::shared_ptr<ASTNode> Parser::parseReturn() {
    expectKeyword("return", "Expected 'return' keyword");
    auto node = std::make_shared<ASTNode>("Return", "");
    if (!(currentToken.type == TokenType::Symbol && currentToken.value == ";")) {
        node->children.push_back(parseExpression());
    }
    expectSymbol(";", "Expected ';' after return statement");
    return node;
}

int getPrecedence(const Token& token) {
    if (token.type != TokenType::Operator) return -1;
    if (token.value == "*" || token.value == "/") return 5;
    if (token.value == "+" || token.value == "-") return 4;
    if (token.value == "==" || token.value == "!=" || token.value == "<" || token.value == ">" || token.value == "<=" || token.value == ">=") return 3;
    if (token.value == "=") return 1;
    return -1;
}

std::shared_ptr<ASTNode> Parser::parsePrimaryExpression() {
    switch (currentToken.type) {
        case TokenType::Identifier: {
            std::string identifierValue = currentToken.value;
            advance();

            if (currentToken.type == TokenType::Symbol && currentToken.value == "(") {
                advance();
                auto callNode = std::make_shared<ASTNode>("FunctionCall", identifierValue);
                bool firstArg = true;
                while (!(currentToken.type == TokenType::Symbol && currentToken.value == ")")) {
                    if (currentToken.type == TokenType::EndOfFile) {
                        throw std::runtime_error("Unexpected end of file during function call arguments for " + identifierValue);
                    }
                    if (!firstArg) {
                        expectSymbol(",", "Expected ',' between function arguments");
                    }
                    firstArg = false;
                    callNode->children.push_back(parseExpression());
                }
                expectSymbol(")", "Expected ')' after function arguments for " + identifierValue);
                return callNode;
            } else {
                return std::make_shared<ASTNode>("IdentifierExpr", identifierValue);
            }
        }
        case TokenType::Integer:
        case TokenType::Float:
        case TokenType::String:
        case TokenType::Boolean:
        case TokenType::Char: {
            std::string nodeType;
            if (currentToken.type == TokenType::Integer) nodeType = "IntegerLiteral";
            else if (currentToken.type == TokenType::Float) nodeType = "FloatLiteral";
            else if (currentToken.type == TokenType::String) nodeType = "StringLiteral";
            else if (currentToken.type == TokenType::Boolean) nodeType = "BooleanLiteral";
            else nodeType = "CharLiteral";
            auto node = std::make_shared<ASTNode>(nodeType, currentToken.value);
            advance();
            return node;
        }
        case TokenType::Symbol:
            if (currentToken.value == "(") {
                advance();
                auto exprNode = parseExpression();
                expectSymbol(")", "Expected ')' after parenthesized expression");
                return exprNode;
            }
            [[fallthrough]];
        default:
            throw std::runtime_error("Unexpected token '" + currentToken.value + "' when expecting start of an expression");
    }
}

std::shared_ptr<ASTNode> Parser::parseExpression(int precedence) {
    auto left = parsePrimaryExpression();

    while (true) {
        int currentPrecedence = getPrecedence(currentToken);
        if (currentPrecedence < precedence) {
            break;
        }

        Token op = currentToken;
        advance();

        auto right = parseExpression(currentPrecedence + 1);

        auto binaryOpNode = std::make_shared<ASTNode>("BinaryOp", op.value);
        binaryOpNode->children.push_back(left);
        binaryOpNode->children.push_back(right);
        left = binaryOpNode;
    }

    return left;
}
