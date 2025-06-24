#include "Parser.hpp"
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

Parser::Parser(Lexer& lexer) : lexer(lexer) {
    advance();
}

void Parser::advance() {
    currentToken = lexer.nextToken();
}

void Parser::expect(TokenType type, const std::string& errorMessage) {
    if (currentToken.type != type) {
        throw std::runtime_error(errorMessage + " | Token atual: " + currentToken.value +
                                 " (Tipo: " + std::to_string(static_cast<int>(currentToken.type)) + ")");
    }
    advance();
}

void Parser::expectSymbol(const std::string& symbol, const std::string& errorMessage) {
    if (currentToken.value != symbol) {
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

std::shared_ptr<ASTNode> Parser::parsePrimary() {
    if (currentToken.type == TokenType::Number) {
        auto node = std::make_shared<ASTNode>("Number", currentToken.value);
        advance();
        return node;
    }

    if (currentToken.type == TokenType::String) {
        auto node = std::make_shared<ASTNode>("String", currentToken.value);
        advance();
        return node;
    }

    if (currentToken.type == TokenType::Identifier) {
        std::string name = currentToken.value;
        advance();

        if (currentToken.value == "(") {
            advance();
            auto callNode = std::make_shared<ASTNode>("FunctionCall", name);
            if (currentToken.value != ")") {
                while (true) {
                    callNode->children.push_back(parseExpression());
                    if (currentToken.value == ")") break;
                    expectSymbol(",", "Expected ',' between function arguments");
                }
            }
            expectSymbol(")", "Expected ')' after function call arguments");
            return callNode;
        } else {
            return std::make_shared<ASTNode>("Variable", name);
        }
    }

    if (currentToken.value == "(") {
        advance();
        auto node = parseExpression();
        expectSymbol(")", "Expected ')' after parenthesized expression");
        return node;
    }

    throw std::runtime_error("Unexpected token " + currentToken.value + " when expecting start of an expression");
}

std::shared_ptr<ASTNode> Parser::parseExpression() {
    auto left = parsePrimary();

    while (currentToken.type == TokenType::Operator) {
        std::string op = currentToken.value;
        advance();
        auto right = parsePrimary();
        auto node = std::make_shared<ASTNode>("BinaryOp", op);
        node->children.push_back(left);
        node->children.push_back(right);
        left = node;
    }
    return left;
}

std::shared_ptr<ASTNode> Parser::parseDeclaration() {
    expectKeyword("var", "Expected 'var' keyword");

    if (currentToken.type != TokenType::Identifier) {
        throw std::runtime_error("Expected variable name | Token atual: " + currentToken.value);
    }
    std::string varName = currentToken.value;
    advance();
    expectSymbol(":", "Expected ':' after variable name");

    if (currentToken.type != TokenType::Identifier && currentToken.type != TokenType::Keyword) {
        throw std::runtime_error("Expected variable type | Token atual: " + currentToken.value);
    }
    std::string varType = currentToken.value;
    advance();

    auto node = std::make_shared<ASTNode>("Declaration", varName + ":" + varType);

    if (currentToken.value == "=") {
        advance();
        node->children.push_back(parseExpression());
    }

    expectSymbol(";", "Expected ';' after variable declaration");
    return node;
}

std::shared_ptr<ASTNode> Parser::parseAssignmentOrFunctionCallStatement() {
    if (currentToken.type != TokenType::Identifier) {
        throw std::runtime_error("Expected identifier at start of statement | Token atual: " + currentToken.value);
    }
    std::string name = currentToken.value;
    advance();

    if (currentToken.value == "(") {
        advance();
        auto callNode = std::make_shared<ASTNode>("FunctionCallStatement", name);
        if (currentToken.value != ")") {
            while (true) {
                callNode->children.push_back(parseExpression());
                if (currentToken.value == ")") break;
                expectSymbol(",", "Expected ',' between function arguments");
            }
        }
        expectSymbol(")", "Expected ')' after function call arguments");
        expectSymbol(";", "Expected ';' after function call statement");
        return callNode;
    } else if (currentToken.value == "=") {
        expectSymbol("=", "Expected '=' after identifier in assignment");
        auto valueNode = parseExpression();
        auto assignmentNode = std::make_shared<ASTNode>("Assignment", "=");
        assignmentNode->children.push_back(std::make_shared<ASTNode>("Variable", name));
        assignmentNode->children.push_back(valueNode);
        expectSymbol(";", "Expected ';' after assignment statement");
        return assignmentNode;
    } else {
        throw std::runtime_error("Expected '(' for function call or '=' for assignment | Token atual: " + currentToken.value);
    }
}

std::shared_ptr<ASTNode> Parser::parseBlock() {
    expectSymbol("{", "Expected '{' to start a block");
    auto node = std::make_shared<ASTNode>("Block", "");
    while (currentToken.value != "}") {
        if (currentToken.type == TokenType::EndOfFile) {
            throw std::runtime_error("Unexpected end of file within block, missing '}'");
        }
        node->children.push_back(parseStatement());
    }
    expectSymbol("}", "Expected '}' to end a block");
    return node;
}

std::shared_ptr<ASTNode> Parser::parseFunction() {
    expectKeyword("func", "Expected 'func' keyword");

    if (currentToken.type != TokenType::Identifier) {
        throw std::runtime_error("Expected function name | Token atual: " + currentToken.value);
    }
    std::string functionName = currentToken.value;
    advance();

    expectSymbol("(", "Expected '(' after function name");

    std::vector<std::pair<std::string, std::string>> parameters;
    if (currentToken.value != ")") {
        while (true) {
            if (currentToken.type != TokenType::Identifier) {
                throw std::runtime_error("Expected parameter name | Token atual: " + currentToken.value);
            }
            std::string paramName = currentToken.value;
            advance();

            expectSymbol(":", "Expected ':' after parameter name");

            if (currentToken.type != TokenType::Identifier && currentToken.type != TokenType::Keyword) {
                throw std::runtime_error("Expected parameter type | Token atual: " + currentToken.value);
            }
            std::string paramType = currentToken.value;
            advance();

            parameters.push_back({paramName, paramType});

            if (currentToken.value == ")") break;
            expectSymbol(",", "Expected ',' between parameters");
        }
    }
    expectSymbol(")", "Expected ')' after parameter list");
    expectSymbol(":", "Expected ':' before return type");

    if (currentToken.type != TokenType::Identifier && currentToken.type != TokenType::Keyword) {
        throw std::runtime_error("Expected return type | Token atual: " + currentToken.value);
    }
    std::string returnType = currentToken.value;
    advance();

    auto body = parseBlock();

    auto node = std::make_shared<ASTNode>("Function", functionName + ":" + returnType);
    for (const auto& param : parameters) {
        auto paramNode = std::make_shared<ASTNode>("Parameter", param.first + ":" + param.second);
        node->children.push_back(paramNode);
    }
    node->children.push_back(body);
    return node;
}

std::shared_ptr<ASTNode> Parser::parseReturnStatement() {
    expectKeyword("return", "Expected 'return' keyword");
    auto node = std::make_shared<ASTNode>("Return", "");

    if (currentToken.value != ";") {
        node->children.push_back(parseExpression());
    }

    expectSymbol(";", "Expected ';' after return statement");
    return node;
}

std::shared_ptr<ASTNode> Parser::parseForStatement() {
    expectKeyword("for", "Expected 'for' keyword");
    expectSymbol("(", "Expected '(' after 'for'");

    auto node = std::make_shared<ASTNode>("ForLoop", "");

    if (currentToken.value == "var") {
        node->children.push_back(parseDeclaration());
    } else if (currentToken.type == TokenType::Identifier) {
        std::string varName = currentToken.value;
        advance();
        expectSymbol("=", "Expected '=' in for loop initializer");
        auto initValue = parseExpression();
        auto initAssign = std::make_shared<ASTNode>("Assignment", "=");
        initAssign->children.push_back(std::make_shared<ASTNode>("Variable", varName));
        initAssign->children.push_back(initValue);
        node->children.push_back(initAssign);
        expectSymbol(";", "Expected ';' after for loop initializer");
    } else {
        expectSymbol(";", "Expected ';' after empty initializer");
    }

    if (currentToken.value != ";") {
        node->children.push_back(parseExpression());
    }
    expectSymbol(";", "Expected ';' after for loop condition");

    if (currentToken.value != ")") {
        if (currentToken.type == TokenType::Identifier) {
            std::string varName = currentToken.value;
            advance();
            expectSymbol("=", "Expected '=' in for loop increment");
            auto incrValue = parseExpression();
            auto incrAssign = std::make_shared<ASTNode>("Assignment", "=");
            incrAssign->children.push_back(std::make_shared<ASTNode>("Variable", varName));
            incrAssign->children.push_back(incrValue);
            node->children.push_back(incrAssign);
        } else {
            throw std::runtime_error("Expected assignment in for loop increment | Token atual: " + currentToken.value);
        }
    }
    expectSymbol(")", "Expected ')' after for loop clauses");

    node->children.push_back(parseBlock());

    return node;
}

std::shared_ptr<ASTNode> Parser::parse() {
    auto root = std::make_shared<ASTNode>("Program", "");
    while (currentToken.type != TokenType::EndOfFile) {
        root->children.push_back(parseStatement());
    }
    return root;
}

std::shared_ptr<ASTNode> Parser::parseStatement() {
    if (currentToken.type == TokenType::Keyword) {
        if (currentToken.value == "var") return parseDeclaration();
        if (currentToken.value == "func") return parseFunction();
        if (currentToken.value == "return") return parseReturnStatement();
        if (currentToken.value == "for") return parseForStatement();
    } else if (currentToken.type == TokenType::Identifier) {
        return parseAssignmentOrFunctionCallStatement();
    } else if (currentToken.value == "{") {
        return parseBlock();
    }

    throw std::runtime_error("Unexpected token '" + currentToken.value + "' at start of statement");
}
