/*
 *   Ce fichier fait partie d'un projet de programmation donné en Licence 3 
 *   à l'Université de Bordeaux.
 *
 *   Copyright (C) 2015 Giuliana Bianchi, Adrien Boussicault, Thomas Place, Marc Zeitoun
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

/** @file rationnel.h */ 

#ifndef __RATIONNEL_H__
#define __RATIONNEL_H__
#include <stdbool.h>
#include <stdio.h>
#include "automate.h"
#include "ensemble.h"

/**
 * @brief Type d'expression.
 *
 * Les différents types pour une expression sont:
 * - EPSILON (mot vide),
 * - LETTRE (une lettre),
 * - STAR (l'étoile d'une sous-expression)
 * - UNION (l'union de 2 sous-expressions)
 * - CONCAT (la concaténation de 2 sous-expressions)
 * 
 * S'il est nécessaire de représenter l'expression vide, on utilise NULL comme expression rationnelle (voir @ref Rationnel).
 */
typedef enum Noeud {EPSILON,	//!< Mot vide
                    LETTRE,		//!< Lettre
                    STAR,		//!< Etoile
                    UNION, 		//!< Union
                    CONCAT 		//!< Concaténation
} Noeud;

/**
 * @brief Le type décrivant une expression rationnelle. 
 *
 * Une expression contient:
 * - un champ étiquette de type Noeud, donnant le type de l'expression.
 *   (l'ensemble vide peut être représenté par NULL, si besoin).
 * - un caractère, dans le cas où l'expression est un noeud LETTRE.
 * - une position_min, et une position_max.
 */
typedef struct Rationnel {
   Noeud etiquette;				//!< Le type de l'expression

   char lettre;					//!< Si l'expression est un neoud LETTRE, le caractère assosié

   struct Rationnel *gauche;	//!<Fils gauche, utilisé pour les noeuds
   								//!binaires et l'étoile:
   								//!- Si le noeud est de type UNION ou CONCAT, représente la sous-expression gauche.
                                //!- Si le noeud est de type STAR, représente la sous-expression, "sous" l'étoile.
                                //!- \b NULL si le noeud est de type EPSILON ou LETTRE.

   struct Rationnel *droit;	    //!<Fils droit, utilisé pour les noeuds binaires \b seulement:
   								//!- Si le noeud est de type UNION ou CONCAT, représente la sous-expression droite.
                                //!- \b NULL si le noeud est de type EPSILON, LETTRE ou STAR.

   struct Rationnel *pere;		//!< Le noeud père.
   int position_min;			//!< Position utilisée pour l'algorithme de Glushkov:
								//!- Pour une lettre, désigne la position de la lettre, en commençant la numéroration par 1. Pour une lettre, 
								//!position_min a donc la même valeur que position_max.
                                //!- Pour un noeud interne, désigne la position <b>la plus petite</b> d'une lettre de la
                                //!sous-expression correspondant à ce noeud.
 								//!- Non utilisée pour EPSILON.
   int position_max;			//!< Position utilisée pour l'algorithme de Glushkov:
								//!- Pour une lettre, désigne la position de la lettre, en commençant la numéroration par 1. Pour une lettre, 
								//!position_max a donc la même valeur que position_min.
                                //!- Pour un noeud interne, désigne la position <b>la plus grande</b> d'une lettre de la
                                //!sous-expression correspondant à ce noeud. Par exemple, pour l'expression \f$(a+b)^*\f$ les champs position_min et
                                //!position_max valent tous les deux 1 pour la sous-expression \f$a\f$, tous les deux 2 pour la sous-expression \f$b\f$,
                                //!mais position_min vaut 1 et position_max vaut 2 pour la sous-expression \f$a+b\f$, ainsi que pour l'expression
                                //!complète \f$(a+b)^*\f$.
 								//!- Non utilisée pour EPSILON.
   void * data;					//!< Donnée additionnelle, non utilisée dans le cadre de ce projet.
} Rationnel;

/**
 *  @brief Un système d'équations est représenté par un tableau à 2 dimensions. Chaque case du tableau contient un pointeur sur un @ref Rationnel.
 *
 * Par exemple, le système
 * \f{alignat*}{{2}
 * 		X_0 &=\ &  a\cdot X_0 +\ &b\cdot X_1 + \varepsilon\\
 *      X_1 &=&& c\cdot X_1 + d^*
 * \f}
 * sera représenté par le tableau <code>systeme</code> à 2 lignes et 3 colonnes contenant les pointeurs sur les @ref Rationnel suivants:
 * \f{array}{{|c|c|c|}
 * \hline
 *  a & b & \varepsilon\\
 * \hline
 * \emptyset & c & d^*\\
 * \hline
 * \f}
 * Ainsi, <code>systeme[0][2]</code> contient un pointeur sur l'expression rationnelle \f$\varepsilon\f$, et <code>systeme[1][0]</code> contient le pointeur NULL représentant l'expression \f$\emptyset\f$.
 */
