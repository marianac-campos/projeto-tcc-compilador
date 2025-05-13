#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <string>
#include <vector>
#include <algorithm>

struct Symbol {
    std::string name;
    std::string type;
    int scopeLevel;
};

class SymbolTable {
public:
    void enterScope();
    void exitScope();
    bool declare(const std::string& name, const std::string& type);
    bool isDeclared(const std::string& name) const;
    std::string getType(const std::string& name) const;

private:
    std::vector<Symbol> symbols;
    int currentScopeLevel = 0;
};

#endif
