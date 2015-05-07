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


#include "table.h"
#include "outils.h"

#include <stdarg.h>

#include "ensemble.h"

typedef struct {
	int cle;
} Cle;

int comparer_cle( const Cle* a, const Cle * b){
	return a->cle - b->cle;
}

void initialiser_cle( Cle* v, int cle ){
	v->cle = cle;
}

void print_cle( const Cle * c ){
	printf( "cle : %d", c->cle );
}

Cle * creer_cle( int cle ){
	Cle * res = xmalloc(sizeof(Cle));
	initialiser_cle( res, cle );
	return res;
}

void supprimer_cle( Cle* cle ){
	xfree( cle );
}


typedef struct {
	int valeur;
} Valeur;

void initialiser_valeur( Valeur* v, int valeur ){
	v->valeur = valeur;
}

Valeur * creer_valeur( int valeur ){
	Valeur * res = xmalloc(sizeof(Valeur));
	initialiser_valeur( res, valeur );
	return res;
}

void supprimer_valeur( Valeur* valeur ){
	xfree( valeur );
}

void print_valeur( const Valeur * v ){
	printf( "valeur : %d", v->valeur );
}



Cle * copier_cle( const Cle * cle ) {
	return creer_cle( cle->cle );
};


int general_test(){
	int result = 1;
	// Tester la création d'une table dont les clés sont des entiers
	Table * table = creer_table( NULL, NULL, NULL );

	add_table( table, 1,11);	
	TEST( get_cle( trouver_table( table, 1 ) ) == 1, result );
	TEST( get_valeur( trouver_table( table, 1 ) ) == 11, result );

	add_table( table, 1,21);	
	TEST( get_cle( trouver_table( table, 1 ) ) == 1, result );
	TEST( get_valeur( trouver_table( table, 1 ) ) == 21, result );

	add_table( table, 5,12);	
	TEST( get_cle( trouver_table( table, 5 ) ) == 5, result );
	TEST( get_valeur( trouver_table( table, 5 ) ) == 12, result );

	add_table( table, 2,12);	
	TEST( get_cle( trouver_table( table, 2 ) ) == 2, result );
	TEST( get_valeur( trouver_table( table, 2 ) ) == 12, result );

	add_table( table, 2,13);	
	TEST( get_cle( trouver_table( table, 2 ) ) == 2, result );
	TEST( get_valeur( trouver_table( table, 2 ) ) == 13, result );

	delete_table( table, 2 );

	TEST( iterateur_est_vide( trouver_table(table, 2) ), result );
	TEST( ! iterateur_est_vide( trouver_table(table, 5) ), result );
	TEST( ! iterateur_est_vide( trouver_table(table, 1) ), result );
	
	TEST( get_cle( trouver_table(table, 5) )==5, result );
	TEST( get_valeur( trouver_table(table, 5) )==12, result );

	TEST( get_cle( trouver_table(table, 1) )==1, result );
	TEST( get_valeur( trouver_table(table, 1) )==21, result );

	add_table( table, -1,-11);	
	TEST( get_cle( trouver_table( table, -1 ) ) == -1, result );
	TEST( get_valeur( trouver_table( table, -1 ) ) == -11, result );
	
	liberer_table( table );
	
	// Tester la création d'une table dont les clés sont des entiers
	table = creer_table( 
		(int (*)( const intptr_t, const intptr_t )) comparer_cle, 
		(intptr_t (*)( const intptr_t )) copier_cle, 
		(void (*)(intptr_t)) supprimer_cle 
	);

	Cle cle;
	initialiser_cle( &cle, 1 ); 	
	add_table( table, (intptr_t) &cle, 21);	
	TEST( ((Cle*) get_cle( trouver_table( table, (intptr_t) &cle ) ))->cle == 1, result );
	TEST( get_valeur( trouver_table( table, (intptr_t) &cle ) ) == 21, result );

	initialiser_cle( &cle, 2 ); 	
	add_table( table, (intptr_t) &cle, 22);	
	TEST( ((Cle*) get_cle( trouver_table( table, (intptr_t) &cle ) ))->cle == 2, result );
	TEST( get_valeur( trouver_table( table, (intptr_t) &cle ) ) == 22, result );

	initialiser_cle( &cle, 5 ); 	
	add_table( table, (intptr_t) &cle, 25);	
	TEST( ((Cle*) get_cle( trouver_table( table, (intptr_t) &cle ) ))->cle == 5, result );
	TEST( get_valeur( trouver_table( table, (intptr_t) &cle ) ) == 25, result );

	initialiser_cle( &cle, 2 ); 	
	add_table( table, (intptr_t) &cle, 23);	
	TEST( ((Cle*) get_cle( trouver_table( table, (intptr_t) &cle ) ))->cle == 2, result );
	TEST( get_valeur( trouver_table( table, (intptr_t) &cle ) ) == 23, result );

	initialiser_cle( &cle, 1 ); 	
	add_table( table, (intptr_t) &cle, 81);	
	TEST( ((Cle*) get_cle( trouver_table( table, (intptr_t) &cle ) ))->cle == 1, result );
	TEST( get_valeur( trouver_table( table, (intptr_t) &cle ) ) == 81, result );


	initialiser_cle( &cle, 2 ); 	
	delete_table( table, (intptr_t) &cle );

	initialiser_cle( &cle, 2 ); 	
	TEST( iterateur_est_vide( trouver_table(table, (intptr_t) &cle) ), result );
	initialiser_cle( &cle, 5 ); 	
	TEST( ! iterateur_est_vide( trouver_table(table, (intptr_t) &cle) ), result );
	initialiser_cle( &cle, 1 ); 	
	TEST( ! iterateur_est_vide( trouver_table(table, (intptr_t) &cle) ), result );

	initialiser_cle( &cle, -3 ); 	
	add_table( table, (intptr_t) &cle, -23);	
	TEST( ((Cle*) get_cle( trouver_table( table, (intptr_t) &cle ) ))->cle == -3, result );
	TEST( get_valeur( trouver_table( table, (intptr_t) &cle ) ) == -23, result );
	
	liberer_table( table );
	return result;
}

