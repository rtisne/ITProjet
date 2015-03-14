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


#ifndef __ENSEMBLE_H__
#define __ENSEMBLE_H__

#include <stdint.h>

#include "avl.h"
#include "table.h"

/*
 * Définit le type d'un ensemble.
 */
struct Ensemble {
	Table* table;
	int (*comparer_element)( const intptr_t elem1, const intptr_t elem2 );
	intptr_t (*copier_element)( const intptr_t elem );
	void (*supprimer_element)(intptr_t elem );
};

typedef struct Ensemble Ensemble;

/*
 * Définit le type d'un itérateur sur les éléments d'un ensemble.
 */
typedef struct avl_traverser Ensemble_iterateur;

/*
 * Renvoie un nouvel ensemble vide.
 *
 * L'ensemble peut contenir des entiers, codés à l'aide du type intptr_t.
 * Mais il peut aussi contenir des pointeurs vers des structures plus complexes.
 * Dans ce cas, c'est l'ensmeble qui devient le responsable de la mémoire des 
 * éléments situé à l'interieur de l'ensemble.
 * Pour pouvoir gérer cette mémoire, l'ensemble à besoin des fonctions :
 *   - int comparer_element( const intptr_t elem1, const intptr_t elem2 ),
 *   - intptr_t copier_element( const intptr_t elem ),
 *   - void supprimer_element( intptr_t elem ),
 * qui permettent de comaprer, upprimer et copier des éléments de l'ensemble.
 *
 */
Ensemble * creer_ensemble(
	int (*comparer_element)( const intptr_t elem1, const intptr_t elem2 ),
	intptr_t (*copier_element)( const intptr_t elem ),
	void (*supprimer_element)( intptr_t elem )
);

/*
 * Libère la mémoire d'un ensemble.
 * La mémoire de tous les éléments de l'ensemble est aussi libérée.
 */
void liberer_ensemble( Ensemble* ens );

/*
 * Ajoute un élement passé en paramètre à l'ensemble.
 *
 * L'élément ajouté est copié. La mémoire de la copie n'est pas à la charge
 * de l'utilisateur. Par contre, l'élément copié (celui qui a été passé en 
 * paramètre) reste à la charge de l'utilisateur qui ne devra pas oublier de 
 * de le supprimer proprement.
 */
void ajouter_element( Ensemble * ensemble, const intptr_t  element );

/*
 * Ajoute tous les éléments d'un ensemble à un ensemble.
 */
void ajouter_elements( Ensemble * ens1, const Ensemble * ens2 );

/*
 * Retire un élément de l'ensemble
 *
 * Cette fonction cherche dans l'ensemble un élément idenitque (pour la fonction
 * de comparaison des éléments) à celui passé en paramètre.
 * Si la fonction trouve cet élément, elle retire l'élément de l'ensemble 
 * et libère son espace mémoire.
 */
void retirer_element( Ensemble * ensemble, const intptr_t element );

/*
 * Retire tous les éléments d'un ensemble
 */
void retirer_elements( Ensemble * ens1, const Ensemble * ens2 );

/*
 * Retire et supprime tous les éléments d'un ensemble.
 */
void vider_ensemble( Ensemble * ensemble );

/*
 * Renvoie Vrai si il existe un élément dans l'ensmble égal (pour la donction
 * de comparaion de l'ensemble) à l'élément passé en paramètre.
 */
int est_dans_l_ensemble( const Ensemble * ensemble, const intptr_t element );

/*
 * Renvoie le nombre d'éléments qui se trouvent dans l'ensemble.
 */
unsigned int taille_ensemble( const Ensemble* ensemble );

/*
 * Compare deux ensembles entre eux.
 *
 * Cette fonction renvoie 0 si les ensembles sont identiques.
 * 
 * Soit t1 (resp. t2) le tuple contenant les éléments de l'ensemble 1 
 * (resp. l'ensemble 2) trié par ordre lexicographique, alors, la fonction 
 * renvoie -1 si t1 est plus petit lexicographiquement que 
 * t2, 0 si ils sont identiques et 1 sinon.
 */
