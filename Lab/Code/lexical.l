%option yylineno

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
	printf(";\n");
}
{COMMA}		{
	printf(", ");
}
{ASSIGNOP}	{
	printf("= ");
}
{RELOP}		{
	printf("%s ", yytext);
}
{PLUS}		{
	printf("+ ");
}
{MINUS}		{
	printf("- ");
}
{STAR}		{
	printf("* " );
}
{DIV}		{
	printf("/ ");
}
{AND}		{
	printf("&& ");
}
{OR}		{
	printf("||");
}
{DOT}		{
	printf(".");
}
{NOT}		{
	printf("!");
}
{TYPE}		{
	printf("%s ", yytext);
}
{LP}		{
	printf("(");
}
{RP}		{
	printf(")");
}
{LB}		{
	printf("[");
}
{RB}		{
	printf("]");
}
{LC}		{
	printf("{");
}
{RC}		{
	printf("}");
}
{STRUCT}	{
	printf("struct ");
}
{RETURN}	{
	printf("return ");
}
{IF}		{
	printf("if ");
}
{ELSE}		{
	printf("else\n");
}
{WHILE}		{
	printf("while ");
}
{WS}		{ /* Do nothing. */ }
{INT}		{
	printf("Integer(%s) ", yytext);
}
{FLOAT}		{
	printf("Float(%s) ", yytext);
}
{ID}		{
	printf("ID(%s) ", yytext);
}
{UNCLOSED_MLCOMMENT}	{
	printf("Unterminated Comment!!\n");
}
.			{
	printf("\nUnknown Character: %s\n", yytext);
}

%%