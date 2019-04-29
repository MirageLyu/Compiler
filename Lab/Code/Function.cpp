#include "Function.h"
#include "ast.h"
#include "Type.h"

vector<Type> Function::getArgsTypeParam() const{
    vector<Type> typesParam;
    for(int i=0; i<args.size(); ++i){
        Type tmp = args[i].getType();
        typesParam.push_back(tmp);
    }
    return typesParam;
}

bool Function::operator==(const Function &func) const{
    if(name != func.getName() || !(return_type == func.getType()) || args.size() != func.args.size() ){
        return false;        
    }

    int sz = args.size();
    for(int i=0; i<sz; i++){
        if(!(args[i].getType() == func.args[i].getType())){
            return false;
        }
    }
    return true;
}

Function::Function(AstNode* funDec, const Type& type, bool isDefinition){
    return_type = type;
    name = funDec->firstChild()->getSValue();
    if(isDefinition){
        def_or_dec = FUNC_DEF;
    }
    else{
        def_or_dec = FUNC_DEC;
    }
    if(funDec->getAttr() == ATTR_FUNC_VAR){
        AstNode* varList = funDec->firstChild()->nextSibling()->nextSibling();
        args = varList->parseVarList();
    }
    lineno = funDec->getLineNo();
}

string Function::getArgsParamString(){
    vector<Type> types = getArgsTypeParam();
    if(types.empty())
        return "()";
    else{
        string paramString = string("(");
        for(int i=0; i<types.size(); i++){
            if(i==0){
                paramString.append(types[i].getTypeName());
            }
            else{
                paramString.append(",").append(types[i].getTypeName());
            }
        }
        paramString.append(")");
        return paramString;
    }
}