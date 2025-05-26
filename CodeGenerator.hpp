
#ifndef CODE_GENERATOR_HPP
#define CODE_GENERATOR_HPP

#include "Parser.hpp"
#include <string>
#include <memory>

class CodeGenerator {
public:
    CodeGenerator();
    std::string generate(const std::shared_ptr<ASTNode>& root);

private:
    int labelCount;
    std::string dataSection;
    std::string codeSection;

    void generateNode(const std::shared_ptr<ASTNode>& node);
    std::string newLabel();
};

#endif
