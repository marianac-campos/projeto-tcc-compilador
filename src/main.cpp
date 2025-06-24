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
        var fat: int = fatorial(numero);
    )";

    Lexer lexer(source);
    Parser parser(lexer);
    SemanticAnalyzer semanticAnalyzer;
    CodeGenerator codeGenerator;

    try {
        auto ast = parser.parse(); 
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
