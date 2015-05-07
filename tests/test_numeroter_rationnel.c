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

int test_numeroter_rationnel(){
	int result = 1;
    
	{
       Rationnel * rat = Lettre('a');
       numeroter_rationnel( rat );      
 
       TEST(
          1
          && rat
          && rat->position_min == 1 && rat->position_max == 1
          , result);
    }

    {

       Rationnel * rat1 = Lettre('a');
       Rationnel * rat2 = Lettre('b');
       Rationnel * rat3 = Union( rat1, rat2 );
       numeroter_rationnel( rat3 );      
       
       TEST(
          1
          && rat1
          && rat2
          && rat3
          && rat1->position_min == 1 && rat1->position_max == 1
          && rat2->position_min == 2 && rat2->position_max == 2
          && rat3->position_min == 1 && rat3->position_max == 2
          , result);
    }

    {

       Rationnel * rat1 = Lettre('a');
       Rationnel * rat2 = Lettre('b');
       Rationnel * rat3 = Concat( rat1, rat2 );
       numeroter_rationnel( rat3 );      
       
       TEST(
          1
          && rat1
          && rat2
          && rat3
          && rat1->position_min == 1 && rat1->position_max == 1
          && rat2->position_min == 2 && rat2->position_max == 2
          && rat3->position_min == 1 && rat3->position_max == 2
          , result);
    }

    {

       Rationnel * rat1 = Lettre('a');
       Rationnel * rat2 = Star( rat1 );
       numeroter_rationnel( rat2 );      
       
       TEST(
          1
          && rat1
          && rat2
          && rat1->position_min == 1 && rat1->position_max == 1
          && rat2->position_min == 1 && rat2->position_max == 1
          , result);
    }

    {

       Rationnel * rat1 = Lettre('a');
       Rationnel * rat2 = Lettre('b');
       Rationnel * rat3 = Concat( rat1, rat2 );
       Rationnel * rat4 = Star( rat3 );
       Rationnel * rat5 = Lettre('c');
       Rationnel * rat6 = Lettre('d');
       Rationnel * rat7 = Union( rat5, rat6 );
       Rationnel * rat8 = Star( rat7 );
       Rationnel * rat9 = Concat( rat4, rat8 );
       Rationnel * rat10 = Star( rat9 );

       numeroter_rationnel( rat10 );      
       
       TEST(
          1
          && rat1
          && rat2
          && rat3
          && rat4
          && rat5
          && rat7
          && rat6
          && rat8
          && rat9
          && rat10

          && rat1->position_min == 1 && rat1->position_max == 1
          && rat2->position_min == 2 && rat2->position_max == 2
          && rat3->position_min == 1 && rat3->position_max == 2
          && rat4->position_min == 1 && rat4->position_max == 2

          && rat5->position_min == 3 && rat5->position_max == 3
          && rat6->position_min == 4 && rat6->position_max == 4
          && rat7->position_min == 3 && rat7->position_max == 4
          && rat8->position_min == 3 && rat8->position_max == 4

          && rat9->position_min == 1 && rat9->position_max == 4
          && rat10->position_min == 1 && rat10->position_max == 4
          , result);
    }

    return result;
}

int main(int argc, char *argv[])
{
   if( ! test_numeroter_rationnel() )
    return 1; 
   
   return 0;
}


