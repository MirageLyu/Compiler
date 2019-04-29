#pragma once
#include "SymbolTable.h"
#include "headers.h"
#include "Type.h"

/* Ast Node Type */
enum ANT{
    /* Tokens */
    ANT_TYPE, ANT_STRUCT, ANT_IF, ANT_ELSE, ANT_WHILE, 
    ANT_RETURN, ANT_INT, ANT_FLOAT,
    ANT_ID, ANT_SEMI, ANT_COMMA, ANT_ASSIGNOP, 
    ANT_RELOP, ANT_PLUS, ANT_MINUS, ANT_STAR, 
    ANT_DIV, ANT_AND, ANT_OR, ANT_DOT, ANT_NOT, ANT_LP, 
    ANT_RP, ANT_LB, ANT_RB, ANT_LC, ANT_RC,

    /* High-Level Definitions */
    Program, ExtDefList, ExtDef, ExtDecList,
    Specifier, StructSpecifier, OptTag, Tag, 
    VarDec, FunDec, VarList, ParamDec,
    CompSt, StmtList, Stmt,
    DefList, Def, DecList, Dec,
    Exp, Args,

    ANT_ERROR, ANT_EMPTY,
};

enum Sattr{
    ATTR_DEC_SPEC, ATTR_STRUCT_SPEC, 
    ATTR_FUNCDEF_SPEC, ATTR_FUNCDEC_SPEC,
    
    ATTR_STRUCT_DEF, ATTR_STRUCT_DEC,

    ATTR_FUNC_VAR, ATTR_FUNC_EMPTY,

    ATTR_EMPTY_DEC, ATTR_ASSIGN_DEC,

    ATTR_EXP, ATTR_COMPST, ATTR_RETURN, 
    ATTR_IF, ATTR_IFELSE, ATTR_WHILE,

    ATTR_ASSIGN, ATTR_AND, ATTR_OR, ATTR_REL, ATTR_PLUS, 
    ATTR_MINUS, ATTR_STAR, ATTR_DIV, ATTR_NEST, 
    ATTR_NEG, ATTR_NOT, 
    ATTR_FUNC_ARGS_EXP, ATTR_FUNC_EMPTY_EXP, 
    ATTR_ARRAY, ATTR_STRUCT, ATTR_ID, ATTR_INT, ATTR_FLOAT,
};

/* ErRor Type */
enum ERT{
    MISSING, SYNERR, NO_ERROR,
};

/* Value Type */
enum VALT{
    IVAL, FVAL, SVAL, NONE,
};


class AstNode {
private:
    int lineno;
    ANT ant;
    union{
        int ival;
        float fval;
        string sval;
    };

    VALT valt;

    Sattr attr;

    ERT ert;

    AstNode* child;
    AstNode* sibling;

public:
    static AstNode* createNewAstNode(ANT, int, VALT, void*, vector<AstNode*>, ERT _e = NO_ERROR);

    static void reportSyntaxError(AstNode* ast, int lineno);

    bool right;

    AstNode(){
        child = NULL;
        sibling = NULL;
        right = false;
    }
    void setAttr(Sattr _attr){
        attr = _attr;
    }
    Sattr getAttr(){
        return attr;
    }
    int getLineNo(){
        return lineno;
    }
    void setErt(ERT e){
        this->ert = e;
    }
    ANT getAnt(){
        return ant;
    }
    void setValt(VALT v){
        this->valt = v;
    }
    void setIValue(int iv){
        this->ival = iv;
    }
    void setFValue(float fv){
        this->fval = fv;
    }
    void setSValue(string sv){
        this->sval = sv;
    }
    void setAnt(ANT a){
        this->ant = a;
    }
    string getSValue(){
        return sval;
    }
    void setLineNo(int no){
        this->lineno = no;
    }
    void setChild(AstNode* c){
        child = c;
    }
    void setNextSibling(AstNode* sb){
        sibling = sb;
    }

    AstNode* firstChild();
    AstNode* nextSibling();
    void printTree(int depth);

    void parseProgram();
    void parseExtDef();
    Type parseSpecifier();
    vector<Symbol> parseVarList();
    void parseCompSt(const Type& type);
    void parseExtDecList(const Type& type);
    vector<Symbol> parseDefList(bool);
    void parseDecList(vector<Symbol>& result, const Type& type, bool notInStruct);
    Type parseExp();
    void parseStmt(const Type& type);
    vector<Type> parseArgs();
};


