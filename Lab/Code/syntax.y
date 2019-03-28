%{

#include "ast.h"
#include "lex.yy.c"

extern bool SYNTAX_ERROR;

extern AstNode* AST;
extern int yylex();
extern int yylineno;
extern void yyerror(char* msg);

%}

%union {
    AstNode* type_ast;
}

/* Terminated Tokens */
%token <type_ast> TYPE STRUCT IF ELSE WHILE RETURN INT FLOAT ID SEMI
%token <type_ast> COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV AND OR 
%token <type_ast> DOT NOT LP RP LB RB LC RC
%token <type_ast> error

/* Unterminters */
%type <type_ast> Program ExtDefList ExtDef ExtDecList Specifier StructSpecifier
%type <type_ast> OptTag Tag VarDec FunDec VarList ParamDec CompSt StmtList Stmt
%type <type_ast> DefList Def DecList Dec Exp Args

%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB DOT

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

Program: ExtDefList                                 { vector<AstNode*> vec = {$1}; $$ = AST = AstNode::createNewAstNode(Program, $1->getLineNo(), NONE, NULL, vec); }
    ;
ExtDefList: ExtDef ExtDefList                       { vector<AstNode*> vec = {$1, $2}; $$ = AstNode::createNewAstNode(ExtDefList, $1->getLineNo(), NONE, NULL, vec);  }
    |                                               { $$ = AstNode::createNewAstNode(ANT_EMPTY, -1, NONE, NULL, empty_vec); }
    ;
ExtDef: Specifier ExtDecList SEMI                   { vector<AstNode*> vec = {$1, $2, $3}; $$ = AstNode::createNewAstNode(ExtDef, $1->getLineNo(), NONE, NULL, vec);  }
    | Specifier SEMI                                { vector<AstNode*> vec = {$1, $2}; $$ = AstNode::createNewAstNode(ExtDef, $1->getLineNo(), NONE, NULL, vec);  }
    | Specifier FunDec CompSt                       { vector<AstNode*> vec = {$1, $2, $3}; $$ = AstNode::createNewAstNode(ExtDef, $1->getLineNo(), NONE, NULL, vec);  }
    ;
ExtDecList: VarDec                                  { vector<AstNode*> vec = {$1}; $$ = AstNode::createNewAstNode(ExtDecList, $1->getLineNo(), NONE, NULL, vec);  }
    | VarDec COMMA ExtDecList                       { vector<AstNode*> vec = {$1, $2, $3}; $$ = AstNode::createNewAstNode(ExtDecList, $1->getLineNo(), NONE, NULL, vec);  }
    ;
Specifier: TYPE                                     { vector<AstNode*> vec = {$1}; $$ = AstNode::createNewAstNode(Specifier, $1->getLineNo(), NONE, NULL, vec); }
    | StructSpecifier                               { vector<AstNode*> vec = {$1}; $$ = AstNode::createNewAstNode(Specifier, $1->getLineNo(), NONE, NULL, vec); }
    ;
StructSpecifier: STRUCT OptTag LC DefList RC        { vector<AstNode*> vec = {$1, $2, $3, $4, $5}; $$ = AstNode::createNewAstNode(StructSpecifier, $1->getLineNo(), NONE, NULL, vec);  }
    | STRUCT Tag                                    { vector<AstNode*> vec = {$1, $2}; $$ = AstNode::createNewAstNode(StructSpecifier, $1->getLineNo(), NONE, NULL, vec);  }
    ;
OptTag: ID                                          { vector<AstNode*> vec = {$1}; $$ = AstNode::createNewAstNode(OptTag, $1->getLineNo(), NONE, NULL, vec);  }
    |                                               { $$ = AstNode::createNewAstNode(ANT_EMPTY, -1, NONE, NULL, empty_vec);  }
    ;
Tag: ID                                             { vector<AstNode*> vec = {$1}; $$ = AstNode::createNewAstNode(Tag, $1->getLineNo(), NONE, NULL, vec);  }
    ;
VarDec: ID                                          { vector<AstNode*> vec = {$1}; $$ = AstNode::createNewAstNode(VarDec, $1->getLineNo(), NONE, NULL, vec);  }
    | VarDec LB INT RB                              { vector<AstNode*> vec = {$1, $2, $3, $4}; $$ = AstNode::createNewAstNode(VarDec, $1->getLineNo(), NONE, NULL, vec);  }
    ;
