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
#include <automate.h>
#include <rationnel.h>
#include <ensemble.h>
#include <outils.h>
#include <parse.h>
#include <scan.h>

int test_suivant(){
	int result = 1;
    {
       Rationnel * rat;
       rat = expression_to_rationnel("a");
       numeroter_rationnel(rat);
       Ensemble * e1 = suivant(rat, 1);
		
       TEST(
          1
          && rat
          && e1
          && taille_ensemble( e1 ) == 0
          , result
       );

		liberer_ensemble( e1 );
    }

    {
       Rationnel * rat;
       rat = expression_to_rationnel("a.b");
       numeroter_rationnel(rat);
       Ensemble * e1 = suivant(rat,1);
       Ensemble * e2 = suivant(rat,2);
       
       TEST(
          1
          && rat
          && e1
          && est_dans_l_ensemble(e1, 2)
          && taille_ensemble( e1 ) == 1
          , result);
       
       TEST(
          1
          && rat
          && e2
          && taille_ensemble( e2 ) == 0
          , result);

		liberer_ensemble( e1 );
		liberer_ensemble( e2 );
    }

    {
       Rationnel * rat;
       rat = expression_to_rationnel("(a.a)*.(b.c)");
       numeroter_rationnel(rat);
       Ensemble * e1 = suivant(rat,1);
       Ensemble * e2 = suivant(rat,2);
       Ensemble * e3 = suivant(rat,3);
       Ensemble * e4 = suivant(rat,4);
       
       TEST(
          1
          && rat
          && e1
          && est_dans_l_ensemble(e1, 2)
          && taille_ensemble( e1 ) == 1
          , result);
	
       TEST(
          1
          && rat
          && e2
          && est_dans_l_ensemble(e2, 1)
          && est_dans_l_ensemble(e2, 3)
          && taille_ensemble( e2 ) == 2
          , result);
		
       TEST(
          1
          && rat
          && e3
          && est_dans_l_ensemble(e3, 4)
          && taille_ensemble( e3 ) == 1
          , result);
	
       TEST(
          1
          && rat
          && e4
          && taille_ensemble( e4 ) == 0
          , result);
	
		liberer_ensemble( e1 );
		liberer_ensemble( e2 );
		liberer_ensemble( e3 );
		liberer_ensemble( e4 );
    }

    {
       Rationnel * rat;
       rat = expression_to_rationnel("(a.a)*.(b+c)");
       numeroter_rationnel(rat);
       
       Ensemble * e1 = suivant(rat,1);
       Ensemble * e2 = suivant(rat,2);
       Ensemble * e3 = suivant(rat,3);
       Ensemble * e4 = suivant(rat,4);
       
       TEST(
          1
          && rat
          && e1
          && est_dans_l_ensemble(e1, 2)
          && taille_ensemble( e1 ) == 1
          , result);
	
       TEST(
          1
          && rat
          && e2
          && est_dans_l_ensemble(e2, 1)
          && est_dans_l_ensemble(e2, 3)
          && est_dans_l_ensemble(e2, 4)
          && taille_ensemble( e2 ) == 3
          , result);
		
       TEST(
          1
          && rat
          && e3
          && taille_ensemble( e3 ) == 0
          , result);
	
       TEST(
          1
          && rat
          && e4
          && taille_ensemble( e4 ) == 0
          , result);
	
		liberer_ensemble( e1 );
		liberer_ensemble( e2 );
		liberer_ensemble( e3 );
		liberer_ensemble( e4 );
    }

    {
       Rationnel * rat;
       rat = expression_to_rationnel(
           "a+a.(a+(a.a+(a+a.(a+a)*.(a.a)*+a)*+a.a)*+a).a+a"
       );
       numeroter_rationnel(rat);
       Ensemble * e3 = suivant(rat,9);

       TEST(
          1
          && rat
          && e3
          && est_dans_l_ensemble(e3, 4)
          && est_dans_l_ensemble(e3, 6)
          && est_dans_l_ensemble(e3, 7)
          && est_dans_l_ensemble(e3, 8)
          && est_dans_l_ensemble(e3, 9)
          && est_dans_l_ensemble(e3, 10)
          && est_dans_l_ensemble(e3, 12)
          && est_dans_l_ensemble(e3, 13)
          && est_dans_l_ensemble(e3, 16)
          && taille_ensemble( e3 ) == 9
          , result);

       liberer_ensemble(e3);
    }

    return result;
}

int main(int argc, char *argv[])
{
   if( ! test_suivant() )
    return 1; 
   
   return 0;
}
