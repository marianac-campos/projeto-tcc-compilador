#include "SymbolTable.hpp"

void SymbolTable::enterScope() {
    currentScopeLevel++;
}

void SymbolTable::exitScope() {
    symbols.erase(
        std::remove_if(symbols.begin(), symbols.end(),
            [this](const Symbol& sym) {
                return sym.scopeLevel == currentScopeLevel;
            }),
        symbols.end()
    );
    currentScopeLevel--;
}

bool SymbolTable::declare(const std::string& name, const std::string& type) {
    for (const auto& sym : symbols) {
        if (sym.name == name && sym.scopeLevel == currentScopeLevel) {
            return false;
        }
    }
    symbols.push_back({ name, type, currentScopeLevel });
    return true;
}

bool SymbolTable::isDeclared(const std::string& name) const {
    for (auto it = symbols.rbegin(); it != symbols.rend(); ++it) {
        if (it->name == name) return true;
    }
    return false;
}

std::string SymbolTable::getType(const std::string& name) const {
    for (auto it = symbols.rbegin(); it != symbols.rend(); ++it) {
        if (it->name == name) return it->type;
    }
    return "";
}
