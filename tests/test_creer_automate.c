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
#include "fifo.h"

#include <signal.h>
#include <errno.h>

int test_creer_automate(){

	int result = 1;

	Automate * automate = creer_automate();

	ajouter_lettre( automate, 'a' );
	ajouter_lettre( automate, 'd' );

	ajouter_etat( automate, 3 );
	ajouter_etat_final( automate, 6 );

	ajouter_transition( automate, 3, 'a', 5 );
	ajouter_transition( automate, 3, 'b', 3 );

	ajouter_etat_final( automate, 5 );
	ajouter_etat_initial( automate, 3 );

	TEST( 
		1
		&& automate
		&& est_un_etat_de_l_automate( automate, 3)
		&& est_un_etat_de_l_automate( automate, 5)
		&& est_un_etat_de_l_automate( automate, 6)
		, result
	);
	TEST( 
		1
		&& automate
		&& ! est_un_etat_final_de_l_automate( automate, 3)
		&& est_un_etat_final_de_l_automate( automate, 5)
		&& est_un_etat_final_de_l_automate( automate, 6)
		, result
	);
	TEST( 
		1
		&& automate
		&& est_un_etat_initial_de_l_automate( automate, 3)
		&& ! est_un_etat_initial_de_l_automate( automate, 5)
		, result
	);
	TEST( 
		1
		&& automate
		&& est_une_lettre_de_l_automate( automate, 'a')
		&& est_une_lettre_de_l_automate( automate, 'b')
		&& est_une_lettre_de_l_automate( automate, 'd')
		&& ! est_une_lettre_de_l_automate( automate, 'c')
		, result
	);
	TEST( 
		1
		&& automate
		&& est_une_transition_de_l_automate( automate, 3, 'a', 5 )
		&& est_une_transition_de_l_automate( automate, 3, 'b', 3 )
		&& ! est_une_transition_de_l_automate( automate, 3, 'b', 5 )
		&& ! est_une_transition_de_l_automate( automate, 3, 'a', 3 )
		&& ! est_une_transition_de_l_automate( automate, 5, 'a', 3 )
		&& ! est_une_transition_de_l_automate( automate, 5, 'b', 3 )
		&& ! est_une_transition_de_l_automate( automate, 5, 'a', 5 )
		&& ! est_une_transition_de_l_automate( automate, 5, 'b', 5 )
		, result
	);

	const Ensemble* ens = get_initiaux( automate );

	TEST( 
		1
		&& ens
		&& est_dans_l_ensemble( ens, 3 )
		&& ! est_dans_l_ensemble( ens, 5 )
		&& ! est_dans_l_ensemble( ens, 6 )
		, result
	);

	ens = get_finaux( automate );

	TEST( 
		1
		&& ens
		&& ! est_dans_l_ensemble( ens, 3 )
		&& est_dans_l_ensemble( ens, 5 )
		&& est_dans_l_ensemble( ens, 6 )
		, result
	);

	ens = get_alphabet( automate );

	TEST( 
		1
		&& ens
		&& est_dans_l_ensemble( ens, 'a')
		&& est_dans_l_ensemble( ens, 'b')
		&& ! est_dans_l_ensemble( ens, 'c')
		&& est_dans_l_ensemble( ens, 'd')
		, result
	);


	liberer_automate( automate );

	return result;
}


int main(){

	if( ! test_creer_automate() ){ return 1; }

	return 0;
}
