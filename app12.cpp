#include <iostream>
#include <string>
#include <cctype>
#include <vector>

enum class TokenType {
    Keyword, Identifier, Number, Operator, EndOfFile, Invalid
};

struct Token {
    TokenType type;
    std::string value;
};

class Lexer {
public:
    Lexer(const std::string& source) : source(source), currentPosition(0) {}

    Token nextToken() {
        // Ignorar espaços em branco
        while (currentPosition < source.length() && std::isspace(source[currentPosition])) {
            currentPosition++;
        }

        // Se atingiu o fim do código, retornar EOF
        if (currentPosition >= source.length()) return { TokenType::EndOfFile, "" };

        char currentChar = source[currentPosition];

        // Se for letra (palavra-chave ou identificador)
        if (std::isalpha(currentChar)) {
            return readIdentifierOrKeyword();
        }
        // Se for número
        else if (std::isdigit(currentChar)) {
            return readNumber();
        }
        // Se for operador (+, -, =, etc.)
        else if (currentChar == '+' || currentChar == '-' || currentChar == '=' || currentChar == ';') {
            return readOperator();
        }
        // Caso contrário, retornar "Invalid"
        else {
            std::cerr << "Invalid character: " << currentChar << std::endl;
            currentPosition++; // Avançar para o próximo caractere
            return { TokenType::Invalid, std::string(1, currentChar) };
        }
    }

private:
    std::string source;
    size_t currentPosition;

    Token readIdentifierOrKeyword() {
        std::string value;
        while (currentPosition < source.length() && std::isalnum(source[currentPosition])) {
            value += source[currentPosition++];
        }
        return { TokenType::Identifier, value };
    }

    Token readNumber() {
        std::string value;
        while (currentPosition < source.length() && std::isdigit(source[currentPosition])) {
            value += source[currentPosition++];
        }
        return { TokenType::Number, value };
    }

    Token readOperator() {
        char currentChar = source[currentPosition++];
        return { TokenType::Operator, std::string(1, currentChar) };
    }
};

int main() {
    Lexer lexer("int x = 10 + 5;");
    Token token;
    while ((token = lexer.nextToken()).type != TokenType::EndOfFile) {
        std::cout << "Token: " << token.value << "\n";
    }
    return 0;
}