int test_creer_table(){
	int result = 1;
	// Tester la création d'une table dont les clés sont des entiers
	Table * table = creer_table( NULL, NULL, NULL );
	
	TEST( table!=NULL, result );
	
	liberer_table( table );
	
	// Tester la création d'une table dont les clés sont des entiers
	table = creer_table( 
		(int (*)( const intptr_t, const intptr_t )) comparer_cle, 
		(intptr_t (*)( const intptr_t )) copier_cle, 
		(void (*)(intptr_t)) supprimer_cle 
	);
	
	TEST( table!=NULL, result );
	
	liberer_table( table );

	return result;
}

int test_liberer_table(){
	// Voir general_test et creer_test
	return 1;
}

int test_add_table(){
	// Voir general tests
	return 1;
}

int test_delete_table(){
	// Voir general_test
	return 1;
}

int test_vider_table(){
	int result = 1;

	Table * table = creer_table( NULL, NULL, NULL );

	add_table( table, 1,11);	
	add_table( table, 1,21);	
	add_table( table, 5,12);	
	add_table( table, 2,12);	
	add_table( table, 2,13);	
	delete_table( table, 2 );
	add_table( table, -1,-11);	

	vider_table( table );

	TEST( iterateur_est_vide( trouver_table( table, 1 ) ), result );
	TEST( iterateur_est_vide( trouver_table( table, 5 ) ), result );
	TEST( iterateur_est_vide( trouver_table( table, 2 ) ), result );
	TEST( iterateur_est_vide( trouver_table( table, -1 ) ) , result );
	
	liberer_table( table );
	
	// Tester la création d'une table dont les clés sont des entiers
	table = creer_table( 
		(int (*)( const intptr_t, const intptr_t )) comparer_cle, 
		(intptr_t (*)( const intptr_t )) copier_cle, 
		(void (*)(intptr_t)) supprimer_cle 
	);

	Cle cle;
	initialiser_cle( &cle, 1 ); 	
	add_table( table, (intptr_t) &cle, 21);	

	initialiser_cle( &cle, 2 ); 	
	add_table( table, (intptr_t) &cle, 22);	

	initialiser_cle( &cle, 5 ); 	
	add_table( table, (intptr_t) &cle, 25);	

	initialiser_cle( &cle, 2 ); 	
	add_table( table, (intptr_t) &cle, 22);	

	initialiser_cle( &cle, 1 ); 	
	add_table( table, (intptr_t) &cle, 81);	

	delete_table( table, (intptr_t) &cle );

	initialiser_cle( &cle, -3 ); 	
	add_table( table, (intptr_t) &cle, -23);	

	vider_table( table );

	initialiser_cle( &cle, 1 ); 	
	TEST( iterateur_est_vide( trouver_table( table, (intptr_t) &cle ) ), result );
	initialiser_cle( &cle, 5 ); 	
	TEST( iterateur_est_vide( trouver_table( table, (intptr_t) &cle ) ), result );
	initialiser_cle( &cle, 2 ); 	
	TEST( iterateur_est_vide( trouver_table( table, (intptr_t) &cle ) ), result );
	initialiser_cle( &cle, -1 ); 	
	TEST( iterateur_est_vide( trouver_table( table, (intptr_t) &cle ) ), result );
	initialiser_cle( &cle, -3 ); 	
	TEST( iterateur_est_vide( trouver_table( table, (intptr_t) &cle ) ), result );
	
	liberer_table( table );

	return result;
}