FunDec: ID LP VarList RP                            { vector<AstNode*> vec = {$1, $2, $3, $4}; $$ = AstNode::createNewAstNode(FunDec, $1->getLineNo(), NONE, NULL, vec);  }
    | ID LP RP                                      { vector<AstNode*> vec = {$1, $2, $3}; $$ = AstNode::createNewAstNode(FunDec, $1->getLineNo(), NONE, NULL, vec);  }
    ;
VarList: ParamDec COMMA VarList                     { vector<AstNode*> vec = {$1, $2, $3}; $$ = AstNode::createNewAstNode(VarList, $1->getLineNo(), NONE, NULL, vec);  }
    | ParamDec                                      { vector<AstNode*> vec = {$1}; $$ = AstNode::createNewAstNode(VarList, $1->getLineNo(), NONE, NULL, vec);  }
    ;
ParamDec: Specifier VarDec                          { vector<AstNode*> vec = {$1, $2}; $$ = AstNode::createNewAstNode(ParamDec, $1->getLineNo(), NONE, NULL, vec);   }
    ;
CompSt: LC DefList StmtList RC                      { vector<AstNode*> vec = {$1, $2, $3, $4}; $$ = AstNode::createNewAstNode(CompSt, $1->getLineNo(), NONE, NULL, vec);  }
    ;
StmtList: Stmt StmtList                             { vector<AstNode*> vec = {$1, $2}; $$ = AstNode::createNewAstNode(StmtList, $1->getLineNo(), NONE, NULL, vec);  }
    |                                               { $$ = AstNode::createNewAstNode(ANT_EMPTY, -1, NONE, NULL, empty_vec);  }
    ;
Stmt: Exp SEMI                                      { vector<AstNode*> vec = {$1, $2}; $$ = AstNode::createNewAstNode(Stmt, $1->getLineNo(), NONE, NULL, vec);  }
    | CompSt                                        { vector<AstNode*> vec = {$1}; $$ = AstNode::createNewAstNode(Stmt, $1->getLineNo(), NONE, NULL, vec);  }
    | RETURN Exp SEMI                               { vector<AstNode*> vec = {$1, $2, $3}; $$ = AstNode::createNewAstNode(Stmt, $1->getLineNo(), NONE, NULL, vec); }
    | IF LP Exp RP Stmt                             { vector<AstNode*> vec = {$1, $2, $3, $4, $5}; $$ = AstNode::createNewAstNode(Stmt, $1->getLineNo(), NONE, NULL, vec);  }
    | IF LP Exp RP Stmt ELSE Stmt                   { vector<AstNode*> vec = {$1, $2, $3, $4, $5, $6, $7}; $$ = AstNode::createNewAstNode(Stmt, $1->getLineNo(), NONE, NULL, vec);  }
    | WHILE LP Exp RP Stmt                          { vector<AstNode*> vec = {$1, $2, $3, $4, $5}; $$ = AstNode::createNewAstNode(Stmt, $1->getLineNo(), NONE, NULL, vec); }
    ;
DefList: Def DefList                                { vector<AstNode*> vec = {$1, $2}; $$ = AstNode::createNewAstNode(DefList, $1->getLineNo(), NONE, NULL, vec);  }
    |                                               { $$ = AstNode::createNewAstNode(ANT_EMPTY, -1, NONE, NULL, empty_vec);  }
    ;
Def: Specifier DecList SEMI                         { vector<AstNode*> vec  = {$1, $2, $3}; $$ = AstNode::createNewAstNode(Def, $1->getLineNo(), NONE, NULL, vec); }
    ;
DecList: Dec                                        { vector<AstNode*> vec = {$1}; $$ = AstNode::createNewAstNode(DecList, $1->getLineNo(), NONE, NULL, vec);  }
    | Dec COMMA DecList                             { vector<AstNode*> vec = {$1, $2, $3}; $$ = AstNode::createNewAstNode(DecList, $1->getLineNo(), NONE, NULL, vec);  }
    ;
Dec: VarDec                                         { vector<AstNode*> vec = {$1}; $$ = AstNode::createNewAstNode(Dec, $1->getLineNo(), NONE, NULL, vec);  }
    | VarDec ASSIGNOP Exp                           { vector<AstNode*> vec = {$1, $2, $3}; $$ = AstNode::createNewAstNode(Dec, $1->getLineNo(), NONE, NULL, vec);  }
    ;
