%{    
/*
 * -*- mode: flex -*-
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
%}

%option outfile="scan.c" header-file="scan.h"

%option warn default 

%option nounput noinput noyywrap

%option reentrant never-interactive 

%option bison-bridge 

%%

[[:lower:]]	{
     yylval->rationnel = Lettre(yytext[0]);
     return TOKEN_LETTRE;
}

[.+*()]   return yytext[0];

[[:blank:]] ;
