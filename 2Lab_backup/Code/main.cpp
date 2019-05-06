#include "ast.h"

extern AstNode* AST;
extern FILE* yyin;
extern int yylval;

extern int yylex();
extern int yyparse();
extern int yyrestart(FILE *);

bool SYNTAX_ERROR = false;

int main(int argc, char** argv){
	if(argc <= 1)
		return 1;
	FILE* f = fopen(argv[1], "r");
	if(!f){
		perror(argv[1]);
		return 1;
	}

	yyrestart(f);
	yyparse();

	if(!SYNTAX_ERROR)
		AST->parseProgram();

	return 0;
}