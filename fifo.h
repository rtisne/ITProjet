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


#ifndef __FIFO_H__
#define __FIFO_H__

#include <stdint.h>

/*
 * Définit le type d'une file first-in first-out contenant des entiers ou 
 * des pointerus vers des stucture plus complexes.
 * La file n'est pas responsable de la mémoire des éléments qui y sont 
 * entreposés.
 */
typedef struct Fifo Fifo;

/*
 * Créer une file vide
 */
Fifo* creer_fifo();

/*
 * Supprimme la mémoire associée à la file.
 * La mamoir associée aux élément de la file ne sont pas supprimé.
 */
void liberer_fifo( Fifo* fifo );

/*
 * Renvoie vrai si la file ne contient pas d'élement.
 */
int est_vide( Fifo* fifo );

/*
 * Ajoute un élément au dessus de la pile.
 */
void ajouter_fifo( Fifo* fifo, intptr_t element );

/*
 * Retire un élément du dessus de la pile et le renvoie.
 */
intptr_t retirer_fifo( Fifo* fifo );

/*
 * Renvoie l'élement qui se trouve au dessus de la pile. L'élément n'est pas
 * retiré de la pile.
 */
intptr_t obtenir_fifo( Fifo* fifo );

#endif
