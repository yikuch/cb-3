%option noyywrap
%option nounput
%option yylineno
%option noinput
%option never-interactive

WHITESPACE [[:space:]]
INTEGER    [[:digit:]]+
FLOAT      {INTEGER}"."{INTEGER}|"."{INTEGER}

%x COMMENT

%{
	#include <stdlib.h>
	#include "minako.h"
	yystype yylval;
%}

%%

{WHITESPACE}+ { /* ignore whitespaces */ }
"//".*\n      {                 /* C++ Comment */ }
"/*"          { BEGIN(COMMENT); /* C   Comment */ }
<COMMENT>"*/" { BEGIN(INITIAL); }
<COMMENT>"*"[^/]
<COMMENT>[^*]+

"&&"          return AND;
"||"          return OR;
"=="          return EQ;
"!="          return NEQ;
"<="          return LEQ;
">="          return GEQ;
"<"           return LSS;
">"           return GRT;

"bool"        return KW_BOOLEAN;
"do"          return KW_DO;
"else"        return KW_ELSE;
"float"       return KW_FLOAT;
"for"         return KW_FOR;
"if"          return KW_IF;
"int"         return KW_INT;
"printf"      return KW_PRINTF;
"return"      return KW_RETURN;
"void"        return KW_VOID;
"while"       return KW_WHILE;

{FLOAT}([eE][\-+]?{INTEGER})? |
{INTEGER}([eE][\-+]?{INTEGER}) {
	yylval.floatValue = strtod(yytext, NULL);
	return CONST_FLOAT;
}
{INTEGER} {
	yylval.intValue = strtol(yytext, NULL, 10);
	return CONST_INT;
}
"true"      { yylval.intValue = 1; return CONST_BOOLEAN; }
"false"     { yylval.intValue = 0; return CONST_BOOLEAN; }
[[:alpha:]][[:alnum:]_]* {
	yylval.string = malloc(yyleng + 1);
	strcpy(yylval.string, yytext);
	return ID;
}

.             return yytext[0];
<<EOF>>       return EOF;

%%
