#ifndef ASTNODE_HPP
#define ASTNODE_HPP

#include <string>
#include <vector>
#include <memory>

struct ASTNode {
    std::string nodeType;            
    std::string value;               
    std::vector<std::shared_ptr<ASTNode>> children; 

    ASTNode(const std::string& type, const std::string& val)
        : nodeType(type), value(val) {{}}
};

#endif 
