/*
 *   Ce fichier fait partie d'un projet de programmation donné en Licence 3
 *   à l'Université de Bordeaux
 *
 *   Copyright (C) 2014, 2015 Adrien Boussicault
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
#include "table.h"
#include "ensemble.h"
#include "outils.h"
#include "fifo.h"

#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include <assert.h>

#include <math.h>

void action_get_max_etat( const intptr_t element, void* data ){
	int * max = (int*) data;
	if( *max < element ) *max = element;
}

int get_max_etat( const Automate* automate ){
	int max = INT_MIN;

	pour_tout_element( automate->etats, action_get_max_etat, &max );

	return max;
}

void action_get_min_etat( const intptr_t element, void* data ){
	int * min = (int*) data;
	if( *min > element ) *min = element;
}

int get_min_etat( const Automate* automate ){
	int min = INT_MAX;

	pour_tout_element( automate->etats, action_get_min_etat, &min );

	return min;
}

int comparer_cle(const Cle *a, const Cle *b) {
	if( a->origine < b->origine )
		return -1;
	if( a->origine > b->origine )
		return 1;
	if( a->lettre < b->lettre )
		return -1;
	if( a->lettre > b->lettre )
		return 1;
	return 0;
}

void print_cle( const Cle * a){
	printf( "(%d, %c)" , a->origine, (char) (a->lettre) );
}

void supprimer_cle( Cle* cle ){
	xfree( cle );
}

void initialiser_cle( Cle* cle, int origine, char lettre ){
	cle->origine = origine;
	cle->lettre = (int) lettre;
}

Cle * creer_cle( int origine, char lettre ){
	Cle * result = xmalloc( sizeof(Cle) );
	initialiser_cle( result, origine, lettre );
	return result;
}

Cle * copier_cle( const Cle* cle ){
	return creer_cle( cle->origine, cle->lettre );
}

Automate * creer_automate(){
	Automate * automate = xmalloc( sizeof(Automate) );
	automate->etats = creer_ensemble( NULL, NULL, NULL );
	automate->alphabet = creer_ensemble( NULL, NULL, NULL );
	automate->transitions = creer_table(
		( int(*)(const intptr_t, const intptr_t) ) comparer_cle ,
		( intptr_t (*)( const intptr_t ) ) copier_cle,
		( void(*)(intptr_t) ) supprimer_cle
	);
	automate->initiaux = creer_ensemble( NULL, NULL, NULL );
	automate->finaux = creer_ensemble( NULL, NULL, NULL );
	automate->vide = creer_ensemble( NULL, NULL, NULL );
	return automate;
}

void liberer_automate( Automate * automate ){
	assert( automate );
	liberer_ensemble( automate->vide );
	liberer_ensemble( automate->finaux );
	liberer_ensemble( automate->initiaux );
	pour_toute_valeur_table(
		automate->transitions, ( void(*)(intptr_t) ) liberer_ensemble
	);
	liberer_table( automate->transitions );
	liberer_ensemble( automate->alphabet );
	liberer_ensemble( automate->etats );
	xfree(automate);
}

const Ensemble * get_etats( const Automate* automate ){
	return automate->etats;
}

const Ensemble * get_initiaux( const Automate* automate ){
	return automate->initiaux;
}

const Ensemble * get_finaux( const Automate* automate ){
	return automate->finaux;
}

const Ensemble * get_alphabet( const Automate* automate ){
	return automate->alphabet;
}

void ajouter_etat( Automate * automate, int etat ){
	ajouter_element( automate->etats, etat );
}

void ajouter_lettre( Automate * automate, char lettre ){
	ajouter_element( automate->alphabet, lettre );
}

void ajouter_transition(
	Automate * automate, int origine, char lettre, int fin
){
	ajouter_etat( automate, origine );
	ajouter_etat( automate, fin );
	ajouter_lettre( automate, lettre );

	Cle cle;
	initialiser_cle( &cle, origine, lettre );
	Table_iterateur it = trouver_table( automate->transitions, (intptr_t) &cle );
	Ensemble * ens;
	if( iterateur_est_vide( it ) ){
		ens = creer_ensemble( NULL, NULL, NULL );
		add_table( automate->transitions, (intptr_t) &cle, (intptr_t) ens );
	}else{
		ens = (Ensemble*) get_valeur( it );
	}
	ajouter_element( ens, fin );
}

void ajouter_etat_final(
	Automate * automate, int etat_final
){
	ajouter_etat( automate, etat_final );
	ajouter_element( automate->finaux, etat_final );
}

void ajouter_etat_initial(
	Automate * automate, int etat_initial
){
	ajouter_etat( automate, etat_initial );
	ajouter_element( automate->initiaux, etat_initial );
}

const Ensemble * voisins( const Automate* automate, int origine, char lettre ){
	Cle cle;
	initialiser_cle( &cle, origine, lettre );
	Table_iterateur it = trouver_table( automate->transitions, (intptr_t) &cle );
	if( ! iterateur_est_vide( it ) ){
		return (Ensemble*) get_valeur( it );
	}else{
		return automate->vide;
	}
}

Ensemble * delta(
	const Automate* automate, const Ensemble * etats_courants, char lettre
){
	Ensemble * res = creer_ensemble( NULL, NULL, NULL );

	Ensemble_iterateur it;
	for(
		it = premier_iterateur_ensemble( etats_courants );
		! iterateur_ensemble_est_vide( it );
		it = iterateur_suivant_ensemble( it )
	){
		const Ensemble * fins = voisins(
			automate, get_element( it ), lettre
		);
		ajouter_elements( res, fins );
	}

	return res;
}

Ensemble * delta_star(
	const Automate* automate, const Ensemble * etats_courants, const char* mot
){
	int len = strlen( mot );
	int i;
	Ensemble * old = copier_ensemble( etats_courants );
	Ensemble * new = old;
	for( i=0; i<len; i++ ){
		new = delta( automate, old, *(mot+i) );
		liberer_ensemble( old );
		old = new;
	}
	return new;
}

void pour_toute_transition(
	const Automate* automate,
	void (* action )( int origine, char lettre, int fin, void* data ),
	void* data
){
	Table_iterateur it1;
	Ensemble_iterateur it2;
	for(
		it1 = premier_iterateur_table( automate->transitions );
		! iterateur_est_vide( it1 );
		it1 = iterateur_suivant_table( it1 )
	){
		Cle * cle = (Cle*) get_cle( it1 );
		Ensemble * fins = (Ensemble*) get_valeur( it1 );
		for(
			it2 = premier_iterateur_ensemble( fins );
			! iterateur_ensemble_est_vide( it2 );
			it2 = iterateur_suivant_ensemble( it2 )
		){
			int fin = get_element( it2 );
			action( cle->origine, cle->lettre, fin, data );
		}
	};
}

Automate* copier_automate( const Automate* automate ){
	Automate * res = creer_automate();
	Ensemble_iterateur it1;
	// On ajoute les états de l'automate
	for(
		it1 = premier_iterateur_ensemble( get_etats( automate ) );
		! iterateur_ensemble_est_vide( it1 );
		it1 = iterateur_suivant_ensemble( it1 )
	){
		ajouter_etat( res, get_element( it1 ) );
	}
	// On ajoute les états initiaux
	for(
		it1 = premier_iterateur_ensemble( get_initiaux( automate ) );
		! iterateur_ensemble_est_vide( it1 );
		it1 = iterateur_suivant_ensemble( it1 )
	){
		ajouter_etat_initial( res, get_element( it1 ) );
	}
	// On ajoute les états finaux
	for(
		it1 = premier_iterateur_ensemble( get_finaux( automate ) );
		! iterateur_ensemble_est_vide( it1 );
		it1 = iterateur_suivant_ensemble( it1 )
	){
		ajouter_etat_final( res, get_element( it1 ) );
	}
	// On ajoute les lettres
	for(
		it1 = premier_iterateur_ensemble( get_alphabet( automate ) );
		! iterateur_ensemble_est_vide( it1 );
		it1 = iterateur_suivant_ensemble( it1 )
	){
		ajouter_lettre( res, (char) get_element( it1 ) );
	}
	// On ajoute les transitions
	Table_iterateur it2;
	for(
		it2 = premier_iterateur_table( automate->transitions );
		! iterateur_est_vide( it2 );
		it2 = iterateur_suivant_table( it2 )
	){
		Cle * cle = (Cle*) get_cle( it2 );
		Ensemble * fins = (Ensemble*) get_valeur( it2 );
		for(
			it1 = premier_iterateur_ensemble( fins );
			! iterateur_ensemble_est_vide( it1 );
			it1 = iterateur_suivant_ensemble( it1 )
		){
			int fin = get_element( it1 );
			ajouter_transition( res, cle->origine, cle->lettre, fin );
		}
	}
	return res;
}

/*
 *                         x = 0
 *                          |
 *                          V
 *
 * 283 237 195 157 123  93  67  91 119 151 187 227 271
 * 238 196 158 124  94  68  46  66  90 118 150 186 226
 * 197 159 125  95  69  47  29  45  65  89 117 149 185
 * 160 126  96  70  48  30  16  28  44  64  88 116 148
 * 127  97  71  49  31  17   7  15  27  43  63  87 115
 *  98  72  50  32  18   8   2   6  14  26  42  62  86
 *  73  51  33  19   9   3   0   1   5  13  25  41  61  <- y = 0
 * 100  74  52  34  20  10   4  12  24  40  60  84 112
 * 131 101  75  53  35  21  11  23  39  59  83 111 143
 * 166 132 102  76  54  36  22  38  58  82 110 142 178
 * 205 167 133 103  77  55  37  57  81 109 141 177 217
 * 248 206 168 134 104  78  56  80 108 140 176 216 260
 * 295 249 207 169 135 105  79 107 139 175 215 259 307
 *
 */
