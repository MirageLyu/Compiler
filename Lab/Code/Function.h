#pragma once

#include "headers.h"
#include "Type.h"
#include "SymbolTable.h"

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
    
    string getName() const { return name; }
    Type getType() const { return return_type; }
    vector<Symbol> getArgs() const { return args; }
    int getLineno()const { return lineno; }

    vector<Type> getArgsTypeParam() const;

    bool isDefiniton() const { return def_or_dec == FUNC_DEF; };
    bool isDeclaration() const { return def_or_dec == FUNC_DEC; };

    bool operator==(const Function &func) const;
};