int test_print_table(){
	int result = 1;

	Table * table = creer_table( NULL, NULL, NULL );

	print_table( table, NULL, NULL, "\n" );

	liberer_table( table );

	table = creer_table( NULL, NULL, NULL );

	add_table( table, 1,11);	
	add_table( table, 1,21);	
	add_table( table, 5,12);	
	add_table( table, 2,12);	
	add_table( table, 2,13);	

	print_table( table, NULL, NULL, "\n" );

	liberer_table( table );

	table = creer_table( 
		(int (*)( const intptr_t, const intptr_t )) comparer_cle, 
		(intptr_t (*)( const intptr_t )) copier_cle, 
		(void (*)(intptr_t)) supprimer_cle 
	);

	Cle cle;
	initialiser_cle( &cle, 1 );
	add_table( table, (intptr_t) &cle, (intptr_t) creer_valeur(11) );

	initialiser_cle( &cle, 2 );
	add_table( table, (intptr_t) &cle, (intptr_t) creer_valeur(12) );

	initialiser_cle( &cle, -1 );
	add_table( table, (intptr_t) &cle, (intptr_t) creer_valeur(-11) );

	initialiser_cle( &cle, 5 );
	add_table( table, (intptr_t) &cle, (intptr_t) creer_valeur(15) );

	print_table(
		table, 
		(void(*)(const intptr_t)) print_cle, 
		(void(*)(const intptr_t)) print_valeur, 
		"\n"
	);

	pour_toute_valeur_table( table, ( void(*)(intptr_t) )supprimer_valeur );

	liberer_table( table );


	return result;
}

int test_pour_toute_valeur_table(){
	Table * table = creer_table( NULL, NULL, NULL );

	add_table( table, 1, (intptr_t) creer_valeur(1) );	
	add_table( table, 2, (intptr_t) creer_valeur(2) );	
	add_table( table, 3, (intptr_t) creer_valeur(3) );	
	add_table( table, 5, (intptr_t) creer_valeur(5) );	
	
	pour_toute_valeur_table( table, ( void(*)(intptr_t) )supprimer_valeur );

	liberer_table( table );

	return 1;
}

void action( const intptr_t cle, intptr_t valeur, void  * data ){
	if( cle+10 != ((Valeur*)valeur)->valeur  ) (*(int*) data) = 0;
}

int test_pour_toute_cle_valeur_table(){
	int result = 1;

	Table * table = creer_table( NULL, NULL, NULL );

	add_table( table, 1, (intptr_t) creer_valeur(11) );	
	add_table( table, 2, (intptr_t) creer_valeur(12) );	
	add_table( table, 3, (intptr_t) creer_valeur(13) );	
	add_table( table, 5, (intptr_t) creer_valeur(15) );	
	
	pour_toute_cle_valeur_table( table, action, &result );

	pour_toute_valeur_table( table, ( void(*)(intptr_t) ) supprimer_valeur );

	liberer_table( table );

	return result;
}

int test_trouver_table(){
	int result = 1;
	Table * table = creer_table( NULL, NULL, NULL );

	add_table( table, 1, 1 );
	add_table( table, 3, 3 );
	add_table( table, 1, -1 );

	TEST( get_cle( trouver_table( table, 3 ) ) == 3, result );
	TEST( get_cle( trouver_table( table, 1 ) ) == 1, result );
	TEST( get_valeur( trouver_table( table, 1 ) ) == -1, result );

	liberer_table( table );

	// Voir general_test;
	return result;
}

int test_get_cle(){
	// Voir general_test
	return 1;
}

int test_get_valeur(){
	// Voir general_test
	return 1;
}


int main(){

	int result = 1;

	result &= general_test();
	result &= test_creer_table();
	result &= test_liberer_table();
	result &= test_add_table();
	result &= test_delete_table();
	result &= test_vider_table();
	result &= test_print_table();
	result &= test_pour_toute_valeur_table();
	result &= test_pour_toute_cle_valeur_table();
	result &= test_trouver_table();
	result &= test_get_cle();
	result &= test_get_valeur();

	if( ! result ){
		fprintf( stderr, "Certains tests du fichier %s ont échoués.\n", __FILE__ );
		return 1;
	}

	return 0;
}