typedef Rationnel *** Systeme;

/**
 * @brief Alloue et remplit une structure Rationnel, et renvoie son adresse.
 * @param etiquette Le type de noeud.
 * @param lettre Le caractère, dans le cas d'un noeud LETTRE.
 * @param position_min Le champ position_min
 * @param position_max Le champ position_max
 * @param data Le champ data
 * @param gauche Le fils gauche
 * @param droit Le fils droit
 * @param pere Le père
*/
Rationnel *rationnel(Noeud etiquette, char lettre, int position_min, int position_max, void *data, Rationnel *gauche, Rationnel *droit, Rationnel *pere);


/**
 * @brief Alloue et remplit une structure Rationnel initialisée à une feuille "mot vide".
 */   
Rationnel *Epsilon();

/**
 * @brief Alloue et remplit une structure Rationnel, initialisée à une feuille "lettre".
 * @param lettre La lettre pour initialiser la structure.
 */   
Rationnel *Lettre(char lettre);

/**
 * @brief Construit l'union de deux rationnels.
 * @param rat1 Pointeur sur le premier rationnel
 * @param rat2 Pointeur sur le second rationnel
 */   
Rationnel *Union(Rationnel* rat1, Rationnel* rat2);

/**
 * @brief Construit le produit (la concaténation) de deux rationnels.
 * @param rat1 Pointeur sur le premier rationnel
 * @param rat2 Pointeur sur le second rationnel
 */   
Rationnel *Concat(Rationnel* rat1, Rationnel* rat2);

/**
 * @brief Construit l'étoile d'un rationnel.
 * @param rat Pointeur sur le rationnel dont on construit l'étoile.
 */   
Rationnel *Star(Rationnel* rat);

/**
 * @brief Teste si un pointeur sur un rationnel représente la racine.
 * @param rat Pointeur sur le rationnel à tester.
 */   
bool est_racine(Rationnel* rat);

/**
 * @brief Renvoie le type de noeud d'un rationnel.
 * @param rat Pointeur sur le rationnel.
 * @return Le type @ref Noeud de l'expression.
 */   
Noeud get_etiquette(Rationnel* rat);

/**
 * @brief Renvoie la lettre portée par une expression de type LETTRE.
 * @param rat Pointeur sur le rationnel, qui doit être de type LETTRE.
 * @return Un char, le caractère portée par l'expression.
 */   
char get_lettre(Rationnel* rat);

/**
 * @brief Renvoie la position minimale d'une expression rationnelle.
 * @param rat Pointeur sur le rationnel.
 * @return Un entier donnant la position minimale contenur dans l'expression rationnelle.
 */   
int get_position_min(Rationnel* rat);

/**
 * @brief Renvoie la position maximale d'une expression rationnelle.
 * @param rat Pointeur sur le rationnel.
 * @return Un entier donnant la position maximale contenue dans l'expression rationnelle.
 */   
int get_position_max(Rationnel* rat);

/**
 * @brief Affecte la position minimale d'une expression rationnelle.
 * @param rat Pointeur sur le rationnel.
 * @param valeur Valeur à laquelle affecter la position minimale.
 */   
void set_position_min(Rationnel* rat, int valeur);

/**
 * @brief Affecte la position maximale d'une expression rationnelle.
 * @param rat Pointeur sur le rationnel.
 * @param valeur Valeur à laquelle affecter la position maximale.
 */
void set_position_max(Rationnel* rat, int valeur);

/**
 * @brief Renvoie le fils gauche d'une expression rationnelle UNION ou CONCAT.
 * @param rat Pointeur sur le rationnel.
 */   
Rationnel *fils_gauche(Rationnel* rat);

/**
 * @brief Renvoie le fils droit d'une expression rationnelle UNION ou CONCAT.
 * @param rat Pointeur sur le rationnel.
 */   
Rationnel *fils_droit(Rationnel* rat);

/**
 * @brief Renvoie le fils d'une expression rationnelle STAR.
 * @param rat Pointeur sur le rationnel.
 */   
Rationnel *fils(Rationnel* rat);