int couple_to_int( int q1, int q2 ){
	if(q1==0 && q2==0) return 0;
	int sum = abs(q1) + abs(q2);
	int prev = 2*sum*(sum-1);
	if( q1 > 0 && q2 >= 0 ) return prev + 1 + q2;
	if( q1 <= 0 && q2 > 0 ) return prev + sum+1 + (-q1);
	if( q1 < 0 && q2 <= 0 ) return prev + 2*sum+ 1 + (-q2);
	return prev + 3*sum+ 1 + q1; //cas q1>=0 && q2<0
}

void int_to_couple( int entree, int * q1, int * q2 ){
	if( entree == 0 ){
		*q1 = 0;
		*q2 = 0;
		return;
	}
	entree -= 1;
	int sum = (int) floor( ( 1+ sqrt( 1 + 2.0*entree ) )/2.0 );
	int couche = entree - 2*sum*(sum-1);
	int r = couche % sum;
	int s = sum-r;
	switch( couche/sum ){
		case 0:
			*q2=r;
			*q1=s;
			break;
		case 1:
			*q1=-r;
			*q2=s;
			break;
		case 2:
			*q2 = -r;
			*q1=-s;
			break;
		case 3:
			*q1 = r;
			*q2=-s;
			break;
		default:
			ERREUR("Impossible !");
	}
}

