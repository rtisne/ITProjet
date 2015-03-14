%{
/*
 * -*- mode: bison -*-
 *   Ce fichier fait partie d'un projet de programmation donné en Licence 3 
 *   à l'Université de Bordeaux
 *
 *   Copyright (C) 2014, 2015 Adrien Boussicault
 *
 *    This Library is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This Library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this Library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "rationnel.h"
#include "parse.h"
#include "scan.h"
 
int yyerror(Rationnel **rat, yyscan_t scanner, const char *msg) {
    fprintf(stderr, "Erreur syntaxique\n");
    return EXIT_FAILURE;
}
 
%}

%code requires {
 
#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif
 
}

%output  "parse.c"
%defines "parse.h"
 
%define api.pure
%lex-param   { yyscan_t scanner }
%parse-param { Rationnel **rationnel }
%parse-param { yyscan_t scanner }
                                                
%union{
  Rationnel *rationnel;
}

%token  <rationnel>TOKEN_LETTRE
%left '+'
%left '.'
%nonassoc '*'

%type <rationnel> expression
                        
%%
input
    : expression { *rationnel = $1; }
    ;

expression:     '(' expression ')' 		{$$ = $2;}
        |       expression '+' expression	{$$ = Union ($1, $3);}
        |       expression  '.' expression 	{$$ = Concat ($1, $3);}
        |       expression '*'			{$$ = Star ($1);}
        |       TOKEN_LETTRE               	{$$ = $1;}
        ;