/**
 * @brief Renvoie le pere d'une (sous-)expression rationnelle.
 * @param rat Pointeur sur le rationnel.
 */   
Rationnel *pere(Rationnel* rat);

/**
 * @brief Affiche une expression rationnelle, avec la syntaxe habituelle.
 * Pour plus de lisibilité, différents jeux de parenthèses sont utilisés.
 * @param rat Pointeur sur le rationnel.
 */   
void print_rationnel(Rationnel* rat);

/**
 * @brief Construit le rationnel correspondant à une expression expr, donnée sous forme d'une chaîne de caractères.
 *
 * La syntaxe pour les expressions est la suivante:
 * - les lettres sont données en minuscule.
 * - la concaténation se note par un point '.'
 * - l'union se note par '+'.
 * - l'étoile se note par '*'. 
 * - on peut parenthéser une sous-expression avec les parenthèses '('...)'.
 * Le parseur ne prend pas en compte le mot vide ni le langage vide.
 * @param expr: expression rationnelle donnée avec la syntaxe ci-dessus.
 */
Rationnel *expression_to_rationnel(const char *expr);

/**
 * @brief Exporte l'arbre syntaxique d'une expression rationnelle dans un fichier dot. Dans chaque noeud, le type du noeud ainsi que les positions min et max sont indiquées.
 * 
 * @param rat Pointeur sur le rationnel.
 * @param nom_fichier Nom du fichier vers lequel exporter.
 *
 *
 * Les fichiers dot sont visualisables par la commande <code>dotty fichier.dot</code>. On peut également les exporter en pdf par la commande <code>dot
 * -Tpdf fichier.dot -o fichier.pdf</code>. Consulter la documentation de <a href="http://www.graphviz.org">Graphviz</a> pour plus de détails.
 */
void rationnel_to_dot(Rationnel *rat, char* nom_fichier);

/**
 * @brief Fonction auxiliaire d'export d'un arbre syntaxique d'une expression vers un fichier dot.
 *
 * Pour visualiser une expression rationnelle, ne pas utiliser cette fonction mais plutôt la fonction @ref rationnel_to_dot.
 * 
 * @param rat Pointeur sur le rationnel.
 * @param output FILE* vers lequel exporter, qui doit avoir été ouvert en écriture.
 * @param pere Le numéro du père du noeud courant, utilisé récursivement pour dessiner les liens.
 * @param noeud_courant Le numéro du noeud courant, utilisé récursivement.
 * @return La prochaine valeur de noeud courant libre.
 */   
int rationnel_to_dot_aux(Rationnel *rat, FILE *output, int pere, int noeud_courant);

/**
 * @brief @todo Affecte les positions <b>position_min</b> et
 * <b>position_max</b> de toutes les sous-expressions d'une expression
 * rationnelle (voir Rationnel::position_min et
 * Rationnel::position_max pour ces champs, ou la structure Rationnel). 
 * Une expression complète sera numérotée à partir de la position 1 pour sa lettre la plus à gauche.
 * @param rat Pointeur sur le rationnel.
 */   
void numeroter_rationnel(Rationnel *rat);

/**
 * @brief @todo Teste si le langage d'une expression rationnelle contient le mot vide.
 * @param rat L'expression rationnelle à tester.
 * @return true si le langage de l'expression contient le mot vide.
 */
bool contient_mot_vide(Rationnel *rat);

/**
 * @brief @todo Calcule l'ensemble des positions des lettres pouvant apparaître comme première lettre d'un mot du langage d'une expression rationnelle.
 * @param rat L'expression rationnelle.
 * @return L'ensemble des positions des lettres pouvant apparaître comme première lettre d'un mot du langage de l'expression.
 */
Ensemble *premier(Rationnel *rat);

/**
 * @brief @todo Calcule l'ensemble des positions des lettres pouvant apparaître comme dernière lettre d'un mot du langage d'une expression rationnelle.
 * @param rat L'expression rationnelle.
 * @return L'ensemble des positions des lettres pouvant apparaître comme dernière lettre d'un mot du langage de l'expression.
 */
Ensemble *dernier(Rationnel *);

/**
 * @brief @todo Calcule l'ensemble des positions des lettres pouvant suivre une position donnée dans au moins un mot du langage de l'expression. 
 * @param rat Une expression rationnelle.
 * @param position Un entier désignant une position dans l'expression rationnelle.
 * @return L'ensemble des positions des lettres pouvant suivre la position donnée en second argument dans au moins un mot du langage de l'expression.
 */
Ensemble *suivant(Rationnel *, int);

