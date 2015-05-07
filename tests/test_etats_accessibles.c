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

int test_etats_accessibles(){

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

		Ensemble * acc = etats_accessibles( automate, 1 );

		TEST(
			1
			&& acc
			&& est_dans_l_ensemble( acc, 1 ) 
			&& est_dans_l_ensemble( acc, 3 ) 
			&& est_dans_l_ensemble( acc, 4 ) 
			&& est_dans_l_ensemble( acc, 5 ) 
			&& ! est_dans_l_ensemble( acc, 2 ) 
			&& ! est_dans_l_ensemble( acc, 6 ) 
			&& ! est_dans_l_ensemble( acc, 7 ) 
			, result
		);
		liberer_automate( automate );
		liberer_ensemble( acc );
	}

	return result;
}



int main(){
	
	if( ! test_etats_accessibles() ){ return 1; }

	return 0;
}