Exp: Exp ASSIGNOP Exp                               { vector<AstNode*> vec = {$1, $2, $3}; $$ = AstNode::createNewAstNode(Exp, $1->getLineNo(), NONE, NULL, vec);  }
    | Exp AND Exp                                   { vector<AstNode*> vec = {$1, $2, $3}; $$ = AstNode::createNewAstNode(Exp, $1->getLineNo(), NONE, NULL, vec);  }
    | Exp OR Exp                                    { vector<AstNode*> vec = {$1, $2, $3}; $$ = AstNode::createNewAstNode(Exp, $1->getLineNo(), NONE, NULL, vec);  }
    | Exp RELOP Exp                                 { vector<AstNode*> vec = {$1, $2, $3}; $$ = AstNode::createNewAstNode(Exp, $1->getLineNo(), NONE, NULL, vec);  }
    | Exp PLUS Exp                                  { vector<AstNode*> vec = {$1, $2, $3}; $$ = AstNode::createNewAstNode(Exp, $1->getLineNo(), NONE, NULL, vec);  }
    | Exp MINUS Exp                                 { vector<AstNode*> vec = {$1, $2, $3}; $$ = AstNode::createNewAstNode(Exp, $1->getLineNo(), NONE, NULL, vec);  }
    | Exp STAR Exp                                  { vector<AstNode*> vec = {$1, $2, $3}; $$ = AstNode::createNewAstNode(Exp, $1->getLineNo(), NONE, NULL, vec);  }
    | Exp DIV Exp                                   { vector<AstNode*> vec = {$1, $2, $3}; $$ = AstNode::createNewAstNode(Exp, $1->getLineNo(), NONE, NULL, vec);  }
    | LP Exp RP                                     { vector<AstNode*> vec = {$1, $2, $3}; $$ = AstNode::createNewAstNode(Exp, $1->getLineNo(), NONE, NULL, vec);  }
    | MINUS Exp                                     { vector<AstNode*> vec = {$1, $2}; $$ = AstNode::createNewAstNode(Exp, $1->getLineNo(), NONE, NULL, vec);  }
    | NOT Exp                                       { vector<AstNode*> vec = {$1, $2}; $$ = AstNode::createNewAstNode(Exp, $1->getLineNo(), NONE, NULL, vec);  }
    | ID LP Args RP                                 { vector<AstNode*> vec = {$1, $2, $3, $4}; $$ = AstNode::createNewAstNode(Exp, $1->getLineNo(), NONE, NULL, vec);  }
    | ID LP RP                                      { vector<AstNode*> vec = {$1, $2, $3}; $$ = AstNode::createNewAstNode(Exp, $1->getLineNo(), NONE, NULL, vec);  }
    | Exp LB Exp RB                                 { vector<AstNode*> vec = {$1, $2, $3, $4}; $$ = AstNode::createNewAstNode(Exp, $1->getLineNo(), NONE, NULL, vec);  }
    | Exp DOT ID                                    { vector<AstNode*> vec = {$1, $2, $3}; $$ = AstNode::createNewAstNode(Exp, $1->getLineNo(), NONE, NULL, vec);  }
    | ID                                            { vector<AstNode*> vec = {$1}; $$ = AstNode::createNewAstNode(Exp, $1->getLineNo(), NONE, NULL, vec);  }
    | INT                                           { vector<AstNode*> vec = {$1}; $$ = AstNode::createNewAstNode(Exp, $1->getLineNo(), NONE, NULL, vec);  }
    | FLOAT                                         { vector<AstNode*> vec = {$1}; $$ = AstNode::createNewAstNode(Exp, $1->getLineNo(), NONE, NULL, vec);  }
    ;
Args: Exp COMMA Args                                { vector<AstNode*> vec = {$1, $2, $3}; $$ = AstNode::createNewAstNode(Args, $1->getLineNo(), NONE, NULL, vec); }
    | Exp                                           { vector<AstNode*> vec = {$1}; $$ = AstNode::createNewAstNode(Args, $1->getLineNo(), NONE, NULL, vec); }
    ;

%%

void yyerror(char* msg){
    SYNTAX_ERROR = true;
    cout << "Error!" << msg << endl;
}









