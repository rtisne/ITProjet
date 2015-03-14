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

/** @file automate.h */ 

#ifndef __AUTOMATE_H__
#define __AUTOMATE_H__

#include "ensemble.h"

/**
 * @brief Le type d'un automate.
 * 
 * Ce type code un automate. Cet automate peut être non déterministe, ses 
 * états sont des entiers codés par le 
 * type int. Les lettres sont codées par le type char, et l'automate n'accepte 
 * pas d'epsilon transition.
 * L'automate codé peut avoir plusieurs états initiaux.
 * 
 */

struct Automate {
   Ensemble * vide; //!<
	Ensemble * etats;
	Ensemble * alphabet;
	Table* transitions;
	Ensemble * initiaux;
	Ensemble * finaux;
};

typedef struct Automate Automate;

typedef struct Cle {
	int origine;
	int lettre;
} Cle;

/**
 * @brief Crée un automate vide, sans états, sans lettres et sans transitions.
 *
 * @return L'automate créé.
 */
Automate * creer_automate();

/**
 * @brief Détruit un automate.
 * 
 * @param automate L'automate à détruire.
 */ 
void liberer_automate( Automate * automate);

/**
 * @brief Ajoute un état à un automate passé en paramètre.
 *
 * @param automate Un automate.
 * @param etat L'état à ajouter.
 */ 
void ajouter_etat( Automate * automate, int etat );

/**
 * @brief Ajoute une lettre à l'alphabet d'un automate passé en paramètre.
 *
 * @param automate Un automate.
 * @param lettre La lettre à ajouter.
 */ 
void ajouter_lettre( Automate * automate, char lettre );

/**
 * @brief Ajoute une transition à l'automate passé en paramètre.
 *
 * Si les états et la lettre de la transition n'existent pas dans l'automate,
 * alors la lettre et les états sont ajoutés automatiquement à l'automate.
 *
 * @param automate Un automate.
 * @param origine L'origine de la transition.
 * @param lettre La lettre de la transition.
 * @param fin La fin de la transition.
 */ 
void ajouter_transition(
	Automate * automate, int origine, char lettre, int fin
);

/**
 * @brief Ajoute un état final à un automate passé en paramètre.
 *
 * Si l'état n'existe pas dans l'automate, l'état est ajouté 
 * automatiquement à l'automate.
 *
 * @param automate Un automate.
 * @param etat_final L'état final.
 */ 
void ajouter_etat_final(
	Automate * automate, int etat_final
);

/**
 * @brief Ajoute un état initial à un automate passé en paramètre.
 *
 * Si l'état n'existe pas dans l'automate, l'état est ajouté 
 * automatiquement à l'automate.
 *
 * @param automate Un automate.
 * @param etat_initial L'état initial.
 */ 
void ajouter_etat_initial(
	Automate * automate, int etat_initial
);

/**
 * @brief Renvoie l'ensemble des états d'un automate
 *
 * La mémoire de l'ensemble renvoyé est gérée par l'automate.
 * L'utilisateur ne doit donc pas modifier ou libérer l'ensemble ainsi obtenu.
 *
 * @param automate Un automate.
 * @return L'ensemble des états de l'automate.
 */ 
const Ensemble * get_etats( const Automate* automate );

/**
 * @brief Renvoie l'ensemble des états intiaux d'un automate
 *
 * La mémoire de l'ensemble renvoyé est gérée par l'automate.
 * L'utilisateur ne doit donc pas modifier ou libérer l'ensemble ainsi obtenu.
 *
 * @param automate Un automate.
 * @return L'ensmble des états initiaux de l'automate.
 */ 
const Ensemble * get_initiaux( const Automate* automate );

/**
 * @brief Renvoie l'ensemble des états finaux d'un automate.
 *
 * La mémoire de l'ensemble renvoyé est gérée par l'automate.
 * L'utilisateur ne doit donc pas modifier ou libérer l'ensemble ainsi obtenu.
 *
 * @param automate Un automate.
 * @return L'ensmble des états finaux de l'automate.
 */ 
