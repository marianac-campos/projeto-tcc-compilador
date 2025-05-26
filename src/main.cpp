#include "../src/lexer/Lexer.hpp"
#include "../src/parser/Parser.hpp"
#include "../src/semantic/SemanticAnalyzer.hpp"
#include "../src/codegen/CodeGenerator.hpp"
#include <iostream>
#include <memory>
#include <fstream>

void printAST(const std::shared_ptr<ASTNode>& node, int depth = 0) {
    for (int i = 0; i < depth; ++i) std::cout << "  ";
    std::cout << node->nodeType << ": " << node->value << std::endl;
    for (const auto& child : node->children) {
        printAST(child, depth + 1);
    }
}

int main() {
    std::string source = R"(
        func fatorial(n: int): int {
            var resultado: int = 1;
            for (var i: int = 1; i <= n; i = i + 1) {
                resultado = resultado * i;
            }
            return resultado;
        }

        print("Digite um número:");
        var numero: int;
        input(numero);

        var fat: int = fatorial(numero);
        print("O fatorial de " + numero + " é " + fat);
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
