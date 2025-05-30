#include <iostream>
#include "lexer.h"

int main() {
    std::string code = "var x = 42;";
    Lexer lexer(code);
    std::vector<Token> tokens = lexer.tokenize();

    for (const auto& token : tokens) {
        std::cout << "Token: " << static_cast<int>(token.type) << ", Value: " << token.value << std::endl;
    }

    return 0;
}
