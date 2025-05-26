#ifndef SEMANTIC_ANALYZER_HPP
#define SEMANTIC_ANALYZER_HPP

#include "../parser/Parser.hpp"
#include "../symbol/SymbolTable.hpp"
#include <memory>

class SemanticAnalyzer {
public:
    SemanticAnalyzer();
    void analyze(const std::shared_ptr<ASTNode>& root);

private:
    SymbolTable symbolTable;

    void analyzeNode(const std::shared_ptr<ASTNode>& node);
};

#endif
