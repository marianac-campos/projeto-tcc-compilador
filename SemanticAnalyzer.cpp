#include "SemanticAnalyzer.hpp"
#include <iostream>

SemanticAnalyzer::SemanticAnalyzer(Lexer& lexer) : lexer(lexer) {
    nextToken();
}

void SemanticAnalyzer::nextToken() {
    currentToken = lexer.nextToken();
}

void SemanticAnalyzer::analyze() {
    while (currentToken.type != TokenType::EndOfFile) {
        if (currentToken.type == TokenType::Keyword && (currentToken.value == "int" || currentToken.value == "string")) {
            handleDeclaration();
        } else if (currentToken.type == TokenType::Identifier) {
            handleUsage();
        } else if (currentToken.type == TokenType::Symbol && currentToken.value == "{") {
            symbolTable.enterScope();
            nextToken();
        } else if (currentToken.type == TokenType::Symbol && currentToken.value == "}") {
            symbolTable.exitScope();
            nextToken();
        } else {
            nextToken();
        }
    }
}

void SemanticAnalyzer::handleDeclaration() {
    std::string varType = currentToken.value;
    nextToken(); // move to identifier

    if (currentToken.type != TokenType::Identifier) {
        std::cerr << "Erro: identificador esperado após tipo '" << varType << "'.\n";
        return;
    }

    std::string varName = currentToken.value;

    if (!symbolTable.declare(varName, varType)) {
        std::cerr << "Erro: variável '" << varName << "' já declarada neste escopo.\n";
    }

    nextToken(); // move to = or ;

    if (currentToken.type == TokenType::Operator && currentToken.value == "=") {
        nextToken(); // move to value

        if ((varType == "int" && currentToken.type != TokenType::Number) ||
            (varType == "string" && currentToken.type != TokenType::String)) {
            std::cerr << "Erro: tipo incompatível na atribuição para variável '" << varName << "'.\n";
        }

        nextToken(); // move after value
    }

    if (currentToken.type == TokenType::Operator && currentToken.value == ";") {
        nextToken(); // consume ;
    }
}

void SemanticAnalyzer::handleUsage() {
    std::string varName = currentToken.value;

    if (!symbolTable.isDeclared(varName)) {
        std::cerr << "Erro: variável '" << varName << "' usada sem declaração.\n";
    }

    nextToken();
}
