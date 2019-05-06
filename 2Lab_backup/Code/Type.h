#pragma once

#include "headers.h"

class AstNode;
class Symbol;

class Type {
private:
    enum {BASIC, ARRAY, STRUCT, error_type} kind;
    enum {TYPE_INT, TYPE_FLOAT} basic_type;
    struct {
        Type *elem;
        int size;
    } arr_type;
    struct {
        string name;
        vector<Symbol> fields;
    } struct_type;

    int lineno;

    bool isArrTypeEqual(const Type& type) const;
    bool isStructTypeEqual(const Type& type) const;
public:
    Type(){ kind =  error_type; }

    explicit Type(bool int0Rfloat);
    explicit Type(AstNode* Specifier);
    Type(AstNode* varDec, const Type &type);
    
    Type(const Type &type);
    Type& operator=(const Type &type);

    bool operator==(const Type &type) const;

    int getLineno() const { return lineno; }
    string getStructName() const { return struct_type.name; }
    string getTypeName() const;

    bool isBasic() const { return kind == BASIC; }
    bool isArray() const { return kind == ARRAY; }
    bool isStruct() const { return kind == STRUCT; }
    bool isInt() const { return kind == BASIC && basic_type == TYPE_INT; }
    bool isFloat() const { return kind == BASIC && basic_type == TYPE_FLOAT; }
    bool isErrorType() const { return kind == error_type; }

    Symbol* hasStructField(const string &name);
    Type getArrayElem() const {return *arr_type.elem; };
};