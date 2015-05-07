/*
 *   Ce fichier fait partie d'un projet de programmation donné en Licence 3 
 *   à l'Université de Bordeaux
 *
 *   Copyright (C) 2015 Giuliana Bianchi, Adrien Boussicault, Thomas Place, Marc Zeitoun
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

#include <rationnel.h>
#include <ensemble.h>
#include <outils.h>
#include <parse.h>
#include <scan.h>

int test_meme_langage(){
	int result = 1;
    {
       bool test1 = meme_langage("a", "a");
       bool test2 = meme_langage("a", "b");
       bool test3 = meme_langage("a.(b.a)*", "(a.b)*.a");
       
       TEST(
          1
          && test1
          && ! test2
          && test3
          , result
       );

    }

    return result;
}

int main(int argc, char *argv[])
{
   if( ! test_meme_langage() )
    return 1; 
   
   return 0;
}