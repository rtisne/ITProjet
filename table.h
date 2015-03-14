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

/** @file table.h */ 

#ifndef __TABLE_H__
#define __TABLE_H__

#include <stdint.h>
#include "avl.h"

/**
 * @brief Définit le type d'une table.
 * 
 * Une table est une structure de donnée qui associe une clé à une et une seule valeur.
 *
 * Une table ne peut pas contenir deux fois la même clé.
 * Par contre, à deux clés différentes, on peut assicer deux fois la même valeur.
 */
typedef struct Table Table;

/**
 * @brief Définit le type d'un itérateur sur les éléments d'une table.
 */
typedef struct avl_traverser Table_iterateur;

/**
 * @brief Renvoie une nouvelle table.
 *
 * La clé et la valeur peuvent être des entiers ou bien des structures 
 * plus compliquées.
 * 
 * La mémoire associée à la clé est gérée par la table, alors que celle
 * de la valeur est laissée à la charge de l'utilisateur.
 * C'est pourquoi, à la création de la table, il faut donner des pointeurs
 * de fonctions à la table pour lui permettre de copier, supprimer et comparer
 * des clés.
 * Si vous choisissez d'utiliser des entiers pour coder la clé, vous pouvez 
 * mettre tous les paramètres 'comparer_cle', 'copier_cle' et 'supprimer_cle' à 
 * NULL. En effet, dans ce cas, la table se chargera d'utiliser ses propres 
 * fonctions pour copier et comparer les clés.
 * 
 *=============================================================================
 *                        Exemple 1
 *=============================================================================
 * Voici un exemple qui 
 *  - crée une table qui, à un entier, associe un entier.
 *  - affiche la table en utilisant des itérateurs.
 *
 * Table * table = creer_table( NULL, NULL, NULL );
 *
 * add_table( table, 1, 2 );
 * add_table( table, 2, 30 );
 *
 * Table_iterateur it;
 * for( 
 *     it = premier_iterateur_table( table ); 
 *     ! iterateur_est_vide( it );  
 *     it = iterateur_suivant_table( it )
 * ){
 *     printf( "cle : %d -> valeur : %d \n", get_cle( it ), get_valeur( it ) );
 * }
 *
 * liberer_table( t )
 *
 * 
 *=============================================================================
 *                        Exemple 2
 *=============================================================================
 * Dans cet exemple, nous montrons comment associer un entier à un couple
 *
 * typdef struct { int x; int y; } Couple;   // On définit le couple
 *
 * Table * t = creer_table( NULL; NULL, NULL );  // On crée la table
 *
 * Couple * c = malloc( sizeof(Couple) ):        // On crée une valeur
 * c.x = 1; c.y=2;
 *
 * add_table( t,(intptr_t) c );           // On ajoute la valeur dans la table
 *
 * ...                                   // On utilise la table
 *
 *
 * pour_toute_valeur_table( t, free )   // On détruit les valeurs
 *                                      // La mémoire des valeurs n'est pas  
 *                                      // gérée par la table ! 
 * liberer_table( t );                  // On détruit la table
 *
 *=============================================================================
 *                        Exemple 3
 *============================================================================= 
 * Voici maintenant un exemple de table où l'on associe un couple à un entier
 *
 *
 * typdef struct { int x; int y; } Couple;   // On définit la clé
 *
 * int comparer_couple( Couple* c1, Couple* c2 ){
 *     if( c1.x == c2.x ) return c1.y -c2.y;
 *     return c1.x - c2.x;
 * }
 *
 * Couple* copier_couple( const Couple* c ){
 *     Couple * res = malloc( sizeof(Couple) );
 *     res.x = c->x;
 *     res.y = c->y;
 *     return res;
 * }
 *
 * void supprimer_couple( Couple* c ){
 *     free( c );
 * }
 * 
 * typdef struct { int x; int y; } Couple;   // On définit le couple
 *
 * Table * table = creer_table(           // On crée la table
 *     ( void(*)( const intptr_t, const intptr_t) ) comparer_couple, 
 *     ( void(*)( const intptr_t ) ) copier_couple, 
 *     ( void(*)( intptr_t ) ) supprimer_couple
 * );
 *
 * Couple c;
 * c.x = 3;  c.y = 4;
 *
 * add_table( table, (intptr_t) &c, 4 ); // On ajoute l'associaiton (3,4) --> 4
 *
 * ...                                   // On utilise la table
 * 
 * liberer_table( table );               // On détruit la table
 */
Table* creer_table(
	int (*comparer_cle)( const intptr_t cle1, const intptr_t cle2),
	intptr_t (*copier_cle)( const intptr_t cle ),
	void (*supprimer_cle)(intptr_t cle)
);

/**
 * @brief
 * Cette fonction détruit une table. La mémoire qui a été allouée par la table 
 * pour stocker les clés sont aussi détruites. 
 * Attention, la table ne gère pas la mémoire associée aux valeurs.
 * C'est à l'utilisateur de détruire proprement les valeurs.
 *
 */
void liberer_table( Table* table );

/**
 * @brief
 * La fonction add_table() ajoute une association entre une clé et une valeur.
 * Pour cela, add_table(), crée une copie de la clé passée en paramètre, 
 * l'enregistre dans la table et l'associe à la valeur passée en paramètre.
 * La mémoire de la copie de la clé est gérée par la table.
 * La mémoire de la clé passée en paramètre reste à la charge de l'utilisateur.
 * 
 * Dans une table, il ne peut pas y avoir deux clés identiques en même temps.
 * Si la clé passée en paramètre à add_table() existe déjà dans la table, alors,
 * l'ancienne valeur associée à la clé est remplacée par la nouvelle passée en 
 * paramètre à add_table().
 *
 */
