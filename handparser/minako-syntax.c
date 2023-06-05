#include <stdio.h>
#include <stdlib.h>
#include "minako.h"

//basis funktionen, zum Auflösen der Zeichen
void eat(void);
int isToken(int);
void isTokenAndEat(int);

//grammatikregeln bzw. Funktionen für die Nichtterminale
void program(void);
void functiondefinition(void);
void functioncall(void);
void statementlist(void);
void block(void);
void statement(void);
void ifstatement(void);
void returnstatement(void);
void tok_printf(void);
void type(void);
void statassignment(void);
void assignment(void);
void expr(void);
void simpexpr(void);
void term(void);
void factor(void);



int currentToken;
int nextToken;

int main(int argc, char **argv) {

    if (argc != 2) {
        yyin = stdin;
    } 
    else {
        yyin = fopen(argv[1], "r");
        if (yyin == 0) {
            fprintf(
                    stderr,
                    "Fehler: Konnte Datei %s nicht zum lesen oeffnen. (eventuell wurden zu viele parameter angegeben. Erwartete Parameteranzahl: 1\n",
                    argv[1]
            );
            exit(-1);
        }
    }
    currentToken = yylex();
    nextToken = yylex();

    program();
    return 0;

}

void eat(void) {
    currentToken = nextToken;
    nextToken = yylex();
}

int isToken(int token) {
    return (currentToken == token);
}

void isTokenAndEat(int token) {
    if (isToken(token)) {
        eat();
    }
    else {
        fprintf(stderr, "ERROR: syntax error in line %d. ", yylineno);
        exit(-1);
    }
}


void program(void) {
//"start""symbol"
    while (isToken(KW_BOOLEAN) || isToken(KW_FLOAT) || isToken(KW_INT) || isToken(KW_VOID)) {
        functiondefinition();
    }
    isTokenAndEat(EOF);

}

void functiondefinition(void) {
    type(); isTokenAndEat(ID); isTokenAndEat('('); isTokenAndEat(')');
    isTokenAndEat('{'); statementlist(); isTokenAndEat('}');
}

void functioncall(void) {
    isTokenAndEat(ID); isTokenAndEat('('); isTokenAndEat(')');
}

void statementlist(void) {
    while (isToken('{') || isToken(KW_IF) || isToken(KW_RETURN) || isToken(KW_PRINTF) || isToken(ID) ) {
        block();
    }
}

void block(void) {
    if (isToken('{')) {
        eat();
        statementlist();
        isTokenAndEat('}');
    } else {
        statement();
    }
}

void statement(void) {
    switch (currentToken) {
        case KW_IF:
            ifstatement(); break;
        case KW_RETURN:
            returnstatement(); isTokenAndEat(';'); break;
        case KW_PRINTF:
            tok_printf(); isTokenAndEat(';'); break;
        case ID:
            if (nextToken == '=') {
                statassignment(); isTokenAndEat(';');
            }
            else if (nextToken == '(') {
                functioncall(); isTokenAndEat(';');
            }
            else {
                fprintf(stderr,"ERROR: cannot find '=' or '(' after ID\n");
            }
            break;
        default:
            fprintf(stderr,"ERROR: no beginning of statement\n");
    }
}

void ifstatement(void) {
    isTokenAndEat(KW_IF);
    isTokenAndEat('('); assignment(); isTokenAndEat(')');
    block();
}

void returnstatement(void) {
    isTokenAndEat(KW_RETURN);
    if (isToken(ID) || isToken('-') || 
        isToken(CONST_INT) || isToken(CONST_FLOAT) || 
        isToken(CONST_BOOLEAN)|| isToken('(')) 
    {
        assignment();
    }
    
}

void tok_printf(void) {
    isTokenAndEat(KW_PRINTF);
    isTokenAndEat('('); assignment(); isTokenAndEat(')');
}

void type(void) {
    switch(currentToken) {
        case KW_BOOLEAN:
        case KW_FLOAT:
        case KW_INT:
        case KW_VOID:
            eat(); break;
        default:
            fprintf(stderr, "ERROR: during parsing in line %d.\n",yylineno);
            exit(-1);
    }
}

void statassignment(void) {
    isTokenAndEat(ID);
    isTokenAndEat('=');
    assignment();
}

void assignment(void) {
    if (isToken(ID) && nextToken == '=') 
    {
        statassignment();
    } 
    else if (isToken (ID) || isToken('-') || 
            isToken(CONST_INT) || isToken(CONST_FLOAT) || 
            isToken(CONST_BOOLEAN)|| isToken('(')) 
    {
        expr();
    } 
    else {
        fprintf(stderr,"ERROR: expected assignment or expression in line %d.\n",yylineno);
        exit(-1);
    }
}

void expr(void) {
    simpexpr();

    if (isToken(EQ) || isToken(NEQ) || 
        isToken(LEQ) || isToken(GEQ)  || 
        isToken(LSS) || isToken(GRT)) 
    {
        eat();
        simpexpr();
    } 
   
}

void simpexpr(void) {
    if (isToken('-')) { //optional
        eat();
    }
    term();
    while (isToken('+') || isToken('-') || isToken(OR)) {
        eat();
        term();
    }
}

void term(void) {
    factor();
    while (isToken('*') || isToken('/') || isToken(AND)) {
        eat();
        factor();
    }
}

void factor(void) {
    switch (currentToken) {
        case CONST_INT:
        case CONST_FLOAT:
        case CONST_BOOLEAN:
            eat(); break;
        case ID:
            if (nextToken == '(')
                functioncall();
            else
                eat();
            break;
        case '(':
            eat(); assignment(); isTokenAndEat(')');
    }
}
