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


int test_automate_miroir(){

	int result = 1;

	{
		Automate * automate = creer_automate();

		ajouter_transition( automate, 1, 'a', 2 );
		ajouter_transition( automate, 1, 'b', 3 );
		ajouter_transition( automate, 4, 'c', 3 );
		ajouter_transition( automate, 5, 'd', 6 );
		ajouter_transition( automate, 7, 'e', 8 );
		ajouter_transition( automate, 4, 'f', 2 );
		ajouter_etat_initial( automate, 1);
		ajouter_etat_initial( automate, 5);
		ajouter_etat_initial( automate, 9);
		ajouter_etat_final( automate, 3);
		ajouter_etat_final( automate, 8);
		ajouter_etat_final( automate, 9);


		Automate * mir = miroir( automate );

		TEST(
			1
			&& mir
			&& le_mot_est_reconnu( mir, "" )
			&& le_mot_est_reconnu( mir, "b" )
			&& ! le_mot_est_reconnu( mir, "c" )
			&& ! le_mot_est_reconnu( mir, "d" )
			&& ! le_mot_est_reconnu( mir, "e" )
			&& ! le_mot_est_reconnu( mir, "f" )
			&& ! le_mot_est_reconnu( mir, "afc" )
			, result
		);
		liberer_automate( automate );
		liberer_automate( mir );
	}

	return result;
}



int main(){

	if( ! test_automate_miroir() ){ return 1; }

	return 0;
}
