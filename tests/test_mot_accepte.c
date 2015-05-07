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


int test_mot_accepte(){

	int result = 1;

	Automate * automate = creer_automate();

	ajouter_transition( automate, 1, 'a', 1 );
	ajouter_transition( automate, 1, 'b', 2 );

	ajouter_etat_final( automate, 2 );
	ajouter_etat_initial( automate, 1 );

	TEST( automate && le_mot_est_reconnu( automate, "b" ), result );
	TEST( automate && le_mot_est_reconnu( automate, "ab" ), result );
	TEST( automate && le_mot_est_reconnu( automate, "aab" ), result );
	TEST( automate && ! le_mot_est_reconnu( automate, "aba" ), result );

	liberer_automate( automate );

	return result;

}

int main(){

	if( ! test_mot_accepte() ){ return 1; }

	return 0;
}
