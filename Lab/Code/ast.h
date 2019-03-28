
#include "headers.h"

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

    ERT ert;

    AstNode* child;
    AstNode* sibling;

public:
    static AstNode* createNewAstNode(ANT, int, VALT, void*, vector<AstNode*>, ERT _e = NO_ERROR);

    static void reportSyntaxError(AstNode* ast, int lineno);

    AstNode(){
        child = NULL;
        sibling = NULL;
    }
    int getLineNo(){
        return lineno;
    }
    void setErt(ERT e){
        this->ert = e;
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
};