const Ensemble * get_finaux( const Automate* automate );

/**
 * @brief Renvoie l'alphabet d'un automate.
 *
 * La mémoire de l'ensemble renvoyé est gérée par l'automate.
 *
 * @param automate Un automate.
 * @return L'ensmble des lettres de l'automate.
 */ 
const Ensemble * get_alphabet( const Automate* automate );

/**
 * @brief Renvoie 1 si ('origine', 'lettre', 'fin') est une transition de 
 *        l'automate et 0 sinon.
 *
 * @param automate Un automate.
 * @param origine L'origine de la transition.
 * @param lettre La lettre de la transition.
 * @param fin La fin de la transition.
 * @return 1 ou 0.
 */ 
int est_une_transition_de_l_automate(
	const Automate* automate,
	int origine, char lettre, int fin
);

/**
 * @brief Renvoie 1 si ('etat') est un état de l'automate et 0 sinon.
 *
 * @param automate Un automate.
 * @param etat Un état.
 * @return 1 ou 0.
 */ 
int est_un_etat_de_l_automate( const Automate* automate, int etat );

/**
 * @brief Renvoie 1 si ('etat') est un état initial de l'automate et 0 sinon.
 *
 * @param automate Un automate.
 * @param etat Un état.
 * @return 1 ou 0.
 */ 
int est_un_etat_initial_de_l_automate( const Automate* automate, int etat );

/**
 * @brief Renvoie 1 si ('etat') est un état final initial de l'automate et 0 sinon.
 *
 * @param automate Un automate.
 * @param etat Un état.
 * @return 1 ou 0.
 */ 
int est_un_etat_final_de_l_automate( const Automate* automate, int etat );

/**
 * @brief Renvoie 1 si ('etat') est une lettre de  l'automate et 0 sinon.
 *
 * @param automate Un automate.
 * @param lettre Une lettre.
 * @return 1 ou 0.
 */ 
int est_une_lettre_de_l_automate( const Automate* automate, char lettre );

/**
 * @brief Renvoie l'ensemble des états accéssibles à partir d'un ensemble 
 *        d'états donné en paramètre et en lisant une lettre donnée en 
 *        paramètre.
 *
 * La mémoire de l'ensemble renvoyé par la fonction est laissée à la charge de 
 * l'utilisateur. L'utilisateur devra donc prendre soin de libérer la mémoire
 * à la fin de son utilisation.
 *
 * @param automate Un automate.
 * @param etats_courants L'ensemble des état origines.
 * @param lettre Une lettre.
 * @return L'ensemble des états accessibles.
 */ 
Ensemble * delta(
	const Automate* automate, const Ensemble * etats_courants, char lettre
);

/**
 * @brief Renvoie l'ensemble des états accessibles à partir d'un ensemble 
 *        d'états donné en paramètre et en lisant un mot donné en 
 *        paramètre.
 *
 * La mémoire de l'ensemble renvoyé par la fonction est laissé à la charge de 
 * l'utilisateur. L'utilisateur devra donc prendre soin de libérer la mémoire
 * à la fin de son utilisation.
 *
 * @param automate Un automate.
 * @param etats_courants L'ensemble des état origines.
 * @param mot Le mot à lire.
 * @return L'ensemble des états accessibles.
 */ 
Ensemble * delta_star(
	const Automate* automate, const Ensemble * etats_courants, const char* mot
);

/**
 * @brief Renvoie vrai si le mot passé en paramètre est reconu par l'automate 
 *        passé en paramètre, et renvoie 0 sinon.
 *
 * @param automate Un automate.
 * @param mot Le mot à reconnaître.
 * @return 1 ou 0
 */ 
int le_mot_est_reconnu( const Automate* automate, const char* mot );