Automate * creer_intersection_des_automates(
	const Automate * automate_1, const Automate * automate_2
){
	Automate * res = creer_automate();

	// On engendre tous les couples et on les ajoute dans l'automate
	Ensemble_iterateur it_etat_1;
	Ensemble_iterateur it_etat_2;
	Ensemble_iterateur it_lettre;
	for(
		it_etat_1 = premier_iterateur_ensemble( get_etats( automate_1 ) );
		! iterateur_ensemble_est_vide( it_etat_1 );
		it_etat_1 = iterateur_suivant_ensemble( it_etat_1 )
	){
		int q1 = get_element( it_etat_1 );
		for(
			it_etat_2 = premier_iterateur_ensemble( get_etats( automate_2 ) );
			! iterateur_ensemble_est_vide( it_etat_2 );
			it_etat_2 = iterateur_suivant_ensemble( it_etat_2 )
		){
			int q2 = get_element( it_etat_2 );
			ajouter_etat( res, couple_to_int( q1, q2 ) );
		}
	}

	// On engendre tous les couples d'états initiaux :
	for(
		it_etat_1 = premier_iterateur_ensemble( get_initiaux( automate_1 ) );
		! iterateur_ensemble_est_vide( it_etat_1 );
		it_etat_1 = iterateur_suivant_ensemble( it_etat_1 )
	){
		int q1 = get_element( it_etat_1 );
		for(
			it_etat_2 = premier_iterateur_ensemble( get_initiaux( automate_2 ) );
			! iterateur_ensemble_est_vide( it_etat_2 );
			it_etat_2 = iterateur_suivant_ensemble( it_etat_2 )
		){
			int q2 = get_element( it_etat_2 );
			ajouter_etat_initial(
				res, couple_to_int( q1, q2 )
			);
		}
	}

	// On engendre tous les couples d'états finaux :
	for(
		it_etat_1 = premier_iterateur_ensemble( get_finaux( automate_1 ) );
		! iterateur_ensemble_est_vide( it_etat_1 );
		it_etat_1 = iterateur_suivant_ensemble( it_etat_1 )
	){
		int q1 = get_element( it_etat_1 );
		for(
			it_etat_2 = premier_iterateur_ensemble( get_finaux( automate_2 ) );
			! iterateur_ensemble_est_vide( it_etat_2 );
			it_etat_2 = iterateur_suivant_ensemble( it_etat_2 )
		){
			int q2 = get_element( it_etat_2 );
			ajouter_etat_final(
				res, couple_to_int( q1, q2 )
			);
		}
	}

	// On engendre l'alphabet :
	for(
		it_lettre = premier_iterateur_ensemble( get_alphabet( automate_1 ) );
		! iterateur_ensemble_est_vide( it_lettre );
		it_lettre = iterateur_suivant_ensemble( it_lettre )
	){
		char lettre = get_element( it_lettre );
		ajouter_lettre( res, lettre );
	}
	for(
		it_lettre = premier_iterateur_ensemble( get_alphabet( automate_2 ) );
		! iterateur_ensemble_est_vide( it_lettre );
		it_lettre = iterateur_suivant_ensemble( it_lettre )
	){
		char lettre = get_element( it_lettre );
		ajouter_lettre( res, lettre );
	}

	// On engendre les transitions :
	Ensemble_iterateur it_etat;
	for(
		it_etat = premier_iterateur_ensemble( get_etats( res ) );
		! iterateur_ensemble_est_vide( it_etat );
		it_etat = iterateur_suivant_ensemble( it_etat )
	){
		int q = get_element( it_etat );
		int o1 , o2, e1, e2;
		int_to_couple( q, &o1, &o2 );
		for(
			it_lettre = premier_iterateur_ensemble( get_alphabet( res ) );
			! iterateur_ensemble_est_vide( it_lettre );
			it_lettre = iterateur_suivant_ensemble( it_lettre )
		){
			char lettre = get_element( it_lettre );
			const Ensemble * v1 = voisins( automate_1, o1, lettre );
			const Ensemble * v2 = voisins( automate_2, o2, lettre );
			for(
				it_etat_1 = premier_iterateur_ensemble( v1 );
				! iterateur_ensemble_est_vide( it_etat_1 );
				it_etat_1 = iterateur_suivant_ensemble( it_etat_1 )
			){
				e1 = get_element( it_etat_1 );
				for(
					it_etat_2 = premier_iterateur_ensemble( v2 );
					! iterateur_ensemble_est_vide( it_etat_2 );
					it_etat_2 = iterateur_suivant_ensemble( it_etat_2 )
				){
					e2 = get_element( it_etat_2 );
					ajouter_transition( res, q, lettre, couple_to_int(e1,e2) );
				}
			}
		}
	}

	return res;
}

