#include "ast.h"
#include <assert.h>

extern bool SYNTAX_ERROR;
AstNode* AST = NULL;
SymbolTable symboltable;

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

void AstNode::parseProgram(){
    assert(ant == Program);

    AstNode* extDefList = firstChild(), *extDef = extDefList->firstChild();
    while(extDefList->ant != ANT_EMPTY){
        extDef->parseExtDef();
        extDefList = extDef->nextSibling();
        extDef = extDefList->firstChild();
    }
    symboltable.checkFuncDefinition();
}

void AstNode::parseExtDef(){

    assert(ant == ExtDef);
    AstNode* specifier = firstChild();
    Type func_type = specifier->parseSpecifier();
    //cout<<func_type.getTypeName()<<endl;
    if(attr==ATTR_FUNCDEC_SPEC){
        AstNode* funDec = specifier->nextSibling();
        Function func(funDec, func_type, false);
        symboltable.addFuncDeclaration(func);
    }
    else if(attr==ATTR_FUNCDEF_SPEC){
        AstNode* funDec = specifier->nextSibling();
        AstNode* compSt = funDec->nextSibling();

        Function func(funDec, func_type, true);
        symboltable.addFuncDefinition(func);
        compSt->parseCompSt(func.getType());
    }
    else if(attr==ATTR_DEC_SPEC){
        //cout<<"here"<<endl;
        AstNode* extDecList = specifier->nextSibling();
        extDecList->parseExtDecList(func_type);
    }
    else if(attr == ATTR_STRUCT_SPEC){
        //cout<<"here"<<endl;
    }
}

Type AstNode::parseSpecifier(){
    assert(ant == Specifier);
    AstNode* child = firstChild();
    if(child->ant==StructSpecifier && child->getAttr() == ATTR_STRUCT_DEC){
        AstNode* Tag = child->firstChild()->nextSibling();
        string name = Tag->firstChild()->sval;
        //cout<<name<<endl;
        if(!symboltable.hasStructType(name)){
            string msg = "Undefined structure ";
            msg.append("\"").append(name).append("\".");
            reportSemanticError(17, msg, lineno);
            return Type();
        }
        else{
            return Type(*symboltable.getStructType(name));
            //cout<<t.getTypeName()<<endl;
        }
    }
    else{
        Type type = Type(this);
        //cout<<type.getTypeName()<<endl;
        if(!type.isBasic() && !(type.getStructName()=="unknown")){
            //cout<<type.getTypeName()<<endl;
            symboltable.addStructType(type);
        }
        return type;
    }
}

void AstNode::parseExtDecList(const Type& type){
    assert(ant == ExtDecList);
    //cout<<type.getTypeName()<<endl;
    AstNode* extDecList = this, *varDec= firstChild();
    while(varDec->nextSibling() != NULL){
        symboltable.addSymbol(Symbol(varDec, type));
        extDecList = varDec->nextSibling()->nextSibling();
        varDec = extDecList->firstChild();
    }
}

vector<Symbol> AstNode::parseDefList(bool notInStruct){
    assert(ant == DefList || ant == ANT_EMPTY);
    vector<Symbol> result;
    AstNode* defList = this, *def = firstChild();
    while(defList->ant != ANT_EMPTY){
        AstNode* specifier = def->firstChild();
        //cout<<specifier->sval<<endl;
        AstNode* decList = specifier->nextSibling();

        Type type = specifier->parseSpecifier();
        //cout<<type.getTypeName()<<endl;
        decList->parseDecList(result, type, notInStruct);
        defList = def->nextSibling();
        def = defList->firstChild();
    }
    return result;
}

void AstNode::parseDecList(vector<Symbol> &result, const Type& type, bool notInStruct){
    assert(ant == DecList);
    
    AstNode* decList = this, *dec = firstChild();
    while(true){
        AstNode* varDec = dec->firstChild();
        Symbol symbol(varDec, type);

        if(notInStruct){
            result.emplace_back(symbol);
            if(dec->getAttr()==ATTR_ASSIGN_DEC){
                AstNode* exp = varDec->nextSibling()->nextSibling();
                Type righttype = exp->parseExp();
                if(!(righttype == type)){
                    string msg = "Type mismatched for assignment";
                    reportSemanticError(5, msg, varDec->lineno);
                }
            }
        }
        else{
            bool hasSymbol = false;
            for(int i=0; i<result.size(); i++){
                if(result[i].getName() == symbol.getName()){
                    hasSymbol = true;
                    break;
                }
            }
            if (!hasSymbol){
                result.emplace_back(symbol);
            }
            else{
                string msg = string("Redefined field").append("\"").append(symbol.getName()).append("\".");
                reportSemanticError(15, msg, symbol.getlineno());
            }

            if(dec->attr == ATTR_ASSIGN_DEC){
                string msg = string("Initialize field ").append("\"").append(symbol.getName()).append("\" in definition.");
                reportSemanticError(15, msg, symbol.getlineno());
            }
        }

        if(dec->nextSibling() != NULL){
            decList = dec->nextSibling()->nextSibling();
            dec = decList->firstChild();
        }
        else{
            break;
        }
    }
}

