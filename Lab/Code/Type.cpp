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
        //recursively copy type
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
        //recursively copy type
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

bool Type::isArrTypeEqual(const Type& type)const{
    int dimension1 = 0, dimension2 = 0;
    Type* elem1 = arr_type.elem, *elem2 = type.arr_type.elem;
    while(elem1->kind = ARRAY){
        dimension1++;
        elem1 = elem1->arr_type.elem;
    }
    while(elem2->kind = ARRAY){
        dimension2++;
        elem2 = elem2->arr_type.elem;
    }
    return dimension1==dimension2 && (*elem1 == *elem2);
}

bool Type::isStructTypeEqual(const Type& type) const{
    return struct_type.name == type.struct_type.name;
}

string Type::getTypeName() const {
    if(kind == BASIC){
        if(basic_type == TYPE_INT){
            return "int";
        }
        else if(basic_type == TYPE_FLOAT){
            return "float";
        }
    }
    else if(kind == STRUCT){
        string type_name = "struct ";
        type_name.append(struct_type.name);
        return type_name;
    }
    else if(kind == ARRAY){
        int dimension = 1;
        Type* e = arr_type.elem;
        for( ; e->kind == ARRAY; e = e->arr_type.elem){
            dimension++;
        }
        string type_name =e->getTypeName();
        for(int i=0; i<dimension; i++){
            type_name.append("[]");
        }
        return type_name;
    }
    return "error_type";
}