int etat_minimal( const Automate * automate ){
	int min = INT_MAX;
	Ensemble_iterateur it1;
	for(
		it1 = premier_iterateur_ensemble( get_etats( automate ) );
		! iterateur_ensemble_est_vide( it1 );
		it1 = iterateur_suivant_ensemble( it1 )
	){
		if( get_element(it1) < min ) min = get_element(it1);
	}
	return min;
}

int etat_maximal( const Automate * automate ){
	int max = INT_MIN;
	Ensemble_iterateur it1;
	for(
		it1 = premier_iterateur_ensemble( get_etats( automate ) );
		! iterateur_ensemble_est_vide( it1 );
		it1 = iterateur_suivant_ensemble( it1 )
	){
		if( get_element(it1) > max ) max = get_element(it1);
	}
	return max;
}

Ensemble* etats_accessibles( const Automate * automate, int etat ){
	Ensemble * resultat = creer_ensemble( NULL, NULL, NULL );
	Ensemble * courants = creer_ensemble( NULL, NULL, NULL );
	ajouter_element( resultat, etat );
	ajouter_elements( courants, resultat );

	int n = taille_ensemble( get_etats( automate ) );
	int i;

	Ensemble* tmp;
	Ensemble* next;

	for( i=0; i< n-1; i++ ){

		next = creer_ensemble( NULL, NULL, NULL );

		Ensemble_iterateur it;
		for(
			it = premier_iterateur_ensemble( get_alphabet( automate ) );
			! iterateur_ensemble_est_vide( it );
			it = iterateur_suivant_ensemble( it )
		){
			tmp = delta( automate, courants, get_element(it) );
			ajouter_elements( next, tmp );
			liberer_ensemble( tmp );
		}

		ajouter_elements( resultat, next );
		liberer_ensemble( courants );
		courants = next;
	}

	liberer_ensemble( courants );

	return resultat;
}

