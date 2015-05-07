/*
 *   Ce fichier fait partie d'un projet de programmation donné en Licence 3 
 *   à l'Université de Bordeaux
 *
 *   Copyright (C) 2015 Adrien Boussicault
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


#include "automate.h"
#include "outils.h"

int test_automate_accessible(){
	int result = 1;

	{
		Automate * automate = creer_automate();

		ajouter_transition( automate, 1, 'a', 3 );
		ajouter_transition( automate, 1, 'b', 1 );
		ajouter_transition( automate, 2, 'a', 1 );
		ajouter_transition( automate, 3, 'a', 4 );
		ajouter_transition( automate, 3, 'a', 5 );
		ajouter_etat( automate, 6 );
		ajouter_etat( automate, 7 );
		ajouter_etat_initial( automate, 1);
		ajouter_etat_initial( automate, 6);
		ajouter_etat_final( automate, 3);
		ajouter_etat_final( automate, 7);

		Automate * aut = automate_accessible( automate );

		TEST(
			1
			&& aut
			&& est_un_etat_de_l_automate( aut, 1 )
			&& est_un_etat_de_l_automate( aut, 3 )
			&& est_un_etat_de_l_automate( aut, 4 )
			&& est_un_etat_de_l_automate( aut, 5 )
			&& est_un_etat_de_l_automate( aut, 6 )
			&& ! est_un_etat_de_l_automate( aut, 7 )
			&& ! est_un_etat_de_l_automate( aut, 2 )
			&& est_une_transition_de_l_automate( aut, 1, 'a', 3 )
			&& est_une_transition_de_l_automate( aut, 1, 'b', 1 )
			&& est_une_transition_de_l_automate( aut, 3, 'a', 4 )
			&& est_une_transition_de_l_automate( aut, 3, 'a', 5 )
			, result
		);
		liberer_automate( aut );
		liberer_automate( automate );
	}

	return result;
}


int main(){

	if( ! test_automate_accessible() ){ return 1; };

	return 0;
	
}
