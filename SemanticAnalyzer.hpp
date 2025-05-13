#ifndef SEMANTIC_ANALYZER_HPP
#define SEMANTIC_ANALYZER_HPP

#include "Lexer.hpp"
#include "SymbolTable.hpp"

class SemanticAnalyzer {
public:
    SemanticAnalyzer(Lexer& lexer);
    void analyze();

private:
    Lexer& lexer;
    SymbolTable symbolTable;
    Token currentToken;

    void nextToken();
    void handleDeclaration();
    void handleUsage();
};

#endif
