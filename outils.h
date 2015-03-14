/*
 *   Ce fichier fait partie d'un projet de programmation donné en Licence 3 
 *   à l'Université de Bordeaux.
 *
 *   Copyright (C) 2014 Adrien Boussicault
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

/** @file outils.h  */

#ifndef __OUTILS_H__
#define __OUTILS_H__

#include <stdio.h>
#include <stdlib.h>

#define DEBUG(x) do { fprintf(stderr,"DEBUG : %s - ligne : %d, fichier : %s\n", (x), __LINE__, __FILE__ ); } while(0)
#define DEBUGO(x) do { fprintf(stdout,"DEBUG : %s - ligne : %d, fichier : %s\n", (x), __LINE__, __FILE__ ); } while(0)
#define ERREUR(x) do { fprintf(stderr,"ERREUR : %s - ligne : %d, fichier : %s\n", (x), __LINE__, __FILE__ ); exit(EXIT_FAILURE); } while(0)

void* xmalloc( size_t n );
void xfree( void* ptr );

#define TEST(y,x) do { x &= (y); if(!(y)){ fprintf(stdout, "\033[31mEchec du test %s() -- ligne : %d, fichier : %s\033[0m\n", __FUNCTION__, __LINE__, __FILE__ ); } } while(0)
#define TEST1(x) test( x, __LINE__)

#define A_FAIRE do { fprintf(stdout,"A FAIRE ! - ligne : %d, fichier : %s, fonction : %s( )\n",  __LINE__, __FILE__, __FUNCTION__ ); } while(0)
#define A_FAIRE_RETURN(x) do { fprintf(stdout,"A FAIRE ! - ligne : %d, fichier : %s, fonction : %s( )\n", __LINE__, __FILE__, __FUNCTION__ ); return x; } while(0)

int test( int result, int ligne );

#endif

