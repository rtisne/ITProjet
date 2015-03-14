/*
 *   Ce fichier fait partie d'un projet de programmation donné en Licence 3 
 *   à l'Université de Bordeaux.
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

#include "table.h"
#include "outils.h"
#include "fifo.h"
#include "avl.h"

#include <assert.h>

#include <search.h>
#include <stdlib.h>

typedef struct Table_association {
	void (*supprimer_cle)(intptr_t cle);
	intptr_t (*copier_cle)(const intptr_t cle);
	int (*comparer_cle)(const intptr_t cle1, const intptr_t cle2);
	intptr_t cle;
	intptr_t valeur;
} Table_association ;

struct Table {
	int (*comparer_cle)( const intptr_t cle1, const intptr_t cle2 );
	intptr_t (*copier_cle)( const intptr_t cle );
	void (*supprimer_cle)(intptr_t cle);
	struct avl_table * root;
};


intptr_t get_cle( Table_iterateur it ){
	const Table_association * asso = ( const Table_association * ) avl_t_cur( &it );
	return (const intptr_t) asso->cle;
}

intptr_t get_valeur( Table_iterateur it ){
	Table_association * asso = ( Table_association * ) avl_t_cur( &it );
	return asso->valeur;
}

Table_association * creer_table_association(
	const Table* table, const intptr_t cle, intptr_t valeur
){
	Table_association * res = xmalloc(
		sizeof( Table_association )
	);
	if( table->copier_cle && cle ){
		res->cle = table->copier_cle( cle );
	}else{
		res->cle = cle;
	}
	res->valeur = valeur;
	res->supprimer_cle = table->supprimer_cle;
	res->copier_cle = table->copier_cle;
	res->comparer_cle = table->comparer_cle;
	return res;
}

Table_association * copier_table_association( Table_association * asso ){
	Table_association * res = xmalloc(
		sizeof( Table_association )
	);
	*res = *asso;
	if( asso->copier_cle && asso->cle ){
		res->cle = asso->copier_cle( asso->cle );
	}
	return res;
}

int compare_table_association( const void * pa1, const void * pb1, void* param ){
	Table_association * pa = (Table_association *) pa1;
	Table_association * pb = (Table_association *) pb1;
	if( pa->comparer_cle ){
		int r = pa->comparer_cle( pa->cle, pb->cle );
		return r;
	}else{
		if( pa->cle < pb->cle )
			return -1;
		if( pa->cle > pb->cle )
			return 1;
		return 0;
	}
}


void supprimer_table_association2( void* asso_tmp, void* data ){
	Table_association * asso = (Table_association*) asso_tmp;
	if( asso->supprimer_cle && asso->cle ){
		asso->supprimer_cle( asso->cle );
	}
	xfree(asso);
}

void supprimer_table_association( Table_association * asso ){
	if( asso->supprimer_cle && asso->cle ){
		asso->supprimer_cle( asso->cle );
	}
	xfree(asso);
}

Table* creer_table(
	int (*comparer_cle)( const intptr_t cle1, const intptr_t cle2 ),
	intptr_t (*copier_cle)( const intptr_t cle ),
	void (*supprimer_cle)(intptr_t cle)
){
	Table* res = xmalloc( sizeof(Table) );
	res->root = avl_create ( compare_table_association, NULL, NULL );

	res->supprimer_cle = supprimer_cle;
	res->comparer_cle = comparer_cle;
	res->copier_cle = copier_cle;
	return res;
}

void liberer_table( Table* table ){
	assert( table );
	avl_destroy ( table->root, supprimer_table_association2 );
	xfree( table );
}

void add_table( Table* table, const intptr_t cle, intptr_t valeur ) {
	Table_association* asso = creer_table_association(table, cle, valeur);
	void* val = avl_probe ( table->root, (void*) asso );
	if( val == NULL ){
		ERREUR( "Espace insuffisant" );
	}
	Table_association* asso_tree = *( Table_association** ) val; 
	if( asso_tree != asso  ){
		supprimer_table_association( asso );
		asso_tree->valeur = valeur;
	}
}

intptr_t delete_table( Table* table, intptr_t cle ){
	intptr_t valeur = (intptr_t) NULL;
	Table_association* asso_tree = NULL;
	Table_association* asso = creer_table_association(
		table, cle, (intptr_t) NULL
	);
	void* val = avl_find( table->root, (void*) asso );
	if( val ){
		asso_tree = ( Table_association* ) val; 
		valeur = asso_tree->valeur;
	}
	avl_delete( table->root, (void*) asso );
	if(asso_tree){
		supprimer_table_association( asso_tree );
	}
	supprimer_table_association( asso );
	return valeur;
}

void pour_toute_cle_valeur_table(
	const Table* table,
	void (* action)( const intptr_t cle, intptr_t valeur, void* data  ),
	void* data
){
	struct avl_traverser traverser;
	void * item;
	avl_t_init( &traverser, table->root );
	while( (item = avl_t_next( &traverser )) ){
		Table_association* asso = (Table_association *) item;
		action( asso->cle, asso->valeur, data );
	}
}

typedef struct {
	void (* supprimer_valeur)(intptr_t valeur );
} data_pour_toute_valeur_table_t;

void action_pour_toute_valeur_table( const intptr_t cle, intptr_t valeur, void* data  ){
	data_pour_toute_valeur_table_t * d = (data_pour_toute_valeur_table_t*) data;
	d->supprimer_valeur( valeur );
}

void pour_toute_valeur_table(
	const Table* table,
	void (* supprimer_valeur)(intptr_t valeur )
){
	data_pour_toute_valeur_table_t data;
	data.supprimer_valeur = supprimer_valeur;
	pour_toute_cle_valeur_table( table, action_pour_toute_valeur_table, &data );
}

void vider_table( Table* table ){
	avl_destroy ( table->root, supprimer_table_association2 );
	table->root = avl_create ( compare_table_association, NULL, NULL );
}

typedef struct {
	void (*print_cle)( const intptr_t cle );
	void (*print_valeur)( const intptr_t valeur );
} data_print_table_t;

void print_association( const intptr_t cle, intptr_t valeur, void* data  ){
	data_print_table_t* d = (data_print_table_t*) data;
	if( d->print_cle ){
		d->print_cle( cle );
	}else{
		printf("%ld", cle );
	}
	printf(" --> ");
	if( d->print_valeur ){
		d->print_valeur( valeur );
	}else{
		printf("%ld", valeur );
	}
	printf(", ");
}

void print_table(
	Table* table, void (*print_cle)( const intptr_t cle ), 
	void (*print_valeur)( const intptr_t valeur ),
	const char* texte_de_fin
){
	data_print_table_t data;
	data.print_cle = print_cle;
	data.print_valeur = print_valeur;
	
	printf( "{ ");
	pour_toute_cle_valeur_table( table, print_association, &data );
	printf( " }%s", texte_de_fin );
}

Table_iterateur trouver_table( const Table* table, intptr_t cle ){
	Table_iterateur it;
	Table_association* asso = creer_table_association(
		table, cle, (intptr_t) NULL
	);
	avl_t_find( &it, table->root, (void*) asso );
	supprimer_table_association( asso );
	return it;
}

Table_iterateur premier_iterateur_table( const Table* table ){
	Table_iterateur it;
	avl_t_first( &it, table->root );
	return it;
}

Table_iterateur dernier_iterateur_table(
	const Table_iterateur * iterator, Table* table 
){
	Table_iterateur it;
	avl_t_last( &it, table->root );
	return it;
}

int iterateur_est_vide( Table_iterateur iterator ){
	return avl_t_is_null( &iterator );	
}

Table_iterateur iterateur_suivant_table( Table_iterateur iterateur ){
	avl_t_next( &iterateur );
	return iterateur;
}

Table_iterateur iterateur_precedent_table( Table_iterateur iterateur ){
	avl_t_prev( &iterateur );
	return iterateur;
}

int taille_table( Table* t ){
	int res = 0;
	Table_iterateur it1;
	for(
		it1 = premier_iterateur_table( t );
		! iterateur_est_vide( it1 );
		it1 = iterateur_suivant_table( it1 )
	){
		res += 1;
	}
	return res;
};
