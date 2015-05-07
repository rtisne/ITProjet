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


int test_delta_delta_star(){

	int result = 1;

	Automate* automate = creer_automate();


	ajouter_etat( automate, 3 );
	ajouter_etat( automate, 5 );
	ajouter_transition( automate, 3, 'a', 5 );
	ajouter_transition( automate, 5, 'b', 3 );
	ajouter_transition( automate, 5, 'a', 5 );
	ajouter_transition( automate, 5, 'c', 6 );
	ajouter_etat_initial( automate, 3 );
	ajouter_etat_final( automate, 6 );

	TEST(
		1
		&& automate
		&& est_un_etat_de_l_automate( automate, 3 )
		&& est_un_etat_de_l_automate( automate, 5 )
		&& est_un_etat_de_l_automate( automate, 6 )

		&& est_un_etat_initial_de_l_automate( automate, 3 )
		&& ! est_un_etat_initial_de_l_automate( automate, 5 )
		&& ! est_un_etat_initial_de_l_automate( automate, 6 )

		&& ! est_un_etat_final_de_l_automate( automate, 3 )
		&& ! est_un_etat_final_de_l_automate( automate, 5 )
		&& est_un_etat_final_de_l_automate( automate, 6 )
		
		&& est_une_lettre_de_l_automate( automate, 'a' )
		&& est_une_lettre_de_l_automate( automate, 'b' )
		&& est_une_lettre_de_l_automate( automate, 'c' )
		&& ! est_une_lettre_de_l_automate( automate, 'd' )

		&& est_une_transition_de_l_automate( automate, 3, 'a', 5 )
		&& est_une_transition_de_l_automate( automate, 5, 'b', 3 )
		&& est_une_transition_de_l_automate( automate, 5, 'a', 5 )
		&& est_une_transition_de_l_automate( automate, 5, 'c', 6 )
		
		&& ! est_une_transition_de_l_automate( automate, 3, 'b', 5 )
		&& ! est_une_transition_de_l_automate( automate, 1, 'b', 5 )
		&& ! est_une_transition_de_l_automate( automate, 3, 'a', 1 )
		, result
	);

	Ensemble * etat_courant = creer_ensemble( NULL, NULL, NULL );
	ajouter_element( etat_courant, 3 ); 

	deplacer_ensemble( etat_courant, delta( automate, etat_courant, 'a' ) );

	TEST(
		1
		&& etat_courant
		&& est_dans_l_ensemble( etat_courant, 5 )
		&& taille_ensemble( etat_courant ) ==1
		, result
	);

	deplacer_ensemble(
		etat_courant, delta_star( automate, etat_courant, "ab" ) 
	);

	TEST(
		1
		&& etat_courant
		&& est_dans_l_ensemble( etat_courant, 3 )
		&& taille_ensemble( etat_courant ) ==1
		, result
	);

	deplacer_ensemble(
		etat_courant, delta_star( automate, etat_courant, "ac" )
	);

	TEST(
		1
		&& etat_courant
		&& est_dans_l_ensemble( etat_courant, 6 )
		&& taille_ensemble( etat_courant ) ==1
		, result
	);

	deplacer_ensemble(
		etat_courant, delta_star( automate, etat_courant, "" )
	);

	TEST(
		1
		&& etat_courant
		&& est_dans_l_ensemble( etat_courant, 6 )
		&& taille_ensemble( etat_courant ) ==1
		, result
	);

	deplacer_ensemble(
		etat_courant, delta_star( automate, etat_courant, "c" )
	);

	TEST(
		1
		&& etat_courant
		&& taille_ensemble( etat_courant ) == 0
		, result
	);

	liberer_ensemble( etat_courant );
	liberer_automate( automate );

	return result;
}


int main(){

	if( ! test_delta_delta_star() ){ return 1; }

	return 0;
}
