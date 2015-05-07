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

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int elmt;
} Elmt;

int comparer_elmt( const Elmt* a, const Elmt * b){
	return a->elmt - b->elmt;
}

void initialiser_elmt( Elmt* v, int elmt ){
	v->elmt = elmt;
}

void print_elmt( const Elmt * c ){
	printf( "elmt : %d", c->elmt );
}

Elmt * creer_elmt( int elmt ){
	Elmt * res = xmalloc(sizeof(Elmt));
	initialiser_elmt( res, elmt );
	return res;
}

void supprimer_elmt( Elmt* elmt ){
	xfree( elmt );
}

Elmt * copier_elmt( const Elmt * elmt ) {
	return creer_elmt( elmt->elmt );
};




int general_tests(){
	int result = 1;

	Ensemble * ens = creer_ensemble( NULL, NULL, NULL );

	ajouter_element( ens, 1 );
	ajouter_element( ens, 5 );
	ajouter_element( ens, -1 );
	ajouter_element( ens, 1 );
	ajouter_element( ens, 5 );
	ajouter_element( ens, 4 );

	TEST( taille_ensemble(ens) == 4, result );
	TEST( est_dans_l_ensemble( ens, 1 ), result );
	TEST( est_dans_l_ensemble( ens, 5 ), result );
	TEST( est_dans_l_ensemble( ens, -1 ), result );
	TEST( est_dans_l_ensemble( ens, 4 ), result );
	TEST( ! est_dans_l_ensemble( ens, 2 ), result );
	TEST( ! est_dans_l_ensemble( ens, 3 ), result );
	TEST( ! est_dans_l_ensemble( ens, 6 ), result );
	TEST( ! est_dans_l_ensemble( ens, -2 ), result );

	retirer_element( ens, 1 );

	TEST( taille_ensemble(ens) == 3, result );
	TEST( ! est_dans_l_ensemble( ens, 1 ), result );
	TEST( est_dans_l_ensemble( ens, 5 ), result );
	TEST( est_dans_l_ensemble( ens, -1 ), result );
	TEST( est_dans_l_ensemble( ens, 4 ), result );
	TEST( ! est_dans_l_ensemble( ens, 2 ), result );
	TEST( ! est_dans_l_ensemble( ens, 3 ), result );
	TEST( ! est_dans_l_ensemble( ens, 6 ), result );
	TEST( ! est_dans_l_ensemble( ens, -2 ), result );

	liberer_ensemble( ens );

	ens = creer_ensemble(
		(int (*)( const intptr_t, const intptr_t)) comparer_elmt, 
		(intptr_t (*)( const intptr_t )) copier_elmt, 
		(void (*)( intptr_t )) supprimer_elmt
	);

	Elmt elmt;

	initialiser_elmt( &elmt, 1 );
	ajouter_element( ens, (intptr_t) &elmt );

	initialiser_elmt( &elmt, 5 );
	ajouter_element( ens, (intptr_t) &elmt );

	initialiser_elmt( &elmt, -1 );
	ajouter_element( ens, (intptr_t) &elmt );

	initialiser_elmt( &elmt, 1 );
	ajouter_element( ens, (intptr_t) &elmt );

	initialiser_elmt( &elmt, 5 );
	ajouter_element( ens, (intptr_t) &elmt );

	initialiser_elmt( &elmt, 4 );
	ajouter_element( ens, (intptr_t) &elmt );

	TEST( taille_ensemble(ens) == 4, result );
	initialiser_elmt( &elmt, 1 );
	TEST( est_dans_l_ensemble( ens, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 5 );
	TEST( est_dans_l_ensemble( ens, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, -1 );
	TEST( est_dans_l_ensemble( ens, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 4 );
	TEST( est_dans_l_ensemble( ens, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 2 );
	TEST( ! est_dans_l_ensemble( ens, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 3 );
	TEST( ! est_dans_l_ensemble( ens, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 6 );
	TEST( ! est_dans_l_ensemble( ens, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, -2 );
	TEST( ! est_dans_l_ensemble( ens, (intptr_t) &elmt ), result );

	initialiser_elmt( &elmt, 1 );
	retirer_element( ens,  (intptr_t) &elmt );

	TEST( taille_ensemble(ens) == 3, result );
	initialiser_elmt( &elmt, 1 );
	TEST( ! est_dans_l_ensemble( ens, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 5 );
	TEST( est_dans_l_ensemble( ens, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, -1 );
	TEST( est_dans_l_ensemble( ens, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 4 );
	TEST( est_dans_l_ensemble( ens, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 2 );
	TEST( ! est_dans_l_ensemble( ens, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 3 );
	TEST( ! est_dans_l_ensemble( ens, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 6 );
	TEST( ! est_dans_l_ensemble( ens, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, -2 );
	TEST( ! est_dans_l_ensemble( ens, (intptr_t) &elmt ), result );

	liberer_ensemble( ens );

	return result;
}

int test_creer_ensemble(){
	int result = 1;

	// Voir general_tests
	return result;
}

int test_liberer_ensemble(){
	int result = 1;

	Ensemble * ens = creer_ensemble( NULL, NULL, NULL );
	
	liberer_ensemble( ens );


	ens = creer_ensemble(
		(int (*)( const intptr_t, const intptr_t)) comparer_elmt, 
		(intptr_t (*)( const intptr_t )) copier_elmt, 
		(void (*)( intptr_t )) supprimer_elmt
	);

	liberer_ensemble( ens );

	// Voir general_tests
	return result;
}

int test_ajouter_element(){
	int result = 1;

	//Voir general_tests
	return result;
}

int test_ajouter_elements(){
	int result = 1;

	Ensemble * ens1 = creer_ensemble( NULL, NULL, NULL );

	ajouter_element( ens1, 1 );
	ajouter_element( ens1, 2 );
	ajouter_element( ens1, 5 );

	Ensemble * ens2 = creer_ensemble( NULL, NULL, NULL );

	ajouter_element( ens2, 2 );
	ajouter_element( ens2, 5 );
	ajouter_element( ens2, 7 );
	ajouter_element( ens2, 9 );

	ajouter_elements( ens1, ens2 );
	
	TEST( taille_ensemble(ens1) == 5, result );
	TEST(  est_dans_l_ensemble( ens1, 1 ), result );
	TEST(  est_dans_l_ensemble( ens1, 2 ), result );
	TEST( ! est_dans_l_ensemble( ens1, 4 ), result );
	TEST( ! est_dans_l_ensemble( ens1, 3 ), result );
	TEST(  est_dans_l_ensemble( ens1, 5 ), result );
	TEST(  est_dans_l_ensemble( ens1, 7 ), result );
	TEST(  est_dans_l_ensemble( ens1, 9 ), result );

	liberer_ensemble( ens1 );
	liberer_ensemble( ens2 );



	ens1 = creer_ensemble(
		(int (*)( const intptr_t, const intptr_t)) comparer_elmt, 
		(intptr_t (*)( const intptr_t )) copier_elmt, 
		(void (*)( intptr_t )) supprimer_elmt
	);


	Elmt elmt;

	initialiser_elmt( &elmt, 1 );
	ajouter_element( ens1, (intptr_t) &elmt );

	initialiser_elmt( &elmt, 5 );
	ajouter_element( ens1, (intptr_t) &elmt );

	initialiser_elmt( &elmt, 4 );
	ajouter_element( ens1, (intptr_t) &elmt );


	ens2 = creer_ensemble(
		(int (*)( const intptr_t, const intptr_t)) comparer_elmt, 
		(intptr_t (*)( const intptr_t )) copier_elmt, 
		(void (*)( intptr_t )) supprimer_elmt
	);

	initialiser_elmt( &elmt, 4 );
	ajouter_element( ens2, (intptr_t) &elmt );

	initialiser_elmt( &elmt, 5 );
	ajouter_element( ens2, (intptr_t) &elmt );

	initialiser_elmt( &elmt, 7 );
	ajouter_element( ens2, (intptr_t) &elmt );


	ajouter_elements( ens1, ens2 );


	TEST( taille_ensemble(ens1) == 4, result );
	initialiser_elmt( &elmt, 1 );
	TEST( est_dans_l_ensemble( ens1, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 2 );
	TEST( ! est_dans_l_ensemble( ens1, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 3 );
	TEST( ! est_dans_l_ensemble( ens1, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 4 );
	TEST( est_dans_l_ensemble( ens1, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 5 );
	TEST( est_dans_l_ensemble( ens1, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 6 );
	TEST( ! est_dans_l_ensemble( ens1, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 7 );
	TEST( est_dans_l_ensemble( ens1, (intptr_t) &elmt ), result );

	liberer_ensemble( ens1 );
	liberer_ensemble( ens2 );

	return result;
}

int test_retirer_element(){
	int result = 1;

	//Voir general_tests
	return result;
}

int test_retirer_elements(){
	int result = 1;

	Ensemble * ens1 = creer_ensemble( NULL, NULL, NULL );

	ajouter_element( ens1, 1 );
	ajouter_element( ens1, 2 );
	ajouter_element( ens1, 5 );

	Ensemble * ens2 = creer_ensemble( NULL, NULL, NULL );

	ajouter_element( ens2, 2 );
	ajouter_element( ens2, 5 );
	ajouter_element( ens2, 7 );
	ajouter_element( ens2, 9 );

	retirer_elements( ens1, ens2 );
	
	TEST( taille_ensemble(ens1) == 1, result );
	TEST(  est_dans_l_ensemble( ens1, 1 ), result );
	TEST( ! est_dans_l_ensemble( ens1, 2 ), result );
	TEST( ! est_dans_l_ensemble( ens1, 4 ), result );
	TEST( ! est_dans_l_ensemble( ens1, 3 ), result );
	TEST( ! est_dans_l_ensemble( ens1, 5 ), result );
	TEST( ! est_dans_l_ensemble( ens1, 7 ), result );
	TEST( ! est_dans_l_ensemble( ens1, 9 ), result );

	liberer_ensemble( ens1 );
	liberer_ensemble( ens2 );

	ens1 = creer_ensemble(
		(int (*)( const intptr_t, const intptr_t)) comparer_elmt, 
		(intptr_t (*)( const intptr_t )) copier_elmt, 
		(void (*)( intptr_t )) supprimer_elmt
	);


	Elmt elmt;

	initialiser_elmt( &elmt, 5 );
	ajouter_element( ens1, (intptr_t) &elmt );

	initialiser_elmt( &elmt, 1 );
	ajouter_element( ens1, (intptr_t) &elmt );

	initialiser_elmt( &elmt, 4 );
	ajouter_element( ens1, (intptr_t) &elmt );


	ens2 = creer_ensemble(
		(int (*)( const intptr_t, const intptr_t)) comparer_elmt, 
		(intptr_t (*)( const intptr_t )) copier_elmt, 
		(void (*)( intptr_t )) supprimer_elmt
	);

	initialiser_elmt( &elmt, 7 );
	ajouter_element( ens2, (intptr_t) &elmt );

	initialiser_elmt( &elmt, 5 );
	ajouter_element( ens2, (intptr_t) &elmt );

	initialiser_elmt( &elmt, 4 );
	ajouter_element( ens2, (intptr_t) &elmt );


	retirer_elements( ens1, ens2 );


	TEST( taille_ensemble(ens1) == 1, result );
	initialiser_elmt( &elmt, 1 );
	TEST( est_dans_l_ensemble( ens1, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 2 );
	TEST( ! est_dans_l_ensemble( ens1, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 3 );
	TEST( ! est_dans_l_ensemble( ens1, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 4 );
	TEST( ! est_dans_l_ensemble( ens1, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 5 );
	TEST( ! est_dans_l_ensemble( ens1, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 6 );
	TEST( ! est_dans_l_ensemble( ens1, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 7 );
	TEST( ! est_dans_l_ensemble( ens1, (intptr_t) &elmt ), result );

	liberer_ensemble( ens1 );
	liberer_ensemble( ens2 );

	return result;
}

int test_vider_ensemble(){
	int result = 1;

	Ensemble * ens1 = creer_ensemble( NULL, NULL, NULL );

	ajouter_element( ens1, 2 );
	ajouter_element( ens1, 1 );
	ajouter_element( ens1, 5 );

	vider_ensemble( ens1 );

	TEST( taille_ensemble(ens1) == 0, result );
	TEST( ! est_dans_l_ensemble( ens1, 1 ), result );
	TEST( ! est_dans_l_ensemble( ens1, 2 ), result );
	TEST( ! est_dans_l_ensemble( ens1, 4 ), result );
	TEST( ! est_dans_l_ensemble( ens1, 3 ), result );
	TEST( ! est_dans_l_ensemble( ens1, 5 ), result );
	TEST( ! est_dans_l_ensemble( ens1, 6 ), result );

	liberer_ensemble( ens1 );

	ens1 = creer_ensemble(
		(int (*)( const intptr_t, const intptr_t)) comparer_elmt, 
		(intptr_t (*)( const intptr_t )) copier_elmt, 
		(void (*)( intptr_t )) supprimer_elmt
	);


	Elmt elmt;

	initialiser_elmt( &elmt, 4 );
	ajouter_element( ens1, (intptr_t) &elmt );

	initialiser_elmt( &elmt, 1 );
	ajouter_element( ens1, (intptr_t) &elmt );

	initialiser_elmt( &elmt, 4 );
	ajouter_element( ens1, (intptr_t) &elmt );

	vider_ensemble( ens1 );

	TEST( taille_ensemble(ens1) == 0, result );
	initialiser_elmt( &elmt, 1 );
	TEST( ! est_dans_l_ensemble( ens1, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 2 );
	TEST( ! est_dans_l_ensemble( ens1, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 3 );
	TEST( ! est_dans_l_ensemble( ens1, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 4 );
	TEST( ! est_dans_l_ensemble( ens1, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 5 );
	TEST( ! est_dans_l_ensemble( ens1, (intptr_t) &elmt ), result );
	initialiser_elmt( &elmt, 6 );
	TEST( ! est_dans_l_ensemble( ens1, (intptr_t) &elmt ), result );

	liberer_ensemble( ens1 );

	return result;
}

int test_est_dans_l_ensemble(){
	int result = 1;
	// Voir general_tests
	return result;
}

int test_taille_ensemble(){
	int result = 1;

	Ensemble * ens = creer_ensemble( NULL, NULL, NULL );

	TEST( taille_ensemble(ens) == 0 , result );
	
	liberer_ensemble( ens );

	ens = creer_ensemble(
		(int (*)( const intptr_t, const intptr_t)) comparer_elmt, 
		(intptr_t (*)( const intptr_t )) copier_elmt, 
		(void (*)( intptr_t )) supprimer_elmt
	);

	TEST( taille_ensemble(ens) == 0 , result );

	liberer_ensemble( ens );

	// Voir general_tests	
	return result;
}

int test_comparer_ensemble(){
	int result = 1;

	Ensemble * ens1 = creer_ensemble( NULL, NULL, NULL );

	Ensemble * ens2 = creer_ensemble( NULL, NULL, NULL );

	TEST( comparer_ensemble(ens1, ens2) == 0, result );

	ajouter_element( ens1, 2 );
	ajouter_element( ens1, 1 );
	ajouter_element( ens1, 5 );

	ajouter_element( ens2, 1 );
	ajouter_element( ens2, 2 );
	ajouter_element( ens2, 5 );

	TEST( comparer_ensemble(ens1, ens2) == 0, result );

	ajouter_element( ens1, 7 );

	TEST( comparer_ensemble(ens1, ens2) == 1, result );

	ajouter_element( ens1, 4 );

	TEST( comparer_ensemble(ens1, ens2) == -1, result );

	ajouter_element( ens2, 7 );

	TEST( comparer_ensemble(ens1, ens2) == -1, result );

	ajouter_element( ens2, 8 );

	TEST( comparer_ensemble(ens1, ens2) == -1, result );

	ajouter_element( ens2, 4 );

	TEST( comparer_ensemble(ens1, ens2) == -1, result );

	ajouter_element( ens2, 4 );

	TEST( comparer_ensemble(ens1, ens2) == -1, result );

	ajouter_element( ens1, 9 );

	TEST( comparer_ensemble(ens1, ens2) == 1, result );

	ajouter_element( ens1, 8 );

	TEST( comparer_ensemble(ens1, ens2) == 1, result );

	ajouter_element( ens2, 9 );

	TEST( comparer_ensemble(ens1, ens2) == 0, result );

	liberer_ensemble( ens1 );
	liberer_ensemble( ens2 );

	return result;
}

int test_copier_ensemble(){
	int result = 1;

	Ensemble * ens1 = creer_ensemble( NULL, NULL, NULL );
	Ensemble * ens2 = copier_ensemble( ens1 );

	TEST( ens1 != ens2, result );
	TEST( comparer_ensemble( ens1, ens2 ) == 0, result );

	liberer_ensemble( ens1 );
	liberer_ensemble( ens2 );

	
	ens1 = creer_ensemble( NULL, NULL, NULL );

	ajouter_element( ens1, 2 );
	ajouter_element( ens1, 1 );
	ajouter_element( ens1, 5 );

	ens2 = copier_ensemble( ens1 );

	TEST( ens1 != ens2, result );
	TEST( comparer_ensemble( ens1, ens2 ) == 0, result );

	liberer_ensemble( ens1 );
	liberer_ensemble( ens2 );



	ens1 = creer_ensemble(
		(int (*)( const intptr_t, const intptr_t)) comparer_elmt, 
		(intptr_t (*)( const intptr_t )) copier_elmt, 
		(void (*)( intptr_t )) supprimer_elmt
	);
	ens2 = copier_ensemble( ens1 );

	TEST( ens1 != ens2, result );
	TEST( comparer_ensemble( ens1, ens2 ) == 0, result );

	liberer_ensemble( ens1 );
	liberer_ensemble( ens2 );



	ens1 = creer_ensemble(
		(int (*)( const intptr_t, const intptr_t)) comparer_elmt, 
		(intptr_t (*)( const intptr_t )) copier_elmt, 
		(void (*)( intptr_t )) supprimer_elmt
	);

	Elmt elmt;

	initialiser_elmt( &elmt, 3 );
	ajouter_element( ens1, (intptr_t) &elmt );

	initialiser_elmt( &elmt, 1 );
	ajouter_element( ens1, (intptr_t) &elmt );

	initialiser_elmt( &elmt, 4 );
	ajouter_element( ens1, (intptr_t) &elmt );

	ens2 = copier_ensemble( ens1 );

	TEST( ens1 != ens2, result );
	TEST( comparer_ensemble( ens1, ens2 ) == 0, result );

	liberer_ensemble( ens1 );
	liberer_ensemble( ens2 );

	return result;
}

typedef struct {
	int min;
	int max;
	int result;
} data_t ;

void action( intptr_t element, void* data ){
	data_t * info = (data_t*) data;
	if( info->min < element && element < info->max  ){
		info->result &= 1;
	}else{
		info->result &= 0;
	}
}

int test_pour_tout_element(){
	int result = 1;

	Ensemble * ens1 = creer_ensemble( NULL, NULL, NULL );

	ajouter_element( ens1, -2 );
	ajouter_element( ens1, 3 );
	ajouter_element( ens1, 1 );

	data_t data;
	data.min = -3;
	data.max = 4;
	data.result = result;

	pour_tout_element( ens1, action, &data );

	liberer_ensemble( ens1 );

	result = data.result;
	return result;
}

int test_print_ensemble(){
	int result = 1;
	Ensemble * ens = creer_ensemble( NULL, NULL, NULL );

	print_ensemble( ens, NULL );
	printf("\n");

	ajouter_element( ens, -2 );
	ajouter_element( ens, 3 );
	ajouter_element( ens, 1 );

	print_ensemble( ens, NULL );
	printf("\n");

	liberer_ensemble( ens );

	ens = creer_ensemble(
		(int (*)( const intptr_t, const intptr_t)) comparer_elmt, 
		(intptr_t (*)( const intptr_t )) copier_elmt, 
		(void (*)( intptr_t )) supprimer_elmt
	);

	Elmt elmt;

	initialiser_elmt( &elmt, 1 );
	ajouter_element( ens, (intptr_t) &elmt );

	initialiser_elmt( &elmt, 5 );
	ajouter_element( ens, (intptr_t) &elmt );

	initialiser_elmt( &elmt, -1 );
	ajouter_element( ens, (intptr_t) &elmt );

	print_ensemble( ens, (void(*)(const intptr_t)) print_elmt );
	printf("\n");

	liberer_ensemble( ens );

	return result;
}

int test_swap_ensemble(){
	int result = 1;

	Ensemble * ens1 = creer_ensemble( NULL, NULL, NULL );

	ajouter_element( ens1, 1 );
	ajouter_element( ens1, 2 );
	ajouter_element( ens1, 5 );

	Ensemble * ens2 = creer_ensemble( NULL, NULL, NULL );

	ajouter_element( ens2, 3 );
	ajouter_element( ens2, 4 );

	swap_ensemble( ens1, ens2 );

	TEST( taille_ensemble( ens1 )==2, result );
	TEST( est_dans_l_ensemble( ens1, 3 ), result );
	TEST( est_dans_l_ensemble( ens1, 4 ), result );

	TEST( taille_ensemble( ens2 )==3, result );
	TEST( est_dans_l_ensemble( ens2, 1 ), result );
	TEST( est_dans_l_ensemble( ens2, 2 ), result );
	TEST( est_dans_l_ensemble( ens2, 5 ), result );

	liberer_ensemble( ens1 );
	liberer_ensemble( ens2 );

	return result;
}

int test_trouver_ensemble(){
	int result = 1;

	Ensemble * ens1 = creer_ensemble( NULL, NULL, NULL );

	ajouter_element( ens1, 1 );
	ajouter_element( ens1, 2 );
	ajouter_element( ens1, 5 );
	
	Ensemble_iterateur it = trouver_ensemble( ens1, 2 );
	
	TEST(  ! iterateur_ensemble_est_vide( it ), result );
	TEST(  get_element( it ) == 2, result );

	it = trouver_ensemble( ens1, -1 );
	
	TEST( iterateur_ensemble_est_vide( it ), result );

	liberer_ensemble( ens1 );

	return result;
}

int test_premier_iterateur_ensemble(){
	int result = 1;

	Ensemble * ens = creer_ensemble( NULL, NULL, NULL );

	Ensemble_iterateur it = premier_iterateur_ensemble( ens );

	TEST(
		1 
		&& iterateur_ensemble_est_vide(it) 
		, result 
	);

	ajouter_element( ens, 1 );
	ajouter_element( ens, -4 );
	ajouter_element( ens, 9 );
	ajouter_element( ens, 9 );
	ajouter_element( ens, -1 );
	ajouter_element( ens, -10 );
	ajouter_element( ens, 4 );
	ajouter_element( ens, 4 );
	ajouter_element( ens, 6 );
	ajouter_element( ens, 2 );
	
	it = premier_iterateur_ensemble( ens );
	
	TEST(
		1 
		&& ! iterateur_ensemble_est_vide(it) 
		&& get_element(it) == -10
		, result 
	);
	
	liberer_ensemble( ens );
	
	return result;
}

int test_iterateur_suivant_ensemble(){
	int result = 1;

	Ensemble * ens = creer_ensemble( NULL, NULL, NULL );

	ajouter_element( ens, 1 );
	ajouter_element( ens, -4 );
	ajouter_element( ens, 9 );
	ajouter_element( ens, 9 );
	ajouter_element( ens, -1 );
	ajouter_element( ens, -10 );
	ajouter_element( ens, 4 );
	ajouter_element( ens, 4 );
	ajouter_element( ens, 6 );
	ajouter_element( ens, 2 );
	
	Ensemble_iterateur it = premier_iterateur_ensemble( ens );
	
	TEST( 
		! iterateur_ensemble_est_vide(it) && get_element(it) == -10, result 
	);

	it = iterateur_suivant_ensemble( it );
	TEST( 
		! iterateur_ensemble_est_vide(it) && get_element(it) == -4, result 
	);

	it = iterateur_suivant_ensemble( it );
	TEST( 
		! iterateur_ensemble_est_vide(it) && get_element(it) == -1, result 
	);
	
	it = iterateur_suivant_ensemble( it );
	TEST( 
		! iterateur_ensemble_est_vide(it) && get_element(it) == 1, result 
	);
	
	it = iterateur_suivant_ensemble( it );
	TEST( 
		! iterateur_ensemble_est_vide(it) && get_element(it) == 2, result 
	);
	
	it = iterateur_suivant_ensemble( it );
	TEST( 
		! iterateur_ensemble_est_vide(it) && get_element(it) == 4, result 
	);
	
	it = iterateur_suivant_ensemble( it );
	TEST( 
		! iterateur_ensemble_est_vide(it) && get_element(it) == 6, result 
	);

	it = iterateur_suivant_ensemble( it );
	TEST( 
		! iterateur_ensemble_est_vide(it) && get_element(it) == 9, result 
	);
	
	it = iterateur_suivant_ensemble( it );
	TEST( iterateur_ensemble_est_vide(it), result );

	it = iterateur_suivant_ensemble( it );
	TEST( 
		! iterateur_ensemble_est_vide(it) && get_element(it) == -10, result 
	);
	
	liberer_ensemble( ens );
	
	return result;
}

int test_iterateur_precedent_ensemble(){
	int result = 1;

	Ensemble * ens = creer_ensemble( NULL, NULL, NULL );

	ajouter_element( ens, 1 );
	ajouter_element( ens, -4 );
	ajouter_element( ens, 9 );
	ajouter_element( ens, 9 );
	ajouter_element( ens, -1 );
	ajouter_element( ens, -10 );
	ajouter_element( ens, 4 );
	ajouter_element( ens, 4 );
	ajouter_element( ens, 6 );
	ajouter_element( ens, 2 );
	
	Ensemble_iterateur it = premier_iterateur_ensemble( ens );
	
	TEST( 
		! iterateur_ensemble_est_vide(it) && get_element(it) == -10, result 
	);

	it = iterateur_precedent_ensemble( it );
	TEST( iterateur_ensemble_est_vide(it), result );

	it = iterateur_precedent_ensemble( it );
	TEST( 
		! iterateur_ensemble_est_vide(it) && get_element(it) == 9, result 
	);
	
	it = iterateur_precedent_ensemble( it );
	TEST( 
		! iterateur_ensemble_est_vide(it) && get_element(it) == 6, result 
	);
	
	it = iterateur_precedent_ensemble( it );
	TEST( 
		! iterateur_ensemble_est_vide(it) && get_element(it) == 4, result 
	);
	
	it = iterateur_precedent_ensemble( it );
	TEST( 
		! iterateur_ensemble_est_vide(it) && get_element(it) == 2, result 
	);
	
	it = iterateur_precedent_ensemble( it );
	TEST( 
		! iterateur_ensemble_est_vide(it) && get_element(it) == 1, result 
	);

	it = iterateur_precedent_ensemble( it );
	TEST( 
		! iterateur_ensemble_est_vide(it) && get_element(it) == -1, result 
	);

	it = iterateur_precedent_ensemble( it );
	TEST( 
		! iterateur_ensemble_est_vide(it) && get_element(it) == -4, result 
	);

	it = iterateur_precedent_ensemble( it );
	TEST( 
		! iterateur_ensemble_est_vide(it) && get_element(it) == -10, result 
	);
	
	it = iterateur_precedent_ensemble( it );
	TEST( iterateur_ensemble_est_vide(it), result );

	
	liberer_ensemble( ens );

	return result;
}

int test_iterateur_ensemble_est_vide(){
	int result = 1;

	// Voir test_premier_iterateur
	return result;
}

int test_get_element(){
	int result = 1;

	//Voir test_trouver_element();
	return result;
}


int main(){
	int result = 1;

	result &= general_tests();
	result &= test_creer_ensemble();
	result &= test_liberer_ensemble();
	result &= test_ajouter_element();
	result &= test_ajouter_elements();
	result &= test_retirer_element();
	result &= test_retirer_elements();
	result &= test_vider_ensemble();
	result &= test_est_dans_l_ensemble();
	result &= test_taille_ensemble();
	result &= test_comparer_ensemble();
	result &= test_copier_ensemble();
	result &= test_pour_tout_element();
	result &= test_print_ensemble();
	result &= test_swap_ensemble();
	result &= test_trouver_ensemble();
	result &= test_premier_iterateur_ensemble();
	result &= test_iterateur_suivant_ensemble();
	result &= test_iterateur_precedent_ensemble();
	result &= test_iterateur_ensemble_est_vide();
	result &= test_get_element();

	if( ! result ){
		fprintf( stderr, "Certains tests du fichier %s ont échoués.\n", __FILE__ );
		return 1;
	}

	return 0;
}

