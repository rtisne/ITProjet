/*
 *   Ce fichier fait partie d'un projet de programmation donné en Licence 3 
 *   à l'Université de Bordeaux
 *
 *   Copyright (C) 2014 Adrien Boussicault
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


#define _GNU_SOURCE

#include "ensemble.h"
#include "outils.h"
#include "table.h"

#include <stdlib.h>
#include <stdio.h>


int* allouer_element( int val ){
	int* result = (int*) xmalloc( sizeof(int) );
	(*result) = val;
	return result;
}

void supprimer_element( int* element ){
	xfree( element );
}

void next_iterators( Table_iterateur * it1, Table_iterateur * it2 ){
	*it1 = iterateur_suivant_table(*it1);
	*it2 = iterateur_suivant_table(*it2);
}

int comparer_ensemble( const Ensemble* ens1, const Ensemble*  ens2 ){
	Table_iterateur it1, it2;
	
	it1 = premier_iterateur_table( ens1->table );
	it2 = premier_iterateur_table( ens2->table );
	for( 
		;
		( ! iterateur_est_vide(it1) ) && ( ! iterateur_est_vide(it2) );
		next_iterators( &it1, &it2 )
	){
		int cmp;
		if( ens1->comparer_element ){
			cmp = ens1->comparer_element( get_cle( it1 ), get_cle( it2 ) );
		}else{
			cmp = get_cle( it1 ) -  get_cle( it2 );
		}
	 	if( cmp > 0 ) return 1;
	 	if( cmp < 0 ) return -1;
	}
	if( iterateur_est_vide(it1) && iterateur_est_vide(it2) )
		return 0;
	if( iterateur_est_vide(it1) ) 
		return -1;
	return 1;
}


Ensemble * creer_ensemble(
	int (*comparer_element)( const intptr_t elem1, const intptr_t elem2 ),
	intptr_t (*copier_element)( const intptr_t elem ),
	void (*supprimer_element)(intptr_t elem )
){
	Ensemble * result = (Ensemble*) xmalloc( sizeof(Ensemble) );
	result->table = creer_table(
		comparer_element, copier_element, supprimer_element
	);
	result->comparer_element = comparer_element;
	result->copier_element = copier_element;
	result->supprimer_element = supprimer_element;
	return result;
}

void liberer_ensemble( Ensemble * ens ){
	if(ens){
		liberer_table( ens->table );
		xfree( ens );
	}
}

void ajouter_element( Ensemble * ensemble, const intptr_t element ){
	add_table( ensemble->table, element, (intptr_t) NULL );
}


void action_ajouter_element( const intptr_t element, void* ens ){
	ajouter_element( (Ensemble*) ens, element );
}

void ajouter_elements( Ensemble * ens1, const Ensemble * ens2 ){
	pour_tout_element( ens2, action_ajouter_element, ens1 );
}

void retirer_element( Ensemble * ensemble, const intptr_t element ){
	delete_table( ensemble->table, element );
}

void action_retirer_elements( const intptr_t element, void* ens ){
	retirer_element( (Ensemble*) ens, element );
}

void retirer_elements( Ensemble * ens1, const Ensemble * ens2 ){
	pour_tout_element( ens2, action_retirer_elements, ens1 );
}

void vider_ensemble( Ensemble * ensemble ){
	vider_table( ensemble->table );
}

int est_dans_l_ensemble( const Ensemble * ensemble, intptr_t element ){
	Table_iterateur it = trouver_table( ensemble->table, element );
	return ! avl_t_is_null( &it ); 
}

void action_taille_ensemble( const intptr_t element, void* taille ){
	(*(int*) taille)+=1;
}

unsigned int taille_ensemble( const Ensemble* ensemble ){
	int taille = 0;
	pour_tout_element( ensemble, action_taille_ensemble, &taille );
	return taille;
}

typedef struct {
	void (*print_element)( const intptr_t cle ); 
} data_print_ensemble;

void action_print_ensemble( const intptr_t element, void* data ){
	data_print_ensemble* info = ( data_print_ensemble* ) data;
	if( info->print_element ){
		info->print_element( element );
	}else{
		printf( "%ld", element );
	}
	printf( ", " );
}

void print_ensemble(
	const Ensemble* ens,
	void (*print_element)( const intptr_t cle ) 
){
	data_print_ensemble data;
	data.print_element = print_element;

	printf( "{" );
	pour_tout_element( ens, action_print_ensemble, &data );
	printf( "}" );
}


typedef struct {
	void (* action )( const intptr_t element, void* data );
	void* data;
} data_pour_tout_element_t;

void action_pour_tout_element_ensemble( const intptr_t cle, intptr_t valeur, void* data  ){
	data_pour_tout_element_t * info = (data_pour_tout_element_t*) data;
	info->action( cle, info->data );
}

void pour_tout_element(
	const Ensemble* ensemble, 
	void (* action )( const intptr_t element, void* data ),
	void* data
){
	data_pour_tout_element_t data1;
	data1.action = action;
	data1.data = data;
	pour_toute_cle_valeur_table(
		ensemble->table, action_pour_tout_element_ensemble, &data1
	);
}

void swap_ensemble( Ensemble* ens1, Ensemble* ens2 ){
	void* tmp = ens1->table;
	ens1->table = ens2->table;
	ens2->table = tmp;
}
void deplacer_ensemble( Ensemble* ens1, Ensemble* ens2 ){
	swap_ensemble( ens1, ens2 );
	liberer_ensemble( ens2 );
}

Ensemble* copier_ensemble( const Ensemble* ensemble ){
	Ensemble* res = creer_ensemble(
		ensemble->comparer_element, ensemble->copier_element,
		ensemble->supprimer_element
	);
	ajouter_elements( res, ensemble  );
	return res;
}

Ensemble * creer_union_ensemble( const Ensemble* ens1, const Ensemble* ens2 ){
	Ensemble * res = copier_ensemble( ens1 );
	ajouter_elements( res, ens2 );
	return res;
}

Ensemble * creer_difference_ensemble(
	const Ensemble* ens1, const Ensemble* ens2
){
	Ensemble * res = copier_ensemble( ens1 );
	retirer_elements( res, ens2 );
	return res;
}

Ensemble * creer_intersection_ensemble(
	const Ensemble* ens1, const Ensemble* ens2
){
	Ensemble *tmp, *res;
	tmp = creer_difference_ensemble( ens1, ens2 );
	res = creer_difference_ensemble( ens1, tmp );
	liberer_ensemble( tmp );
	return res;
}

Ensemble_iterateur trouver_ensemble(
	const Ensemble* ensemble, const intptr_t element
){
	return trouver_table( ensemble->table, element );
}

Ensemble_iterateur premier_iterateur_ensemble( const Ensemble* ensemble ){
	return premier_iterateur_table( ensemble->table );
}

Ensemble_iterateur iterateur_suivant_ensemble(
	const Ensemble_iterateur iterateur
){
	return iterateur_suivant_table( iterateur );
}

Ensemble_iterateur iterateur_precedent_ensemble( Ensemble_iterateur iterateur ){
	return iterateur_precedent_table( iterateur );
}

int iterateur_ensemble_est_vide( Ensemble_iterateur iterateur ){
	return iterateur_est_vide( iterateur );
}

intptr_t get_element( Ensemble_iterateur it ){
	return get_cle( it );
}
