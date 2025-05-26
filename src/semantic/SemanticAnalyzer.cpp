#include "SemanticAnalyzer.hpp"
#include <iostream>

SemanticAnalyzer::SemanticAnalyzer() {}

void SemanticAnalyzer::analyze(const std::shared_ptr<ASTNode>& root) {
    analyzeNode(root);
}

void SemanticAnalyzer::analyzeNode(const std::shared_ptr<ASTNode>& node) {
    if (node->nodeType == "Program" || node->nodeType == "Block") {
        symbolTable.enterScope();
        for (const auto& child : node->children) {
            analyzeNode(child);
        }
        symbolTable.exitScope();
    }
    else if (node->nodeType == "Declaration") {
        auto parts = node->value.find(':');
        std::string varName = node->value.substr(0, parts);
        std::string varType = node->value.substr(parts + 1);
        
        if (!symbolTable.declare(varName, varType)) {
            std::cerr << "Erro: variável '" << varName << "' já declarada neste escopo.\n";
        }
    }
    else if (node->nodeType == "Expression") {
        std::string varName = node->value;
        if (!symbolTable.isDeclared(varName)) {
            std::cerr << "Erro: variável '" << varName << "' não declarada.\n";
        }
    }
    else if (node->nodeType == "If" || node->nodeType == "While") {
        for (const auto& child : node->children) {
            analyzeNode(child);
        }
    }
    else if (node->nodeType == "For") {
        symbolTable.enterScope();

        analyzeNode(node->children[0]); 
        analyzeNode(node->children[1]); 
        analyzeNode(node->children[2]); 
        analyzeNode(node->children[3]); 

        symbolTable.exitScope();
    }
    else if (node->nodeType == "Function") {
        symbolTable.enterScope();
        for (const auto& child : node->children) {
            analyzeNode(child);
        }
        symbolTable.exitScope();
    }
}