Ensemble* accessibles( const Automate * automate ){
	Ensemble_iterateur it1;
	Ensemble * access = creer_ensemble( NULL, NULL, NULL );
	for(
		it1 = premier_iterateur_ensemble( get_initiaux( automate ) );
		! iterateur_ensemble_est_vide( it1 );
		it1 = iterateur_suivant_ensemble( it1 )
	){
		Ensemble * ens = etats_accessibles( automate, get_element(it1) );
		ajouter_elements( access, ens );
		liberer_ensemble( ens );
	}
	return access;
}

Automate *automate_accessible( const Automate * automate ){
	Automate * res = creer_automate();
	Ensemble_iterateur it1;

	Ensemble * access = accessibles( automate );

	// On ajoute les états de l'automate
	for(
		it1 = premier_iterateur_ensemble( access );
		! iterateur_ensemble_est_vide( it1 );
		it1 = iterateur_suivant_ensemble( it1 )
	){
		ajouter_etat( res, get_element( it1 ) );
	}
	// On ajoute les états initiaux
	for(
		it1 = premier_iterateur_ensemble( get_initiaux( automate ) );
		! iterateur_ensemble_est_vide( it1 );
		it1 = iterateur_suivant_ensemble( it1 )
	){
		ajouter_etat_initial( res, get_element( it1 ) );
	}
	// On ajoute les états finaux
	for(
		it1 = premier_iterateur_ensemble( access );
		! iterateur_ensemble_est_vide( it1 );
		it1 = iterateur_suivant_ensemble( it1 )
	){
		int etat = get_element( it1 );
		if( est_un_etat_final_de_l_automate( automate, etat ) ){
			ajouter_etat_final( res, etat );
		}
	}
	// On ajoute les lettres
	for(
		it1 = premier_iterateur_ensemble( get_alphabet( automate ) );
		! iterateur_ensemble_est_vide( it1 );
		it1 = iterateur_suivant_ensemble( it1 )
	){
		ajouter_lettre( res, (char) get_element( it1 ) );
	}
	// On ajoute les transitions
	Table_iterateur it2;
	for(
		it2 = premier_iterateur_table( automate->transitions );
		! iterateur_est_vide( it2 );
		it2 = iterateur_suivant_table( it2 )
	){
		Cle * cle = (Cle*) get_cle( it2 );
		int origine = cle->origine;
		char lettre = cle->lettre;
		if( est_dans_l_ensemble( access, origine ) ){
			Ensemble * fins = (Ensemble*) get_valeur( it2 );
			for(
				it1 = premier_iterateur_ensemble( fins );
				! iterateur_ensemble_est_vide( it1 );
				it1 = iterateur_suivant_ensemble( it1 )
			){
				int fin = get_element( it1 );
				ajouter_transition( res, origine, lettre, fin );
			}
		}
	};
	liberer_ensemble( access );
	return res;
}

