#include "Lexer.hpp"
#include "SemanticAnalyzer.hpp"
#include <iostream>

int main() {
    std::string source = R"(
    {
    int x = 10;
    string nome = "João";
    x = 5;
    nome = "Maria";
    }

    x = 20;             // VAI DAR ERRO DE ESCOPO AQ 
    int y = "abc";      // ERRO DE TIPO
    z = 3;              // Z NÃO VAI SER DECLARADO
    )";


    Lexer lexer(source);
    SemanticAnalyzer analyzer(lexer);
    analyzer.analyze();

    return 0;
}
