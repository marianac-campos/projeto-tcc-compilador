
#include "Lexer.hpp"
#include "Parser.hpp"
#include "SemanticAnalyzer.hpp"
#include "CodeGenerator.hpp"
#include <iostream>
#include <memory>
#include <fstream>

// Função para imprimir a AST
void printAST(const std::shared_ptr<ASTNode>& node, int depth = 0) {
    for (int i = 0; i < depth; ++i) std::cout << "  ";
    std::cout << node->nodeType << ": " << node->value << std::endl;
    for (const auto& child : node->children) {
        printAST(child, depth + 1);
    }
}

int main() {
    std::string source = R"(
        {
            var x: int = 10;
            var nome: string = "João";
            if (x) {
                print(nome);
            } else {
                nome = "Maria";
            }
            while (x) {
                x = x + 1;
            }
            func soma(a: int, b: int): int {
                return a + b;
            }
        }
    )";

    Lexer lexer(source);
    Parser parser(lexer);
    SemanticAnalyzer semanticAnalyzer;
    CodeGenerator codeGenerator;

    try {
        auto ast = parser.parseProgram();
        std::cout << "==== AST ====" << std::endl;
        printAST(ast);

        semanticAnalyzer.analyze(ast);
        std::cout << "\n==== Análise Semântica: OK ====" << std::endl;

        auto assembly = codeGenerator.generate(ast);
        std::cout << "\n==== Assembly Gerado ====" << std::endl;
        std::cout << assembly << std::endl;

        // Opcional: salvar em arquivo
        std::ofstream outFile("output.asm");
        outFile << assembly;
        outFile.close();
        std::cout << "\nAssembly salvo em output.asm\n";

    } catch (const std::exception& ex) {
        std::cerr << "Erro: " << ex.what() << std::endl;
    }

    return 0;
}
