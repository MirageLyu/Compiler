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
    if(name != func.getName() || return_type == func.getType() || args.size() != func.args.size() ){
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