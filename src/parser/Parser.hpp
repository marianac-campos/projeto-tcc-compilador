#ifndef PARSER_HPP
#define PARSER_HPP

#include "../lexer/Lexer.hpp"
#include <vector>
#include <memory>
#include <string>

struct Token;

struct ASTNode {
    std::string nodeType;
    std::string value;
    std::vector<std::shared_ptr<ASTNode>> children;

    ASTNode(const std::string& type, const std::string& val)
        : nodeType(type), value(val) {}
};

class Parser {
public:
    Parser(Lexer& lexer);

    std::shared_ptr<ASTNode> parseProgram();

private:
    Lexer& lexer;
    Token currentToken;

    void advance();
    void expect(TokenType type, const std::string& errorMessage);
    void expectSymbol(const std::string& symbol, const std::string& errorMessage);
    void expectKeyword(const std::string& keyword, const std::string& errorMessage);

    std::shared_ptr<ASTNode> parseStatement();
    std::shared_ptr<ASTNode> parseExpression(int precedence = 0);
    std::shared_ptr<ASTNode> parsePrimaryExpression();
    std::shared_ptr<ASTNode> parseBlock();
    std::shared_ptr<ASTNode> parseIf();
    std::shared_ptr<ASTNode> parseWhile();
    std::shared_ptr<ASTNode> parseFor();
    std::shared_ptr<ASTNode> parseFunction();
    std::shared_ptr<ASTNode> parseDeclaration();
    std::shared_ptr<ASTNode> parsePrint();
    std::shared_ptr<ASTNode> parseInput();
    std::shared_ptr<ASTNode> parseReturn();
};

#endif
