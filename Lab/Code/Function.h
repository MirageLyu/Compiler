#pragma once

#include "headers.h"
#include "Type.h"

class Function{
private:
    string name;
    Type return_type;
    vector<Symbol> args;
    enum {FUNC_DEF, FUNC_DEC} def_or_dec;
    int lineno;
public:
    Function() = default;
    Function(const Function &func) = default;
    Function& operator=(const Function &func) = default;
    Function(AstNode* funDec, const Type& type, bool isDefinition);
    
    string getName() const { return name; }
    Type getType() const { return return_type; }
    vector<Symbol> getArgs() const { return args; }
    int getLineno()const { return lineno; }

    vector<Type> getArgsTypeParam() const;

    bool isDefinition() const { return def_or_dec == FUNC_DEF; };
    bool isDeclaration() const { return def_or_dec == FUNC_DEC; };

    bool operator==(const Function &func) const;

    string getArgsParamString();
};