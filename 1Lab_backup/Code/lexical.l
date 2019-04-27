%option yylineno

%{
#include "syntax.tab.h"
#include "headers.h"

extern bool SYNTAX_ERROR;
static vector<AstNode*> empty_vec;

static void reportLexicalError(string anno){
	SYNTAX_ERROR = true;
	printf("\nError type A at Line %d: %s.\n", yylineno, anno.c_str());
}

%}

SLCOMMENT	\/\/[^\n]*[\n]
LCOMMENT	\/\*
RCOMMENT	\*\/
MLCOMMENT	{LCOMMENT}[^*]*\*+([^*/][^*]*\*+)*\/
UNCLOSED_MLCOMMENT1	{LCOMMENT}[^*]*
UNCLOSED_MLCOMMENT2	{LCOMMENT}[^*]*\*+([^*/][^*]*\*+)*([^*/][^*]*)
UNCLOSED_MLCOMMENT	{UNCLOSED_MLCOMMENT1}|{UNCLOSED_MLCOMMENT2}

SEMI		;
COMMA		,
ASSIGNOP	=
RELOP		>|<|>=|<=|==|!=
PLUS		\+
MINUS		-
STAR		\*
DIV			\/
AND			&&
OR			\|\|
DOT			\.
NOT			!
TYPE		int|float
LP			\(
RP			\)
LB			\[
RB			\]
LC			\{
RC			\}
STRUCT		struct
RETURN		return
IF			if
ELSE		else
WHILE		while

DELIM		[ \n\t\f\r\v]
WS			{DELIM}+

DIGIT		[0-9]
DIGITS		[0-9]+
INT 		0|([1-9]{DIGIT}*)
FLOAT 		{DIGIT}+{DOT}{DIGITS}


LETTER		[_a-zA-Z]
ID			{LETTER}({LETTER}|{DIGIT})*



%%

{SLCOMMENT}	{
	/* Do nothing with comment. */
}
{MLCOMMENT}	{
	/* Do nothing with comment. */
}
{SEMI}		{ 
    yylval.type_ast = AstNode::createNewAstNode(ANT_SEMI, yylineno, NONE, NULL, empty_vec);
	return SEMI;
}
{COMMA}		{
    yylval.type_ast = AstNode::createNewAstNode(ANT_COMMA, yylineno, NONE, NULL, empty_vec);
	return COMMA;
}
{ASSIGNOP}	{
    yylval.type_ast = AstNode::createNewAstNode(ANT_ASSIGNOP, yylineno, NONE, NULL, empty_vec);
	return ASSIGNOP;
}
{RELOP}		{
	yylval.type_ast = AstNode::createNewAstNode(ANT_RELOP, yylineno, NONE, NULL, empty_vec);
	return RELOP;
}
{PLUS}		{
	yylval.type_ast = AstNode::createNewAstNode(ANT_PLUS, yylineno, NONE, NULL, empty_vec);
	return PLUS;
}
{MINUS}		{
	yylval.type_ast = AstNode::createNewAstNode(ANT_MINUS, yylineno, NONE, NULL, empty_vec);
	return MINUS;
}
{STAR}		{
	yylval.type_ast = AstNode::createNewAstNode(ANT_STAR, yylineno, NONE, NULL, empty_vec);
	return STAR;
}
{DIV}		{
    yylval.type_ast = AstNode::createNewAstNode(ANT_DIV, yylineno, NONE, NULL, empty_vec);
	return DIV;
}
{AND}		{
    yylval.type_ast = AstNode::createNewAstNode(ANT_AND, yylineno, NONE, NULL, empty_vec);
	return AND;
}
{OR}		{
    yylval.type_ast = AstNode::createNewAstNode(ANT_OR, yylineno, NONE, NULL, empty_vec);
	return OR;
}
{DOT}		{
    yylval.type_ast = AstNode::createNewAstNode(ANT_DOT, yylineno, NONE, NULL, empty_vec);
	return DOT;
}
{NOT}		{
    yylval.type_ast = AstNode::createNewAstNode(ANT_NOT, yylineno, NONE, NULL, empty_vec);
	return NOT;
}
{TYPE}		{
    string sval = string(yytext);
	yylval.type_ast = AstNode::createNewAstNode(ANT_TYPE, yylineno, SVAL, &sval, empty_vec);
	return TYPE;
}
{LP}		{
	yylval.type_ast = AstNode::createNewAstNode(ANT_LP, yylineno, NONE, NULL, empty_vec);
	return LP;
}
{RP}		{
	yylval.type_ast = AstNode::createNewAstNode(ANT_RP, yylineno, NONE, NULL, empty_vec);
	return RP;
}
{LB}		{
	yylval.type_ast = AstNode::createNewAstNode(ANT_LB, yylineno, NONE, NULL, empty_vec);
	return LB;
}
{RB}		{
	yylval.type_ast = AstNode::createNewAstNode(ANT_RB, yylineno, NONE, NULL, empty_vec);
	return RB;
}
{LC}		{
    yylval.type_ast = AstNode::createNewAstNode(ANT_LC, yylineno, NONE, NULL, empty_vec);
	return LC;
}
{RC}		{
    yylval.type_ast = AstNode::createNewAstNode(ANT_RC, yylineno, NONE, NULL, empty_vec);
	return RC;
}
{STRUCT}	{
	yylval.type_ast = AstNode::createNewAstNode(ANT_STRUCT, yylineno, NONE, NULL, empty_vec);
	return STRUCT;
}
{RETURN}	{
	yylval.type_ast = AstNode::createNewAstNode(ANT_RETURN, yylineno, NONE, NULL, empty_vec);
	return RETURN;
}
{IF}		{
	yylval.type_ast = AstNode::createNewAstNode(ANT_IF, yylineno, NONE, NULL, empty_vec);
	return IF;
}
{ELSE}		{
	yylval.type_ast = AstNode::createNewAstNode(ANT_ELSE, yylineno, NONE, NULL, empty_vec);
	return ELSE;
}
{WHILE}		{
	yylval.type_ast = AstNode::createNewAstNode(ANT_WHILE, yylineno, NONE, NULL, empty_vec);
	return WHILE;
}
{WS}		{ /* Do nothing. */ }
{INT}		{
	int ival = 0;
    sscanf(yytext, "%d", &ival);
    yylval.type_ast = AstNode::createNewAstNode(ANT_INT, yylineno, IVAL, &ival, empty_vec);
	return INT;
}
{FLOAT}		{
    float fval = 0;
    sscanf(yytext, "%f", &fval);
    yylval.type_ast = AstNode::createNewAstNode(ANT_FLOAT, yylineno, FVAL, &fval, empty_vec);
	return FLOAT;
}
{ID}		{
	string sval = string(yytext);
    yylval.type_ast = AstNode::createNewAstNode(ANT_ID, yylineno, SVAL, &sval, empty_vec);
	return ID;
}
{UNCLOSED_MLCOMMENT}	{
	reportLexicalError("Unterminated Comment");
}
.			{
	string str = "Mysterious character \'";
	str.append(yytext);
	str.append("\'");
	reportLexicalError(str);
}

%%

