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
    else if (node->nodeType == "Param") {
        auto parts = node->value.find(':');
        std::string paramName = node->value.substr(0, parts);
        std::string paramType = node->value.substr(parts + 1);
        symbolTable.declare(paramName, paramType);
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
            if (child->nodeType == "Param") {
                auto split = child->value.find(':');
                std::string name = child->value.substr(0, split);
                std::string type = child->value.substr(split + 1);
                symbolTable.declare(name, type);
            }
        }

        for (const auto& child : node->children) {
            if (child->nodeType != "Param") {
                analyzeNode(child);
            }
        }

        symbolTable.exitScope();
    }
}
