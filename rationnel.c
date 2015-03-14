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

#include "rationnel.h"
#include "ensemble.h"
#include "automate.h"
#include "parse.h"
#include "scan.h"
#include "outils.h"

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

int yyparse(Rationnel **rationnel, yyscan_t scanner);

Rationnel *rationnel(Noeud etiquette, char lettre, int position_min, int position_max, void *data, Rationnel *gauche, Rationnel *droit, Rationnel *pere)
{
   Rationnel *rat;
   rat = (Rationnel *) malloc(sizeof(Rationnel));

   rat->etiquette = etiquette;
   rat->lettre = lettre;
   rat->position_min = position_min;
   rat->position_max = position_max;
   rat->data = data;
   rat->gauche = gauche;
   rat->droit = droit;
   rat->pere = pere;
   return rat;
}

Rationnel *Epsilon()
{
   return rationnel(EPSILON, 0, 0, 0, NULL, NULL, NULL, NULL);
}

Rationnel *Lettre(char l)
{
   return rationnel(LETTRE, l, 0, 0, NULL, NULL, NULL, NULL);
}

Rationnel *Union(Rationnel* rat1, Rationnel* rat2)
{
   // Cas particulier où rat1 est vide
   if (!rat1)
      return rat2;

   // Cas particulier où rat2 est vide
   if (!rat2)
      return rat1;
   
   return rationnel(UNION, 0, 0, 0, NULL, rat1, rat2, NULL);
}

Rationnel *Concat(Rationnel* rat1, Rationnel* rat2)
{
   if (!rat1 || !rat2)
      return NULL;

   if (get_etiquette(rat1) == EPSILON)
      return rat2;

   if (get_etiquette(rat2) == EPSILON)
      return rat1;
   
   return rationnel(CONCAT, 0, 0, 0, NULL, rat1, rat2, NULL);
}

Rationnel *Star(Rationnel* rat)
{
   return rationnel(STAR, 0, 0, 0, NULL, rat, NULL, NULL);
}

bool est_racine(Rationnel* rat)
{
   return (rat->pere == NULL);
}

Noeud get_etiquette(Rationnel* rat)
{
   return rat->etiquette;
}

char get_lettre(Rationnel* rat)
{
   assert (get_etiquette(rat) == LETTRE);
   return rat->lettre;
}

int get_position_min(Rationnel* rat)
{
   assert (get_etiquette(rat) == LETTRE);
   return rat->position_min;
}

int get_position_max(Rationnel* rat)
{
   assert (get_etiquette(rat) == LETTRE);
   return rat->position_max;
}

void set_position_min(Rationnel* rat, int valeur)
{
   assert (get_etiquette(rat) == LETTRE);
   rat->position_min = valeur;
   return;
}

void set_position_max(Rationnel* rat, int valeur)
{
   assert (get_etiquette(rat) == LETTRE);
   rat->position_max = valeur;
   return;
}

Rationnel *fils_gauche(Rationnel* rat)
{
   assert((get_etiquette(rat) == CONCAT) || (get_etiquette(rat) == UNION));
   return rat->gauche;
}

Rationnel *fils_droit(Rationnel* rat)
{
   assert((get_etiquette(rat) == CONCAT) || (get_etiquette(rat) == UNION));
   return rat->droit;
}

Rationnel *fils(Rationnel* rat)
{
   assert(get_etiquette(rat) == STAR);
   return rat->gauche;
}

Rationnel *pere(Rationnel* rat)
{
   assert(!est_racine(rat));
   return rat->pere;
}

void print_rationnel(Rationnel* rat)
{
   if (rat == NULL)
   {
      printf("∅");
      return;
   }
   
   switch(get_etiquette(rat))
   {
      case EPSILON:
         printf("ε");         
         break;
         
      case LETTRE:
         printf("%c", get_lettre(rat));
         break;

      case UNION:
         printf("(");
         print_rationnel(fils_gauche(rat));
         printf(" + ");
         print_rationnel(fils_droit(rat));
         printf(")");         
         break;

      case CONCAT:
         printf("[");
         print_rationnel(fils_gauche(rat));
         printf(" . ");
         print_rationnel(fils_droit(rat));
         printf("]");         
         break;

      case STAR:
         printf("{");
         print_rationnel(fils(rat));
         printf("}*");         
         break;

      default:
         assert(false);
         break;
   }
}

Rationnel *expression_to_rationnel(const char *expr)
{
    Rationnel *rat;
    yyscan_t scanner;
    YY_BUFFER_STATE state;

    // Initialisation du scanner
    if (yylex_init(&scanner))
        return NULL;
 
    state = yy_scan_string(expr, scanner);

    // Test si parsing ok.
    if (yyparse(&rat, scanner)) 
        return NULL;
    
    // Libération mémoire
    yy_delete_buffer(state, scanner);
 
    yylex_destroy(scanner);
 
    return rat;
}

