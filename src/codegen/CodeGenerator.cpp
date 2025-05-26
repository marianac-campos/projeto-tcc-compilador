
#include "CodeGenerator.hpp"
#include <sstream>

CodeGenerator::CodeGenerator() : labelCount(0) {
    dataSection = ".DATA\n";
    codeSection = ".CODE\n";
}

std::string CodeGenerator::newLabel() {
    return "L" + std::to_string(labelCount++);
}

void CodeGenerator::generateNode(const std::shared_ptr<ASTNode>& node) {
    if (node->nodeType == "Program" || node->nodeType == "Block") {
        for (const auto& child : node->children) {
            generateNode(child);
        }
    }
    else if (node->nodeType == "Declaration") {
        auto pos = node->value.find(':');
        std::string varName = node->value.substr(0, pos);
        std::string varType = node->value.substr(pos + 1);

        dataSection += varName + " DW 0\n";

        if (!node->children.empty()) {
            codeSection += "MOV " + varName + ", ";
            codeSection += node->children[0]->value + "\n";
        }
    }
    else if (node->nodeType == "Expression") {
        // Simples atribuição ou uso
        codeSection += "; Expression uses " + node->value + "\n";
    }
    else if (node->nodeType == "If") {
        std::string elseLabel = newLabel();
        std::string endLabel = newLabel();

        codeSection += "; IF condition\n";
        codeSection += "CMP " + node->children[0]->value + ", 0\n";
        codeSection += "JE " + elseLabel + "\n";

        generateNode(node->children[1]); // if block

        codeSection += "JMP " + endLabel + "\n";
        codeSection += elseLabel + ":\n";

        if (node->children.size() > 2) {
            generateNode(node->children[2]); // else block
        }

        codeSection += endLabel + ":\n";
    }
    else if (node->nodeType == "While") {
        std::string startLabel = newLabel();
        std::string endLabel = newLabel();

        codeSection += startLabel + ":\n";
        codeSection += "CMP " + node->children[0]->value + ", 0\n";
        codeSection += "JE " + endLabel + "\n";

        generateNode(node->children[1]); // block

        codeSection += "JMP " + startLabel + "\n";
        codeSection += endLabel + ":\n";
    }
    else if (node->nodeType == "For") {
        std::string startLabel = newLabel();
        std::string endLabel = newLabel();

        generateNode(node->children[0]); // inicialização

        codeSection += startLabel + ":\n";
        codeSection += "CMP " + node->children[1]->value + ", 0\n";
        codeSection += "JE " + endLabel + "\n";

        generateNode(node->children[3]); // bloco

        generateNode(node->children[2]); // incremento

        codeSection += "JMP " + startLabel + "\n";
        codeSection += endLabel + ":\n";
    }
    else if (node->nodeType == "Function") {
        codeSection += node->value + ":\n";
        for (const auto& child : node->children) {
            generateNode(child);
        }
        codeSection += "RET\n";
    }
}

std::string CodeGenerator::generate(const std::shared_ptr<ASTNode>& root) {
    generateNode(root);
    return dataSection + "\n" + codeSection;
}
