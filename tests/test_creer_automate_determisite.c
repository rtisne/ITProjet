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
#include "ensemble.h"

#include <stdarg.h>

int sont_dans_l_ensemble( int n, Ensemble * ens, ...  ){
	va_list pile;
	int res = 1;
	int i;
	va_start(pile,ens);
	for (i=0;i<n;i++){
		res &= est_dans_l_ensemble( ens, va_arg(pile,int) );
	}
	va_end(pile);
	return res;
}

int l_ensemble_est_egal( int n, const Ensemble* ens, ... ){
	va_list pile;
	int res;
	int i;

	Ensemble * e = creer_ensemble( NULL, NULL, NULL );

	va_start(pile,ens);
	for (i=0;i<n;i++){
		ajouter_element( e, va_arg(pile,int) ); 
 	}

	res = comparer_ensemble( ens, e );
	liberer_ensemble( e );

	va_end(pile);
	return res == 0;
}

void action_test_transitions_automate(
	int origine, char lettre, int fin, void* data
){
	int * res = (int*) data;
	(*res) += 1;
}

int test_transitions_automate( int nb_transitions, const Automate* aut, ... ){
	va_list pile;
	int i;
	int nb = 0;
	pour_toute_transition( aut, action_test_transitions_automate, &nb );
	if( nb_transitions != nb ){ return 0; }

	va_start(pile,aut);
	for (i=0;i<nb_transitions;i++){
		int origine = va_arg(pile,int);
		char lettre = (char) va_arg(pile,int);
		int fin = va_arg(pile,int);
		if(
			! est_une_transition_de_l_automate(
				aut, origine, lettre, fin 
			)
		){ return 0; }
 	}
	
	va_end(pile);
	return 1;
}


int test_creer_automate_deterministe(){
	int resultat = 1;
	{
		Automate* automate = creer_automate();
		Automate* determinise;		

		ajouter_transition( automate, 0, 'a', 1 );
		ajouter_transition( automate, 0, 'a', 2 );
		ajouter_transition( automate, 0, 'b', 3 );

		determinise = creer_automate_deterministe( automate );

		TEST(
			1
			&& l_ensemble_est_egal( 1, get_etats( determinise ), 0 )
			&& l_ensemble_est_egal( 1, get_initiaux( determinise ), 0 )
			&& l_ensemble_est_egal( 0, get_finaux( determinise ) ),
			resultat
		);	

		liberer_automate( determinise );
		liberer_automate( automate );
	}

	{
		Automate* automate = creer_automate();
		Automate* determinise;		

		ajouter_etat_initial( automate, 0 );
		ajouter_etat_final( automate, 2 );
		ajouter_transition( automate, 0, 'a', 1 );
		ajouter_transition( automate, 0, 'a', 2 );
		ajouter_transition( automate, 0, 'b', 3 );

		determinise = creer_automate_deterministe( automate );

		TEST(
			1
			&& l_ensemble_est_egal( 4, get_etats( determinise ), 0, 1, 2, 3 )
			&& l_ensemble_est_egal( 1, get_initiaux( determinise ), 0 )
			&& l_ensemble_est_egal( 1, get_finaux( determinise ), 1 )
			&& test_transitions_automate( 
				8, determinise,
				0, 'a', 1, 
				0, 'b', 2, 
				1, 'a', 3, 
				1, 'b', 3, 
				2, 'a', 3, 
				2, 'b', 3, 
				3, 'a', 3, 
				3, 'b', 3
			),
			resultat
		);	

		liberer_automate( determinise );
		liberer_automate( automate );
	}

	{
		Automate* automate = creer_automate();
		Automate* determinise;		

		determinise = creer_automate_deterministe( automate );

		TEST(
			1
			&& l_ensemble_est_egal( 1, get_etats( determinise ), 0 )
			&& l_ensemble_est_egal( 1, get_initiaux( determinise ), 0 )
			&& l_ensemble_est_egal( 0, get_finaux( determinise ) )
			&& test_transitions_automate( 
				0, determinise
			),
			resultat
		);	

		liberer_automate( determinise );
		liberer_automate( automate );
	}

	{
		Automate* automate = creer_automate();
		Automate* determinise;		

		ajouter_etat_initial( automate, 0 );
		ajouter_etat_initial( automate, 7 );
		ajouter_etat_final( automate, 3 );
		ajouter_etat_final( automate, 4 );
		ajouter_transition( automate, 6, 'a', 0 );
		ajouter_transition( automate, 0, 'a', 0 );
		ajouter_transition( automate, 0, 'b', 0 );
		ajouter_transition( automate, 0, 'a', 2 );
		ajouter_transition( automate, 2, 'b', 3 );
		ajouter_transition( automate, 3, 'a', 3 );
		ajouter_transition( automate, 3, 'b', 3 );

		determinise = creer_automate_deterministe( automate );

		TEST(
			1
			&& l_ensemble_est_egal( 5, get_etats( determinise ), 0, 1, 2, 3, 4 )
			&& l_ensemble_est_egal( 1, get_initiaux( determinise ), 0 )
			&& l_ensemble_est_egal( 2, get_finaux( determinise ), 3, 4 )
			&& test_transitions_automate( 
				10, determinise,
				0, 'a', 1, 
				0, 'b', 2, 
				1, 'a', 1, 
				1, 'b', 3, 
				2, 'a', 1, 
				2, 'b', 2, 
				3, 'a', 4, 
				3, 'b', 3,
				4, 'a', 4, 
				4, 'b', 3
			),
			resultat
		);	

		liberer_automate( determinise );
		liberer_automate( automate );
	}


	return resultat;
};




int main(){

	if( ! test_creer_automate_deterministe() ){ return 1; }

	return 0;
}