void add_table( Table* table, const intptr_t cle, const intptr_t valeur );


/**
 * @brief
 * Supprime une clé de la table. La mémoire de la clé est libérée et la valeur
 * associée à la clé est perdue.
 * 
 * Si la valeur contenait un pointeur vers un espace mémoire, cette mémoire
 * n'est <b>PAS</b> libérée ! Sa gestion est laissée à la charge de l'utilisateur.
 *
 */
intptr_t delete_table( Table* table, const intptr_t cle );


/**
 * @brief
 * Supprime toutes les clés de la table.
 */
void vider_table( Table* table );

/**
 * @brief
 * Affiche la table. Vous devez donner en paramètres de print_table, des
 * fonctions pour afficher les clés et les valeurs.
 *
 * Si les clés ou les valeurs sont des entiers, vous pouvez ne pas donner 
 * ces fonctions en remplissant les champs par le pointeur vide : NULL .
 */
void print_table(
	Table* table, void (*print_cle)( const intptr_t cle ), 
	void (*print_valeur)( const intptr_t valeur ),
	const char* text_de_fin
);

/**
 * @brief
 * La fonction passe en revue toutes les valeurs de la table passée en paramètre
 * et apelle la fonction passée en paramètre pour chacune des valeur parcourues.
 * Une valeur est parcourue autant de fois qu'elle apparaît dans la table, 
 * et la fonction est executée autant de fois que la valeur apparaît.
 *
 * La fonction qui sera executée (et qui a été passée en paramètre), doit 
 * posséder l'entête suivante :
 *     void NON_FONCTION( intptr_t valeur, void* data  ),
 *     void NON_FONCTION( const intptr_t cle, intptr_t valeur, void* data  ),
 * Le pramètre 'valeur' correspond à la valeur qui est en train d'être parcouru.
 * Le paramètre 'data' est un pointeur qui sera identique à celui passé par le
 * paramètre 'data' de la fonction pour_toute_transition().
 */ 
void pour_toute_valeur_table(
	const Table* table,
	void (*action)(intptr_t valeur )
);


/**
 * @brief
 * La fonction passe en revue toutes les associaitons (clé, valeur) de la table,
 * et apelle la fonction passée en paramètre pour chasune de ces associations.
 *
 * La fonction qui sera executée (et qui a été passée en paramètre), doit 
 * posséder l'entête suivante :
 *     void NON_FONCTION( const intptr_t cle, intptr_t valeur, void* data  ),
 * Les pramètres 'cle et 'valeur' correspondent à la clé et la valeur de 
 * l'association qui est en train d'être parcouru.
 * Le paramètre 'data' est un pointeur qui sera identique à celui passé par le
 * paramètre 'data' de la fonction pour_toute_transition().
 */ 
void pour_toute_cle_valeur_table(
	const Table* table,
	void (* action)( const intptr_t cle, intptr_t valeur, void* data  ),
	void* data
);

/**
 * @brief
 * Renvoie un itérateur positionné sur l'association dont la clé est identique 
 * (pour la fonction de comparaison de clé de la table) à la clé passée en 
 * paramètre.
 */
Table_iterateur trouver_table( const Table* table, const intptr_t cle );

/**
 * @brief
 * Renvoie un itérateur positionné sur la première association de la table.
 *
 * Deux associations sont comparer en comparant leurs clés et la fonction de 
 * comparaison des clés de la table.
 */
Table_iterateur premier_iterateur_table( const Table* table );

/**
 * @brief
 * Renvoie l'itérateur suivant.
 *
 * C'est-à-dire, l'itérateur renvoyé est positionné sur une nouvelle 
 * association qui est la plus petite des associations plus grandes que celle
 * associée à l'itérateur passé en en paramètre.
 *
 * Deux associations sont comparées en comparant leurs clés et la fonction de 
 * comparaison des clés de la table.
 *
 * S'il n'y a pas d'itérateur suivant (l'itérateur passé en paramètre est 
 * associé à la plus grande association), l'itérateur vide est renvoyé.
 */
Table_iterateur iterateur_suivant_table( Table_iterateur iterator );

/**
 * @brief
 * Renvoie l'itérateur précédent.
 *
 * C'est-à-dire, l'itérateur renvoyé est positionné sur une nouvelle 
 * association qui est la plus grande des associations plus petites que celle
 * associée à l'itérateur passé en en paramètre.
 *
 * Deux associations sont comparées en comparant leurs clés par la fonction de 
 * comparaison des clés de la table.
 *
 * S'il n'y a pas d'itérateur précédent (l'itérateur passé en paramètre est 
 * associé à la plus petite association), l'itérateur vide est renvoyé.
 */
Table_iterateur iterateur_precedent_table( Table_iterateur iterator );

/**
 * @brief
 * Renvoie 1 si l'itérateur passé en paramètre est vide.
 * Renvoie 0 sinon.
 */
int iterateur_est_vide( Table_iterateur iterator );

/**
 * @brief
 * Renvoie la clé de l'association pointée par l'itérateur passé en paramètre.
 */
intptr_t get_cle( Table_iterateur it );

/**
 * @brief
 * Renvoie la valeur de l'association pointée par l'itérateur passé en paramètre.
 */
intptr_t get_valeur( Table_iterateur it );

/**
 * @brief
 * Renvoie la taille de la table.
 */
int taille_table( Table* t );

#endif