Automate *miroir( const Automate * automate)
{

    Automate *automate_miroir = creer_automate();

    // On remplace les états finaux de "automate" dans les états initiaux de "automate_miroir"
	ajouter_elements( automate_miroir->initiaux, get_finaux( automate ) );
	// On ajoute les états initiaux de "automate" dans les états finaux de "automate_miroir"
	ajouter_elements( automate_miroir->finaux, get_initiaux( automate ) );
	// On ajoute les états de "automate" dans les états de "automate_miroir"
	ajouter_elements( automate_miroir->etats, get_etats( automate ) );
	// On ajoute l'alphabet de "automate" dans l'alphabet de "automate_miroir"
	ajouter_elements( automate_miroir->alphabet, get_alphabet( automate ) );

    // On inverse le sens des fleches de l'automate
    Table_iterateur it_transition;
    Ensemble_iterateur it_origines;
    for(
        it_transition = premier_iterateur_table( automate->transitions );
        ! iterateur_est_vide( it_transition );
        it_transition = iterateur_suivant_table( it_transition )
        )
    {
        Cle *cle = (Cle *) get_cle( it_transition );
        // l'origine de la transition de "automate" devient la fin de la transition de "automate_miroir"
        int fin = cle->origine;
        char lettre = cle->lettre;

        // les voisins de cette cle->origine (ie fin) et cette lettre deviennent l'ensemble des origines
        const Ensemble *origines = voisins( automate, fin, lettre );

        for(
            it_origines = premier_iterateur_ensemble( origines );
            ! iterateur_ensemble_est_vide( it_origines );
            it_origines = iterateur_suivant_ensemble( it_origines )
            )
        {
            // l'origine de la transition de "automate" devient la fin de la transition de "automate_miroir"
            int origine = get_element( it_origines );
            ajouter_transition( automate_miroir, origine, lettre, fin );
        }
    }

   	return automate_miroir;
}

void action_nombre_de_transitions(
	int origine, char lettre, int fin, void* data
){
	int* nb = (int*) data;
	(*nb) += 1;
}

int nombre_de_transitions( const Automate* automate ){
	int res = 0;
	pour_toute_transition( automate, action_nombre_de_transitions ,&res );
	return res;
}

void action_creer_intersection_des_automates(
	int origine, char lettre, int fin, void* data
){
	Automate * res = (Automate*) data;
	ajouter_transition( res, origine, lettre, fin );
}

int est_une_transition_de_l_automate(
	const Automate* automate,
	int origine, char lettre, int fin
){
	return est_dans_l_ensemble( voisins( automate, origine, lettre ), fin );
}

int est_un_etat_de_l_automate( const Automate* automate, int etat ){
	return est_dans_l_ensemble( get_etats( automate ), etat );
}

int est_un_etat_initial_de_l_automate( const Automate* automate, int etat ){
	return est_dans_l_ensemble( get_initiaux( automate ), etat );
}

int est_un_etat_final_de_l_automate( const Automate* automate, int etat ){
	return est_dans_l_ensemble( get_finaux( automate ), etat );
}

int est_une_lettre_de_l_automate( const Automate* automate, char lettre ){
	return est_dans_l_ensemble( get_alphabet( automate ), lettre );
}

void print_ensemble_2( const intptr_t ens ){
	print_ensemble( (Ensemble*) ens, NULL );
}

void print_lettre( intptr_t c ){
	printf("%c", (char) c );
}

void print_automate( const Automate * automate ){
	printf("- Etats : ");
	print_ensemble( get_etats( automate ), NULL );
	printf("\n- Initiaux : ");
	print_ensemble( get_initiaux( automate ), NULL );
	printf("\n- Finaux : ");
	print_ensemble( get_finaux( automate ), NULL );
	printf("\n- Alphabet : ");
	print_ensemble( get_alphabet( automate ), print_lettre );
	printf("\n- Transitions : ");
	print_table(
		automate->transitions,
		( void (*)( const intptr_t ) ) print_cle,
		( void (*)( const intptr_t ) ) print_ensemble_2,
		""
	);
	printf("\n");
}