/**
 * @brief La fonction passe en revue toutes les transitions de l'automate et 
 *        appelle la fonction passée en paramètre.
 *
 * La fonction qui sera executée (et qui a été passée en paramètre), doit 
 * posséder l'en-tête suivante :
 *   void NOM_FONCTION( int origine, char lettre, int fin, void* data );
 * Les pramètres 'origine', 'lettre' et 'fin' correspondent à l'origine, la 
 * lettre et la fin de la transitions en cours de parcours.
 * Le paramètre 'data' est un pointeur qui sera identique à celui passé par le
 * paramètre 'data' de la fonction pour_toute_transition().
 *
 * @param automate Un automate.
 * @param action La fonction à exécuter.
 * @param data La donnée supplémentaire à passer en paramètre à la fonction 
 *             'action' executée à chaque transition.
 */ 
void pour_toute_transition(
	const Automate* automate,
	void (* action )( int origine, char lettre, int fin, void* data ),
	void* data
);

/**
 * @brief Renvoie l'état ayant le numéro le plus grand de l'automate passé en 
 *        paramètre.
 *
 * Si l'automate est vide (sans état), alors il renvoie INT_MIN.
 * (voir limits.h pour la valeur de INT_MIN).
 *
 * @param automate Un automate.
 */
int get_max_etat( const Automate* automate );

/**
 * @brief Renvoie l'état ayant le numéro le plus petit de l'automate passé en 
 *        paramètre.
 *  
 * Si l'automate est vide (sans état), alors il renvoie INT_MAX.
 * (voir limits.h pour la valeure de INT_MAX).
 *
 * @parama automate Un automate.
 *
 */
int get_min_etat( const Automate* automate );

/**
 * @brief Copie un automate.
 *
 * L'automate copié et l'automate à copier sont indépendants du point de vue de 
 * la mémoire.
 *
 * @param automate L'automate à copier.
 * @return La copie de l'automate.
 */ 
Automate* copier_automate( const Automate* automate );

/**
 * @brief Renvoie l'ensemble des états accessibles à partir d'un état en lisant 
 *        un mot quelcquonque.
 *
 * @param automate Un automate.
 * @param etat L'état de départ.
 * @return L'ensemble des états accessibles.
 */ 
Ensemble* etats_accessibles( const Automate * automate, int etat );

/**
 * @brief Renvoie l'ensemble des états accessibles à partir des états initiaux
 *        en lisant un mot quelconque.
 *
 * @param automate Un automate.
 * @return L'ensemble des états accessibles.
 */ 
Ensemble* accessibles( const Automate * automate );

/**
 * @brief Renvoie l'automate passé en paramètre dont les états non accessibles 
 *        ont été supprimés.
 *
 * @param automate Un automate.
 * @return L'automate accessible.
 */ 
Automate *automate_accessible( const Automate * automate );

/**
 * @brief @todo Renvoie l'automate miroir d'un automate.
 *
 * Il s'agit de l'automate qui reconnaît les mots renversés du langage associé
 * à l'automate passé en paramètre.
 *
 * @param automate Un automate.
 * @return L'automate miroir.
 */ 
Automate *miroir( const Automate * automate);

/**
 * @brief Affiche sur l'entrée standard (stdout) l'automate passé en paramètre.
 *
 * @param automate L'automate à afficher.
 */ 
void print_automate( const Automate * automate );


/**
 * @brief Crée l'intersection de deux automates.
 *
 */
Automate * creer_intersection_des_automates(
	const Automate * automate_1, const Automate * automate_2
);

/**
 * @brief Renvoie l'automate déterministe.
 *
 * @param automate L'automate à déterminiser.
 * @return L'automate déterministe correspondant.
 */ 
Automate * creer_automate_deterministe( const Automate* automate );

/**
 * @brief @todo Renvoie l'automate minimal.
 *
 * @param automate L'automate à minimiser.
 * @return L'automate minimal correspondant.
 */ 
Automate * creer_automate_minimal( const Automate* automate );

/**
 * @brief Renvoie le nombre de transitions d'un automate.
 *
 * @param automate Un automate.
 * @return un entier.
 */
int nombre_de_transitions( const Automate* automate );

#endif
