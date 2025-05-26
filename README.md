
# MACSLang Compiler

🚀 Compilador desenvolvido para a linguagem MACSLang, como parte do trabalho acadêmico da disciplina de Compiladores - UNI-BH.

## 🛠️ Fases implementadas
- ✔️ Analisador Léxico
- ✔️ Analisador Sintático (Parser + AST)
- ✔️ Analisador Semântico (Validação de escopo e tipos básicos)
- ✔️ Gerador de Código (Assembly x86 simplificado)

## 📜 Sobre a Linguagem MACSLang
- Paradigma: Imperativa e Estruturada
- Tipagem: Estática
- Suporte a:
  - Declaração de variáveis
  - Condicionais (`if/else`)
  - Laços (`while`, `for`)
  - Funções
  - Entrada (`input()`) e saída (`print()`)

## 📂 Estrutura do Projeto

```
📁 projeto-tcc-compilador
 ┣ 📁 src
 ┃ ┣ 📁 lexer
 ┃ ┃ ┣ 📄 Lexer.cpp / Lexer.hpp / Token.hpp
 ┃ ┣ 📁 parser
 ┃ ┃ ┣ 📄 Parser.cpp / Parser.hpp
 ┃ ┣ 📁 semantic
 ┃ ┃ ┣ 📄 SemanticAnalyzer.cpp / SemanticAnalyzer.hpp
 ┃ ┣ 📁 codegen
 ┃ ┃ ┣ 📄 CodeGenerator.cpp / CodeGenerator.hpp
 ┃ ┣ 📁 symbol
 ┃ ┃ ┣ 📄 SymbolTable.cpp / SymbolTable.hpp
 ┃ ┣ 📄 main.cpp
 ┣ 📁 bin (arquivos compilados)
 ┣ 📄 README.md
 ┗ 📄 output.asm
```

## 💻 Como compilar e executar

### 🔧 Compilar
Execute no terminal dentro da pasta `src`:

```bash
g++ lexer/*.cpp parser/*.cpp semantic/*.cpp codegen/*.cpp symbol/*.cpp main.cpp -o ../bin/compilador
```

### ▶️ Executar

```bash
cd ../bin
./compilador
```

## 📦 Saída

- O compilador irá gerar:
  - A árvore sintática (AST) no terminal
  - Mensagens de validação semântica
  - Código Assembly no terminal
  - Arquivo `output.asm` salvo na pasta principal

## 🏗️ Exemplo de código compilado (Assembly gerado)

```asm
.DATA
x DW 0
nome DW 0

.CODE
MOV x, 10
CMP x, 0
JE L1
PRINT nome
JMP L2
L1:
MOV nome, Maria
L2:
L3:
CMP x, 0
JE L4
MOV x, x + 1
JMP L3
L4:
soma:
RET
```

## 👨‍🏫 Observações
- Este compilador gera Assembly x86 simplificado, acadêmico.
- Focado na compreensão dos processos de compilação.

## ✨ Feito por:
- 💻 Gabriel Henrique, Gustavo Nunes e Mariana Campos
- 🎓 UNI-BH - Disciplina de Compiladores