int le_mot_est_reconnu( const Automate* automate, const char* mot ){
	Ensemble * arrivee = delta_star( automate, get_initiaux(automate) , mot );

	int result = 0;

	Ensemble_iterateur it;
	for(
		it = premier_iterateur_ensemble( arrivee );
		! iterateur_ensemble_est_vide( it );
		it = iterateur_suivant_ensemble( it )
	){
		if( est_un_etat_final_de_l_automate( automate, get_element(it) ) ){
			result = 1;
			break;
		}
	}
	liberer_ensemble( arrivee );
	return result;
}

int ajouter_ensemble(
	const Ensemble* ens,
	Table* ensemble_to_id, Table* id_to_ensemble, Fifo* f,
	Automate * aut, int next_id
){

	Table_iterateur it = trouver_table( ensemble_to_id, (intptr_t) ens );
	if( iterateur_est_vide( it ) ){
		add_table( ensemble_to_id, (intptr_t) ens, next_id );
		add_table( id_to_ensemble, next_id, (intptr_t) ens );
		ajouter_fifo( f, (intptr_t) ens );
		ajouter_etat( aut, next_id );
		return next_id+1;
	}else{
		return next_id;
	}
}

Automate * creer_automate_deterministe( const Automate* automate ){
	Automate * res = creer_automate();

	Fifo* f = creer_fifo();
	Table* ensemble_to_id = creer_table(
		( int(*)(const intptr_t, const intptr_t) ) comparer_ensemble,
		( intptr_t (*)( const intptr_t ) ) copier_ensemble,
		( void(*)(intptr_t) ) liberer_ensemble
	);
	Table* id_to_ensemble = creer_table( NULL, NULL, NULL );

	int next_id = ajouter_ensemble(
		copier_ensemble( get_initiaux( automate ) ),
		ensemble_to_id, id_to_ensemble, f, res, 0
	);
	ajouter_etat_initial( res, 0 );

	while( ! est_vide( f ) ){
		Ensemble* e = (Ensemble*) retirer_fifo( f );
		int id_e = get_valeur( trouver_table( ensemble_to_id, (intptr_t) e ) );

		Ensemble_iterateur it_lettre;
		for(
			it_lettre = premier_iterateur_ensemble( get_alphabet( automate ) );
			! iterateur_ensemble_est_vide( it_lettre );
			it_lettre = iterateur_suivant_ensemble( it_lettre )
		){
			char lettre = (char) get_element( it_lettre );
			Ensemble * img = delta( automate, e, lettre );
			int id = ajouter_ensemble(
				img, ensemble_to_id, id_to_ensemble, f, res, next_id
			);
			ajouter_transition(
				res, id_e, lettre, get_valeur(
					trouver_table( ensemble_to_id, (intptr_t) img )
				)
			);
			if( next_id == id ){
				liberer_ensemble(img);
			}else{
				next_id = id;
			}
		}

		Ensemble_iterateur it_e;
		for(
			it_e = premier_iterateur_ensemble( e );
			! iterateur_ensemble_est_vide( it_e );
			it_e = iterateur_suivant_ensemble( it_e )
		){
			int elmt = get_element( it_e );
			if(  est_un_etat_final_de_l_automate( automate, elmt ) ){
				ajouter_etat_final( res, id_e );
				break;
			}
		}
	}

	pour_toute_valeur_table(
		id_to_ensemble,
		( void (*)(intptr_t valeur ) ) liberer_ensemble
	);
	liberer_table( id_to_ensemble );
	liberer_table( ensemble_to_id );

	liberer_fifo( f );
	return res;
}

Automate * creer_automate_minimal( const Automate* automate )
{
	Automate *automate_minimal = creer_automate();
	automate_minimal = miroir(automate);
	automate_minimal = creer_automate_deterministe(automate_minimal);
	automate_minimal = miroir(automate_minimal);
	automate_minimal = creer_automate_deterministe(automate_minimal);

	return automate_minimal;
}
