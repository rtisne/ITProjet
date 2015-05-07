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

#include "rationnel.h"
#include "rationnel.c"
#include "outils.h"
#include "fifo.h"

#include <signal.h>
#include <errno.h>

int test_glushkov(){

	int result = 1;
	
	{
		Rationnel * expression = expression_to_rationnel( "a" );
		Automate * automate1 = Glushkov( expression );

		Automate * automate2 = creer_automate();
		ajouter_etat_initial( automate2, 0 );
		ajouter_transition( automate2, 0, 'a', 1 );
		ajouter_etat_final( automate2, 1 );

		TEST( 
			1
			&& expression 
			&& automate1
			&& automate2
			&& comparer_automates( automate1, automate2 )
			, result
		);

		liberer_automate( automate1 );
		liberer_automate( automate2 );
	}

	{
		Rationnel * expression = expression_to_rationnel( "b" );
		Automate * automate1 = Glushkov( expression );

		Automate * automate2 = creer_automate();
		ajouter_etat_initial( automate2, 0 );
		ajouter_transition( automate2, 0, 'b', 1 );
		ajouter_etat_final( automate2, 1 );

		TEST( 
			1
			&& expression 
			&& automate1
			&& automate2
			&& comparer_automates( automate1, automate2 )
			, result
		);

		liberer_automate( automate1 );
		liberer_automate( automate2 );
	}

	{
		Rationnel * expression = expression_to_rationnel( "a.b" );
		Automate * automate1 = Glushkov( expression );

		Automate * automate2 = creer_automate();
		ajouter_etat_initial( automate2, 0 );
		ajouter_transition( automate2, 0, 'a', 1 );
		ajouter_transition( automate2, 1, 'b', 2 );
		ajouter_etat_final( automate2, 2 );

		TEST( 
			1
			&& expression 
			&& automate1
			&& automate2
			&& comparer_automates( automate1, automate2 )
			, result
		);

		liberer_automate( automate1 );
		liberer_automate( automate2 );
	}

	{
		Rationnel * expression = expression_to_rationnel( "a+b" );
		Automate * automate1 = Glushkov( expression );

		Automate * automate2 = creer_automate();
		ajouter_etat_initial( automate2, 0 );
		ajouter_transition( automate2, 0, 'a', 1 );
		ajouter_transition( automate2, 0, 'b', 2 );
		ajouter_etat_final( automate2, 1 );
		ajouter_etat_final( automate2, 2 );

		TEST( 
			1
			&& expression 
			&& automate1
			&& automate2
			&& comparer_automates( automate1, automate2 )
			, result
		);

		liberer_automate( automate1 );
		liberer_automate( automate2 );
	}

	{
		Rationnel * expression = expression_to_rationnel( "((a*.b)*.c)*.d" );
		Automate * automate1 = Glushkov( expression );

		Automate * automate2 = creer_automate();
		ajouter_etat_initial( automate2, 0 );

		ajouter_transition( automate2, 0, 'a', 1 );
		ajouter_transition( automate2, 0, 'b', 2 );
		ajouter_transition( automate2, 0, 'c', 3 );
		ajouter_transition( automate2, 0, 'd', 4 );

		ajouter_transition( automate2, 1, 'a', 1 );
		ajouter_transition( automate2, 1, 'b', 2 );

		ajouter_transition( automate2, 2, 'a', 1 );
		ajouter_transition( automate2, 2, 'b', 2 );
		ajouter_transition( automate2, 2, 'c', 3 );

		ajouter_transition( automate2, 3, 'a', 1 );
		ajouter_transition( automate2, 3, 'b', 2 );
		ajouter_transition( automate2, 3, 'c', 3 );
		ajouter_transition( automate2, 3, 'd', 4 );

		ajouter_etat_final( automate2, 4 );

		TEST( 
			1
			&& expression 
			&& automate1
			&& automate2
			&& comparer_automates( automate1, automate2 )
			, result
		);

		liberer_automate( automate1 );
		liberer_automate( automate2 );
	}

	{
		Rationnel * expression = expression_to_rationnel( "(a*+(b.a)*.c)*.(b.c)*" );
		Automate * automate1 = Glushkov( expression );

		Automate * automate2 = creer_automate();
		ajouter_etat_initial( automate2, 0 );
		ajouter_transition( automate2, 0, 'a', 1 );
		ajouter_transition( automate2, 0, 'b', 2 );
		ajouter_transition( automate2, 0, 'c', 4 );
		ajouter_transition( automate2, 0, 'b', 5 );
		ajouter_transition( automate2, 1, 'a', 1 );
		ajouter_transition( automate2, 1, 'b', 2 );
		ajouter_transition( automate2, 1, 'c', 4 );
		ajouter_transition( automate2, 1, 'b', 5 );
		ajouter_transition( automate2, 2, 'a', 3 );
		ajouter_transition( automate2, 3, 'b', 2 );
		ajouter_transition( automate2, 3, 'c', 4 );
		ajouter_transition( automate2, 4, 'a', 1 );
		ajouter_transition( automate2, 4, 'b', 2 );
		ajouter_transition( automate2, 4, 'c', 4 );
		ajouter_transition( automate2, 4, 'b', 5 );
		ajouter_transition( automate2, 5, 'c', 6 );
		ajouter_transition( automate2, 6, 'b', 5 );
		ajouter_etat_final( automate2, 0 );
		ajouter_etat_final( automate2, 1 );
		ajouter_etat_final( automate2, 4 );
		ajouter_etat_final( automate2, 6 );

		TEST( 
			1
			&& expression 
			&& automate1
			&& automate2
			&& comparer_automates( automate1, automate2 )
			, result
		);

		liberer_automate( automate1 );
		liberer_automate( automate2 );
	}

	return result;
}


int main(){

	if( ! test_glushkov() ){ return 1; }

	return 0;
}
