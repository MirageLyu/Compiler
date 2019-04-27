#include "Type.h"

Type::Type(bool int0Rfloat){
    kind = BASIC;
    if(int0Rfloat)
        basic_type = TYPE_INT;
    else
        basic_type = TYPE_FLOAT;
}

Type::Type(const Type& type){
    kind = type.kind;
    lineno = type.lineno;
    basic_type = type.basic_type;

    if(type.kind == ARRAY){
        arr_type.elem = new Type(*(type.arr_type.elem));
        arr_type.size = type.arr_type.size;
    }
    else if(type.kind == STRUCT){
        struct_type.name = type.struct_type.name;
        for(int i=0; i<type.struct_type.fields.size(); i++){
            struct_type.fields.push_back(type.struct_type.fields[i]);
        }
    }
}

Type& Type::operator=(const Type &type){
    kind = type.kind;
    lineno = type.lineno;
    basic_type = type.basic_type;

    if(type.kind == ARRAY){
        arr_type.elem = new Type(*(type.arr_type.elem));
        arr_type.size = type.arr_type.size;
    }
    else if(type.kind == STRUCT){
        struct_type.name = type.struct_type.name;
        for(int i=0; i<type.struct_type.fields.size(); i++){
            struct_type.fields.push_back(type.struct_type.fields[i]);
        }
    }
    return *this;
}

bool Type::operator==(const Type& type) const {
    if(kind != type.kind)
        return false;
    if(kind == BASIC){
        return basic_type == type.basic_type;
    }
    else if(kind == ARRAY){
        return isArrTypeEqual(type);
    }
    else if(kind == STRUCT){
        return isStructTypeEqual(type);
    }
    return false;
}