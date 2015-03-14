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


#include "outils.h"
#include "fifo.h"

typedef struct List List;

struct List {
	List * next;
	intptr_t element;
};

struct Fifo {
	List * list;
};

List* allouer_list( List * next, intptr_t element ){
	List* res = (List*) xmalloc( sizeof(List) );
	res->next = next;
	res->element = element;
	return res;
}

void liberer_list( List * list ){
	xfree( list );
}

void ajouter_fifo( Fifo* fifo, intptr_t element ){
	fifo->list = allouer_list( fifo->list, element );
}

intptr_t retirer_fifo( Fifo* fifo ){
	intptr_t res = fifo->list->element;
	List* tmp = fifo->list;
	fifo->list = fifo->list->next;
	liberer_list( tmp );
	return res;
}

intptr_t obtenir_fifo( Fifo* fifo ){
	return fifo->list->element;
}

int est_vide( Fifo* fifo ){
	return fifo->list == NULL;
}

Fifo* creer_fifo(){
	Fifo* res = xmalloc( sizeof(Fifo) );
	res->list = NULL;
	return res;
}

void vider_list( List * list ){
	if( list ){
		vider_list( list->next );
		xfree( list );
	}
}

void liberer_fifo( Fifo* file ){
	vider_list( file->list );
	xfree( file );
}
