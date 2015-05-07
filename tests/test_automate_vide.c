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


int test_automate_vide(){

	int result = 1;

	Automate * automate = creer_automate();

	TEST( 
		1
		&& automate
		&& ! est_un_etat_de_l_automate( automate, 0)
		&& ! est_un_etat_final_de_l_automate( automate, 0)
		&& ! est_un_etat_initial_de_l_automate( automate, 0)
		&& ! est_une_lettre_de_l_automate( automate, 'a')
		&& ! est_une_transition_de_l_automate( automate, 0, 'a', 0 )
		, result
	);

	liberer_automate( automate );

	return result;

}


int main(){

	if( ! test_automate_vide() ){ return 1; }

	return 0;
}