void rationnel_to_dot(Rationnel *rat, char* nom_fichier)
{
   FILE *fp = fopen(nom_fichier, "w+");
   rationnel_to_dot_aux(rat, fp, -1, 1);
}

int rationnel_to_dot_aux(Rationnel *rat, FILE *output, int pere, int noeud_courant)
{   
   int saved_pere = noeud_courant;

   if (pere >= 1)
      fprintf(output, "\tnode%d -> node%d;\n", pere, noeud_courant);
   else
      fprintf(output, "digraph G{\n");
   
   switch(get_etiquette(rat))
   {
      case LETTRE:
         fprintf(output, "\tnode%d [label = \"%c-%d\"];\n", noeud_courant, get_lettre(rat), rat->position_min);
         noeud_courant++;
         break;

      case EPSILON:
         fprintf(output, "\tnode%d [label = \"ε-%d\"];\n", noeud_courant, rat->position_min);
         noeud_courant++;
         break;

      case UNION:
         fprintf(output, "\tnode%d [label = \"+ (%d/%d)\"];\n", noeud_courant, rat->position_min, rat->position_max);
         noeud_courant = rationnel_to_dot_aux(fils_gauche(rat), output, noeud_courant, noeud_courant+1);
         noeud_courant = rationnel_to_dot_aux(fils_droit(rat), output, saved_pere, noeud_courant+1);
         break;

      case CONCAT:
         fprintf(output, "\tnode%d [label = \". (%d/%d)\"];\n", noeud_courant, rat->position_min, rat->position_max);
         noeud_courant = rationnel_to_dot_aux(fils_gauche(rat), output, noeud_courant, noeud_courant+1);
         noeud_courant = rationnel_to_dot_aux(fils_droit(rat), output, saved_pere, noeud_courant+1);
         break;

      case STAR:
         fprintf(output, "\tnode%d [label = \"* (%d/%d)\"];\n", noeud_courant, rat->position_min, rat->position_max);
         noeud_courant = rationnel_to_dot_aux(fils(rat), output, noeud_courant, noeud_courant+1);
         break;
         
      default:
         assert(false);
         break;
   }
   if (pere < 0)
      fprintf(output, "}\n");
   return noeud_courant;
}

Rationnel *miroir_expression_rationnelle(Rationnel *rat)
{
   if(!rat)
      return NULL;

   Rationnel *f1, *f2;
   switch(get_etiquette){
      case EPSILON:
         return Epsilon();
         break;
      case LETTRE:
         return Lettre(get_lettre(rat));
         break;
      case UNION:
         f1 = miroir_expression_rationnelle(fils_gauche(rat));
         f2 = miroir_expression_rationnelle(fils_droit(rat));
         return Union(f1, f2);
         break;
      case CONCAT:
         f1 = miroir_expression_rationnelle(fils_gauche(rat));
         f2 = miroir_expression_rationnelle(fils_droit(rat));
         return Concat(f2, f1);
         break;
      case STAR:
         f1 = miroir_expression_rationnelle(fils(rat));
         return Star(f1);
         break;
      default:
         assert(false);
         break;
   }
}

void numeroter_rationnel(Rationnel *rat)
{
   A_FAIRE;
}

bool contient_mot_vide(Rationnel *rat)
{
   A_FAIRE_RETURN(true);
}

Ensemble *premier(Rationnel *rat)
{
   A_FAIRE_RETURN(NULL);
}

Ensemble *dernier(Rationnel *rat)
{
   A_FAIRE_RETURN(NULL);
}

Ensemble *suivant(Rationnel *rat, int position)
{
   A_FAIRE_RETURN(NULL);
}

Automate *Glushkov(Rationnel *rat)
{
   A_FAIRE_RETURN(NULL);
}

bool meme_langage (const char *expr1, const char* expr2)
{
   A_FAIRE_RETURN(true);
}

Systeme systeme(Automate *automate)
{
   A_FAIRE_RETURN(NULL);
}

void print_ligne(Rationnel **ligne, int n)
{
   for (int j = 0; j <=n; j++)
      {
         print_rationnel(ligne[j]);
         if (j<n)
            printf("X%d\t+\t", j);
      }
   printf("\n");
}

void print_systeme(Systeme systeme, int n)
{
   for (int i = 0; i <= n-1; i++)
   {
      printf("X%d\t= ", i);
      print_ligne(systeme[i], n);
   }
}

Rationnel **resoudre_variable_arden(Rationnel **ligne, int numero_variable, int n)
{
   A_FAIRE_RETURN(NULL);
}

Rationnel **substituer_variable(Rationnel **ligne, int numero_variable, Rationnel **valeur_variable, int n)
{
   A_FAIRE_RETURN(NULL);
}

Systeme resoudre_systeme(Systeme systeme, int n)
{
   A_FAIRE_RETURN(NULL);
}

Rationnel *Arden(Automate *automate)
{
   A_FAIRE_RETURN(NULL);
}

