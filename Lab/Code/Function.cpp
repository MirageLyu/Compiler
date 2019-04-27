#include "Function.h"

vector<Type> Function::getArgsTypeParam() const{
    vector<Type> typesParam;
    for(int i=0; i<args.size(); ++i){
        Type tmp = args[i].getType();
        typesParam.push_back(tmp);
    }
    return typesParam;
}

bool Function::operator==(const Function &func) const{
    if(name != func.getName() || return_type == func.getType() || args.size() != func.getArgs().size() ){
        return false;        
    }
    //more TODO: check each arg's type
}