int comparer_ensemble( const Ensemble* ens1, const Ensemble*  ens2 );

/*
 * Renvoie une copie de l'ensemble passé en paramètre
 */
Ensemble* copier_ensemble( const Ensemble* ensemble );

/*
 * Crée un nouvel ensemble qui est la copie de deux ensembles passés en 
 * paramètre
 */
Ensemble * creer_union_ensemble( const Ensemble* ens1, const Ensemble* ens2 );

/*
 * Créé un nouvel ensemble qui est la différence de deux ensembles passés en 
 * paramètre.
 */
Ensemble * creer_difference_ensemble(
	const Ensemble* ens1, const Ensemble* ens2
);

/*
 * Créé un nouvel ensemble qui est l'intersection de deux ensembles passés en 
 * paramètre.
 */
Ensemble * creer_intersection_ensemble(
	const Ensemble* ens1, const Ensemble* ens2
);

/*
 * Passe en revue tous les éléments d'un ensemble et execute un fonction 
 * passée en paramètre.
 */
void pour_tout_element(
	const Ensemble* ensemble, 
	void (* action )( const intptr_t element, void* data ),
	void* data
);

/*
 * Affiche tous les éléments d'un ensemble.
 */
void print_ensemble(
	const Ensemble* ens, void (*print_element)( const intptr_t element ) 
);

/*
 * Échange le contenu de deux ensembles passés en paramètre.
 */
void swap_ensemble( Ensemble* ens1, Ensemble* ens2 );

/*
 * Supprime le contenu de l'ensemble destination, déplace le contenu de 
 * l'ensemble source dans celui de destination et libère la mémoire
 * de l'ensemble source. 
 */
void deplacer_ensemble( Ensemble* dest, Ensemble* source );

/*
 * Renvoie un itérateur positionné sur l'élément identique (pour la fonction 
 * de comparaison des éléments de la table) à l'élément passé en 
 * paramètre.
 */
Ensemble_iterateur trouver_ensemble(
	const Ensemble* ensemble, const intptr_t element
);

/*
 * Renvoie un itérateur positionné sur le premier élement de l'ensemble.
 */
Ensemble_iterateur premier_iterateur_ensemble( const Ensemble* ensemble );

/*
 * Renvoie l'iterateur suivant.
 *
 * C'est à dire, l'iterateur renvoyé est positionné sur un nouvel élément
 * qui est le plus petit des éléments parmi les élément plus grands que celui 
 * associé à l'iterateur passé en en paramètre.
 *
 * Deux éléments sont comparés en utilsant la fonction de 
 * comparaison des éléments de l'ensemble.
 *
 * S'il n'y a pas d'itérateur suivant (l'itérateur passé en paramètre est 
 * associé au plus grand élément), l'itérateur vide est renvoyé.
 */
Ensemble_iterateur iterateur_suivant_ensemble( Ensemble_iterateur iterateur );

/*
 * Renvoie l'iterateur précédent.
 *
 * C'est à dire, l'iterateur renvoyé est positionné sur un nouvel élément 
 * qui est le plus grand élément parmi les éléments plus petit que celui 
 * associé à l'iterateur passé en en paramètre.
 *
 * Deux éléments sont comparés en utilisant la fonction de comparaison des 
 * élément de l'ensemble.
 *
 * S'il n'y a pas d'itérateur précédent (l'itérateur passé en paramètre est 
 * associé à la plus petite association), l'itérateur vide est renvoyé.
 */
Ensemble_iterateur iterateur_precedent_ensemble( Ensemble_iterateur iterateur );

/*
 * Renvoie 1 si l'iterateur passé en paramètre est vide.
 * Renvoie 0 sinon.
 */
int iterateur_ensemble_est_vide( Ensemble_iterateur iterateur );

/*
 * Renvoie l'élément associé à l'itérateur passé en paramètre.
 */
intptr_t get_element( Ensemble_iterateur it );

#endif