vector<Symbol> AstNode::parseVarList(){
    assert(ant == VarList);
    vector<Symbol> result;
    AstNode* parameter = firstChild();
    while(true){
        result.emplace_back(Symbol(parameter));
        if(parameter->nextSibling() != NULL){
            parameter = parameter->nextSibling()->firstChild();
        }
        else{
            break;
        }
    }
    for(int i=0; i<result.size(); i++){
        symboltable.addSymbol(result[i]);
    }
    return result;
}

void AstNode::parseCompSt(const Type& type){
    assert(ant == CompSt);
    AstNode* defList = firstChild()->nextSibling();
    AstNode* stmtList = defList->nextSibling();
    AstNode* stmt = stmtList->firstChild();

    vector<Symbol> symbols = defList->parseDefList(true);
    for(int i=0; i<symbols.size(); i++){
        symboltable.addSymbol(symbols[i]);
    }
    while(stmtList->getAnt() != ANT_EMPTY){
        stmt->parseStmt(type);
        stmtList = stmt->nextSibling();
        stmt = stmt->nextSibling()->firstChild();
    }
}

void AstNode::parseStmt(const Type& type){
    assert(ant == Stmt);
    if(attr == ATTR_EXP){
        firstChild()->parseExp();
    }
    else if(attr == ATTR_COMPST){
        firstChild()->parseCompSt(type);
    }
    else if(attr == ATTR_RETURN){
        AstNode* exp = firstChild()->nextSibling();
        Type _type = exp->parseExp();
        if(!(_type == type)){

            //cout<< _type.getTypeName() <<" "<<type.getTypeName() <<endl;
            string msg = "Type mismatched for return.";
            reportSemanticError(8, msg, exp->lineno);
        }
    }
    else if(attr == ATTR_IF || attr == ATTR_WHILE){
        AstNode* exp = firstChild()->nextSibling()->nextSibling();
        AstNode* stmt = exp->nextSibling()->nextSibling();
        exp->parseExp();
        stmt->parseStmt(type);
    }
    else if(attr == ATTR_IFELSE){
        AstNode* exp = firstChild()->nextSibling()->nextSibling();
        AstNode* stmt1 = exp->nextSibling()->nextSibling(), *stmt2 = stmt1->nextSibling()->nextSibling();
        exp->parseExp();
        stmt1->parseStmt(type);
        stmt2->parseStmt(type);
    }
}

