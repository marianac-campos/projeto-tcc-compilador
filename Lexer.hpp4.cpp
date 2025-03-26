#include "Lexer.hpp"
#include <iostream>

int main() {
    Lexer lexer("int x = 10 + 5; return x;");

    Token token;
    while ((token = lexer.nextToken()).type != TokenType::EndOfFile) {
        std::cout << "Token: " << token.value << " (" << static_cast<int>(token.type) << ")\n";
    }

    return 0;
}
