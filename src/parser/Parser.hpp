#ifndef PARSER_HPP
#define PARSER_HPP

#include "../lexer/Lexer.hpp"
#include "../symbol/ASTNode.hpp"
#include <memory>
#include <vector> 
#include <string> 
#include <utility>

class Parser {
public:
    Parser(Lexer& lexer);
    std::shared_ptr<ASTNode> parse();

private:
    Token currentToken;
    Lexer& lexer;

    void advance();
    void expect(TokenType type, const std::string& errorMessage);
    void expectSymbol(const std::string& symbol, const std::string& errorMessage);
    void expectKeyword(const std::string& keyword, const std::string& errorMessage);

    std::shared_ptr<ASTNode> parseBlock();
    std::shared_ptr<ASTNode> parseStatement();
    std::shared_ptr<ASTNode> parseDeclaration();
    std::shared_ptr<ASTNode> parseForStatement(); 
    std::shared_ptr<ASTNode> parseFunction();
    std::shared_ptr<ASTNode> parseReturnStatement(); 
    std::shared_ptr<ASTNode> parseAssignmentOrFunctionCallStatement(); 

    std::shared_ptr<ASTNode> parsePrimary();
    std::shared_ptr<ASTNode> parseExpression();
    std::shared_ptr<ASTNode> parseAssignment();
};

#endif