Type AstNode::parseExp(){
    assert(ant == Exp);
    if(attr == ATTR_INT){
        right = false;
        return Type(true);
    }
    else if(attr == ATTR_FLOAT){
        right = false;
        return Type(false);
    }
    else if(attr == ATTR_ID){
        

        right = true;
        AstNode* idnode = firstChild();

        assert(idnode->getAnt() == ANT_ID);

        

        if(symboltable.hasSymbolName(idnode->getSValue())){
            
            return symboltable.getSymbol(idnode->getSValue())->getType();
        }
        else{
            string msg = string("Undefined variable ").append("\"").append(idnode->getSValue()).append("\"");
            reportSemanticError(1, msg, idnode->getAnt());
            return Type();
        }
    }
    else if(attr == ATTR_FUNC_ARGS_EXP || attr == ATTR_FUNC_EMPTY_EXP){
        right = false;
        AstNode* idnode = firstChild();
        assert(idnode->getAnt() == ANT_ID);
        

        if(symboltable.hasFuncDeclaration(idnode->getSValue()) || symboltable.hasFuncDefinition(idnode->getSValue())){
            //check arguments' parameter types.

            //cout << idnode->getSValue() << endl;

            Function* func = NULL;
            if(symboltable.hasFuncDeclaration(idnode->getSValue())){
                func = symboltable.getFuncDeclaration(idnode->getSValue());
                //cout << idnode->getSValue() << endl;
            }
            else{
                func = symboltable.getFuncDefinition(idnode->getSValue());
                //cout << idnode->getSValue() << endl;
            }

            //cout << func->getArgsParamString() << endl;

            vector<Type> types;
            types = func->getArgsTypeParam();

            //cout << types[0].getTypeName() << endl;

            vector<Type> types1;
            if(attr == ATTR_FUNC_ARGS_EXP){
                types1 = idnode->nextSibling()->nextSibling()->parseArgs();
                //cout << types1[0].getTypeName() << endl;
            }
            bool matched = true;
            if(types.size() != types1.size())
                matched = false;
            else{
                //cout << "Here" << endl;
                for(int i=0; i<types.size(); i++){
                    if(!(types[i] == types1[i])){
                        matched = false;
                        break;
                    }
                }
            }


            if(!matched){
                string msg = string("Function ").append("\"").append(func->getName()).append(func->getArgsParamString()).append("\"");
                msg.append(" is not applicable for arguments.");
                reportSemanticError(9, msg, lineno);
                return Type();
            }
            return func->getType();
        }
        else{
            if(!symboltable.hasSymbolName(idnode->getSValue())){
                string msg = string("Undefined function ").append("\"").append(idnode->getSValue()).append("\"");
                reportSemanticError(2, msg, idnode->getLineNo());
            }
            else{
                string msg = string("\"").append(idnode->getSValue()).append("\" is not a function.");
                reportSemanticError(11, msg, idnode->getLineNo());
            }
            return Type();
        }
    }
    else if(attr == ATTR_STRUCT){
        right = true;
        AstNode* exp = firstChild(), *idnode = exp->nextSibling()->nextSibling();
        Type type = exp->parseExp();
        if(type.isErrorType()){
            return Type();
        }
        if(!type.isStruct()){
            //cout<<type.getLineno()<<endl;
            string msg = string("Illegal use of \".\"");
            reportSemanticError(13, msg, exp->getLineNo());
            return Type();
        }
        //followings are struct. to find fields.
        Symbol* fieldSymbol = type.hasStructField(idnode->getSValue());
        if(fieldSymbol == NULL){
            string msg = string("Non-existent field ").append("\"").append(idnode->getSValue()).append("\".");
            reportSemanticError(14, msg, exp->getLineNo());
            return Type();
        }
        Type fieldType = fieldSymbol->getType();
        return fieldType;
    }
    else if(attr == ATTR_ARRAY){
        right = true;
        AstNode* exp = firstChild(), *exp1 = exp->nextSibling()->nextSibling();
        Type _type = exp->parseExp(), type1 = exp1->parseExp();
        if(_type.isErrorType() || type1.isErrorType()){
            return Type();
        }
        if(!_type.isArray()){
            string msg = string("\"").append(exp->getSValue()).append("\" is not a array.");
            reportSemanticError(10, msg, exp->getLineNo());
            return Type();
        }
        if(!type1.isInt()){
            string msg = string("\"").append(exp1->getSValue()).append("\" is not a integer.");
            reportSemanticError(12, msg, exp1->getLineNo());
            return Type();
        }
        return _type.getArrayElem();
    }
    else if(attr == ATTR_NEST){
        AstNode* exp = firstChild()->nextSibling();
        right = exp->right;
        Type type = exp->parseExp();
        if(type.isErrorType())
            return Type();
        else{
            return type;
        }
    }
    else if(attr == ATTR_NEG || attr == ATTR_NOT){
        right = false;
        AstNode* exp = firstChild()->nextSibling();
        Type type = exp->parseExp();
        if(type.isErrorType()){
            return Type();
        }
        if((attr == ATTR_NOT && !type.isInt()) || (attr == ATTR_NEG && !type.isBasic())){
            string msg = "Type mismatched for operands.";
            reportSemanticError(7, msg, exp->getLineNo());
        }
        return type;
    }
    else if(attr == ATTR_ASSIGN){
        right = false;
        AstNode* exp1 = firstChild(), *exp2 = exp1->nextSibling()->nextSibling();
        Type type1 = exp1->parseExp();
        Type type2 = exp2->parseExp();
        if(!exp1->right){
            string msg = string("The left-hand side of an assignment must be a variable.");
            reportSemanticError(6, msg, exp1->getLineNo());
            return Type();
        }
        if(type1.isErrorType() || type2.isErrorType()){
            return Type();
        }
        if(!(type1 == type2)){
            string msg = "Type mismatched for assignment.";
            reportSemanticError(5, msg, exp1->getLineNo());
            return Type();
        }
        return type1;
    }
    else if(attr == ATTR_AND || attr == ATTR_OR || attr == ATTR_REL || attr == ATTR_PLUS || attr == ATTR_MINUS || attr == ATTR_STAR || attr == ATTR_DIV){
        right = false;
        AstNode *exp1 = firstChild(), *exp2 = exp1->nextSibling()->nextSibling();
        Type type1 = exp1->parseExp();
        Type type2 = exp2->parseExp();
        if(type1.isErrorType() || type2.isErrorType()){
            return Type();
        }
        bool correct = false;
        if(attr == ATTR_AND || attr == ATTR_OR || attr == ATTR_REL){
            correct = type1.isInt() && type2.isInt();
        }
        else{
            correct = (type1.isInt() && type2.isInt()) || (type1.isFloat() && type2.isFloat());
        }
        if(correct){
            return type1;
        }
        else{
            string msg = string("Type mismatched for operands.");
            reportSemanticError(7, msg, exp1->getLineNo());
            return Type();
        }
    }
}

vector<Type> AstNode::parseArgs(){
    assert(ant == Args);
    vector<Type> result;
    AstNode* args = this, *exp = firstChild();
    while(true){
        Type type = exp->parseExp();
        result.emplace_back(type);

        if(exp->nextSibling() == NULL){
            break;
        }
        args = exp->nextSibling()->nextSibling();
        exp = args->firstChild();
    }
    return result;
}