/**
 * @brief @todo
 * @brief Retourne l'automate de Glushkov associé à une expression rationnelle.
 * @param rat Une expression rationnelle.
 * @return L'automate de Glushkov associé à l'expression rationnelle. Ses états seront numérotés par des entiers commençant à 0, l'état initial.
 */
Automate *Glushkov(Rationnel *rat);

/**
 * @brief @todo
 * Teste si deux expressions reconnaissent le même langage.
 *
 * @param expr1 La première expression.
 * @param expr2 La deuxième expression.
 * @result true ou false.
 */
bool meme_langage (const char *expr1, const char* expr2);

/**
 * @brief @todo Construit le système d'équations de langages associé à un automate. Voir @ref Systeme pour la représentation de ce système.
 * @param automate L'automate à transformer en système, en supposant ses états
 * numérotés de 0 à n-1.
 * @return Le système d'équations de langages associé à l'automate.
 */
Systeme systeme(Automate *automate);

/**
 * @brief Affiche une ligne d'un système d'équations.
 * @param ligne La ligne à afficher.
 * @param nb_vars Le nombre de variables de cette ligne.
 */
void print_ligne(Rationnel **ligne, int nb_vars);

/**
 * @brief Affiche un système d'équations.
 * @param sys Le système d'équations à afficher.
 * @param nb_vars Le nombre de variables (et donc aussi le nombre d'équations).
 */
void print_systeme(Systeme systeme, int taille);

/**
 * @brief @todo Résout une équation de langages en utilisant le lemme d'Arden: si \f$U\f$ et \f$V\f$ sont deux langages et \f$U\f$ ne contient pas le mot vide, et si \f$X\f$ est un langage tel que \f$X=UX+V\f$, alors \f$X=U^*V\f$. 
 * @param ligne La ligne codant l'équation de langages.
 * @param numero_variable Le numéro de la variable jouant le rôle de X dans le lemme d'Arden, à exprimer en fonction des autres variables. Si ce numéro est \f$i\f$, le membre gauche de l'équation est donc \f$X_i\f$ (ce membre gauche n'est pas codé dans le paramètre 'ligne').
 * @param nb_vars Le nombre de variables.
 * @return Une ligne codant l'équation résultat, dans laquelle la variable de numéro 'numero_variable' a été exprimée en fonction des autres.
 *
 * Par exemple, si 'nb_vars' vaut 3, il y a 3 variables \f$X_0\f$, \f$X_1\f$ et \f$X_2\f$. Si en plus l'argument 'numero_variable' vaut 1 et l'argument 'ligne' vaut \f$\{a, b, \sf{NULL},\varepsilon \}\f$, alors l'équation représentée par ces arguments est \f[X_1 = a\cdot X_0 + b\cdot X_1 + \emptyset\cdot X_2 + \varepsilon\f] ou encore \f[X_1 = a\cdot X_0 + b\cdot X_1 +  \varepsilon.\f] Le lemme d'Arden donne \f[X_1 = b^*(a\cdot X_0+\varepsilon)=b^*a\cdot X_0+b^*.\f] La fonction doit donc retourner la ligne \f[\{b^*a, {\sf NULL}, {\sf NULL}, b^* \}.\f]
 */
Rationnel **resoudre_variable_arden(Rationnel **ligne, int numero_variable, int nb_vars);

/**
 * @brief @todo Substitue dans une ligne une variable par sa valeur en fonction des autres variables.`
 * @param ligne La ligne dans laquelle effectuer la substitution.
 * @param numero_variable Le numéro de la variable à substituer par une valeur dépendant des autres variables.
 * @param valeur_variable La valeur, en fonction des autres variables, par laquelle remplacer dans la ligne donnée en premier argument la variable dont le numéro est donné en second argument.
 * @return La ligne résultant de la substitution dans 'ligne' de la variable de numéro 'numero_variable' par la valeur 'valeur_variable'.
*/
Rationnel **substituer_variable(Rationnel **ligne, int numero_variable, Rationnel **valeur_variable, int n);

/**
 * @brief @todo Résout un système d'équations de langages.
 * @param sys Le système à résoudre.
 * @param nb_vars Le nombre de variables.
 * @return Le système d'équations représentant la valeur de chaque variable comme un langage rationnel.
 */
Systeme resoudre_systeme(Systeme sys, int nb_vars);

/**
 * @brief @todo Convertit un automate en expression rationnelle.
 * @param automate L'automate d'entrée.
 * @return Une expression rationnelle décrivant le langage reconnu par l'automate.
 */
Rationnel *Arden(Automate *automate);

#endif
