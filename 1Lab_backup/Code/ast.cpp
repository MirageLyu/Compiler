#include "ast.h"

extern bool SYNTAX_ERROR;
AstNode* AST = NULL;

string ANTMap[] = {
    "TYPE", "STRUCT", "IF", "ELSE", "WHILE", "RETURN", "INT",
    "FLOAT", "ID", "SEMI", "COMMA", "ASSIGNOP", "RELOP", "PLUS",
    "MINUS", "STAR", "DIV", "AND", "OR", "DOT", "NOT", "LP",
    "RP", "LB", "RB", "LC", "RC", "Program", "ExtDefList", "ExtDef",
    "ExtDecList", "Specifier", "StructSpecifier", "OptTag", "Tag",
    "VarDec", "FunDec", "VarList", "ParamDec", "CompSt", "StmtList",
    "Stmt", "DefList", "Def", "DecList", "Dec", "Exp", "Args"
};

AstNode* AstNode::firstChild(){
    return this->child;
}
AstNode* AstNode::nextSibling(){
    return this->sibling;
}
void AstNode::printTree(int depth){
    if(ant == ANT_EMPTY)
        return;
    for(int i=0; i<2*depth; i++){
        cout<<" ";
    }
    cout << ANTMap[ant];
    if(ant == ANT_ID || ant == ANT_TYPE){
        cout<<": " << sval << endl;
    }
    else if(ant == ANT_FLOAT)
        cout << ": "<< fval << endl;
    else if(ant == ANT_INT)
        cout << ": " << ival << endl;
    else if(ant <= 26){
        cout << endl;
    }
    else{
        cout<< " ("<<lineno<<")\n";
        AstNode* nxt = child;
        while(nxt != NULL){
            nxt->printTree(depth+1);
            nxt = nxt -> sibling;
        }
    }
}

void AstNode::reportSyntaxError(AstNode* ast, int lineno){
    string msg;
    if(ast->ert == MISSING){
        msg.append("Missing character: \'");
        msg.append(ast->sval);
        msg.append("\'.");
    }
    else {
        msg.append("syntax error");
    }
    printf("\nError type B at Line %d: %s.\n", lineno, msg.c_str());
}

/* AST Factory */
AstNode* AstNode::createNewAstNode(ANT _ant, int _lineno, VALT _valt, void* _val, vector<AstNode*> _children, ERT _e){
/* ParaList: _ant, _lineno, _valt, _val, _children, ERT _e = NO_ERROR */
    AstNode* node = new AstNode();
    node->setAnt(_ant);
    
    if(_children.empty())
        node->setChild(NULL);
    else{
        node->setChild(_children[0]);
        for(int i=0; i<_children.size(); i++){
            if(i == _children.size()-1)
                _children[i]->setNextSibling(NULL);
            else
                _children[i]->setNextSibling(_children[i+1]);
        }
    }

    node->setLineNo(_lineno);
    node->setNextSibling(NULL); //Built by high level Unterminaters
    node->setErt(_e);
    node->setValt(_valt);
    switch(_valt){
        case IVAL: node->setIValue(*((int*)_val)); break;
        case FVAL: node->setFValue(*((float*)_val)); break;
        case SVAL: node->setSValue(*((string*)_val)); break;
        case NONE: break;
        default: cout << "No Such VALT: " << _valt << endl; break;
    }

    return node;
}