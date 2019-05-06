#include "SymbolTable.h"
#include "ast.h"

Symbol* SymbolTable::getSymbol(const string &name) const {
    map<string, Symbol>::const_iterator iter = symbols.find(name);
    if(iter == symbols.end())
        return NULL;
    else{
        return (Symbol*)&(iter->second);
    }
}

Function* SymbolTable::getFuncDefinition(const string &name) const{
    map<string, Function>::const_iterator iter = functions_def.find(name);
    if(iter == functions_def.end())
        return NULL;
    else{
        return (Function*)&(iter->second);
    }
}

Function* SymbolTable::getFuncDeclaration(const string &name) const{
    map<string, Function>::const_iterator iter = functions_dec.find(name);
    if(iter == functions_dec.end())
        return NULL;
    else{
        return (Function*)&(iter->second);
    }
}

Type* SymbolTable::getStructType(const string &name) const{
    map<string, Type>::const_iterator iter = struct_types.find(name);
    if(iter == struct_types.end())
        return NULL;
    else{
        return (Type*)&(iter->second);
    }
}

void SymbolTable::addSymbol(const Symbol &symbol){
    string name = symbol.getName();
    if(hasStructType(name) || hasSymbolName(name)){
        string msg = "Redefined variable";
        msg.append("\"");
        msg.append(name);
        msg.append("\".");
        reportSemanticError(3, msg, symbol.getlineno());
    }
    else{
        symbols.emplace(name, symbol);
    }
}

void SymbolTable::addFuncDeclaration(const Function &func){
    string name = func.getName();
    if(hasFuncDeclaration(name)){
        Function* f = getFuncDeclaration(name);
        if(!(*f == func)){
            string msg = "Inconsistent declaration of function ";
            msg.append("\"");
            msg.append(name);
            msg.append("\".");
            reportSemanticError(19, msg, func.getLineno());
        }
    }
    else{
        functions_dec.emplace(name, func);
    }
}

void SymbolTable::addFuncDefinition(const Function &func){
    string name = func.getName();
    if(hasFuncDefinition(name)){
        string msg = "Redefined function ";
        msg.append("\"");
        msg.append(name);
        msg.append("\".");
        reportSemanticError(4, msg, func.getLineno());
    }
    else{
        functions_def.emplace(name, func);
        if(hasFuncDeclaration(name)){
            Function* f = getFuncDeclaration(name);
            if(!(*f == func)){
                string msg = "Inconsistent declaration of function ";
                msg.append("\"");
                msg.append(name);
                msg.append("\".");
                reportSemanticError(19, msg, f->getLineno());
            }
        }
    }
}

void SymbolTable::addStructType(const Type &type){
    string name = type.getStructName();
    if(hasStructType(name) || hasSymbolName(name)){
        string msg = "Duplicated name ";
        msg.append("\"");
        msg.append(name);
        msg.append("\".");
        reportSemanticError(16, msg, type.getLineno());
    }
    else{
        struct_types.emplace(name, type);
    }
}

void SymbolTable::checkFuncDefinition(){
    map<string, Function>::iterator iter = functions_dec.begin();
    while(iter != functions_dec.end()){
        string name = iter->second.getName();
        if(!hasFuncDefinition(name)){
            string msg = "Undefined function ";
            msg.append("\"");
            msg.append(name);
            msg.append("\".");
            reportSemanticError(18, msg, iter->second.getLineno());
        }
        iter++;
    }
}

Symbol::Symbol(AstNode* parameter){
    AstNode* specifier = parameter->firstChild(), *varDec = specifier->nextSibling();
    AstNode* id = varDec;
    while(id->getAnt() == VarDec)
        id = id->firstChild();
    
    name = id->getSValue();
    lineno = parameter->getLineNo();
    type = specifier->parseSpecifier();
}

Symbol::Symbol(AstNode* varDec, const Type& _type){
    //cout<<_type.getTypeName()<<endl;
    AstNode* child = varDec->firstChild();

    if(child->getAnt() == ANT_ID){
        //cout<<_type.getTypeName()<<endl;
        type = _type;
    }
    else{
        type = Type(varDec, type);
    }
    while(child->getAnt() == VarDec){
        child = child->firstChild();
    }

    lineno = child->getLineNo();
    name = child->getSValue();
}