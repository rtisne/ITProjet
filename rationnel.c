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
   switch(get_etiquette(rat)){
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
   int valeur = 1;
   switch(get_etiquette(rat))
   {
      case LETTRE:
         set_position_min(rat, *valeur);
         set_position_max(rat, *valeur);
         *valeur=*valeur+1;
         break;

      case EPSILON:
         break;

      case UNION:
         numeroter_rationnel_aux(fils_gauche(rat), valeur);
         numeroter_rationnel_aux(fils_droit(rat), valeur);
         set_position_min(rat, get_position_min(fils_gauche(rat)));
         set_position_max(rat, get_position_max(fils_droit(rat)));         
         break;

      case CONCAT:
         numeroter_rationnel_aux(fils_gauche(rat), valeur);
         numeroter_rationnel_aux(fils_droit(rat), valeur);
         set_position_min(rat, get_position_min(fils_gauche(rat)));
         set_position_max(rat, get_position_max(fils_droit(rat)));

         break;

      case STAR:
         numeroter_rationnel_aux(fils_gauche(rat), valeur);
         set_position_min(rat, get_position_min(fils_gauche(rat)));
         set_position_max(rat, get_position_max(fils_gauche(rat)));
         break;
         
      default:
         break;
   }
}

bool contient_mot_vide(Rationnel *rat)
{
   switch(get_etiquette(rat))
   {
      case LETTRE:
      return false;
         break;

      case EPSILON:
         return true;
         break;

      case UNION:
         return (contient_mot_vide(fils_gauche(rat)) 
            || contient_mot_vide(fils_droit(rat)));       
         break;

      case CONCAT:
         return (contient_mot_vide(fils_gauche(rat))
            && contient_mot_vide(fils_droit(rat)));
         break;

      case STAR:
         return true;
         break;
         
      default:
         break;
   }
   return false;
}

Ensemble *premier(Rationnel *rat)
{
   if(!rat)
      return NULL;

  Ensemble *ensemble_premier = creer_ensemble(NULL,NULL,NULL);
  switch(get_etiquette(rat))
   {
      case LETTRE:
         ajouter_element(ensemble_premier, get_position_min(rat));
         break;

      case EPSILON:  
         break;

      case UNION:
         ajouter_elements(ensemble_premier, premier(fils_gauche(rat), ensemble_premier));
         ajouter_elements(ensemble_premier, premier(fils_droit(rat), ensemble_premier));
            
         break;

      case CONCAT:
         if (contient_mot_vide(fils_gauche(rat)))
         {
            ajouter_elements(ensemble_premier, premier(fils_gauche(rat), ensemble_premier));
            ajouter_elements(ensemble_premier, premier(fils_droit(rat), ensemble_premier));
         }
         else
         {
            ajouter_elements(ensemble_premier, premier(fils_gauche(rat), ensemble_premier));
         }
         break;

      case STAR:
         ajouter_elements(ensemble_premier, premier(fils_gauche(rat), ensemble_premier));
         break;
         
      default:
         break;
   }
   return ensemble_premier;
}



Ensemble *dernier(Rationnel *rat)
{
   Ensemble *ensemble_dernier = creer_ensemble(NULL,NULL,NULL);
   switch(get_etiquette(rat))
   {
      case LETTRE:
         ajouter_element(ensemble_dernier, get_position_min(rat));
         break;

      case EPSILON:  
         break;

      case UNION:
         ajouter_elements(ensemble_dernier, dernier(fils_gauche(rat), ensemble_dernier));
         ajouter_elements(ensemble_dernier, dernier(fils_droit(rat), ensemble_dernier));
            
         break;

      case CONCAT:
         if (contient_mot_vide(fils_droit(rat)))
         {
            ajouter_elements(ensemble_dernier, dernier(fils_gauche(rat), ensemble_dernier));
            ajouter_elements(ensemble_dernier, dernier(fils_droit(rat), ensemble_dernier));
           
         }
         else
         {
             ajouter_elements(ensemble_dernier, dernier(fils_droit(rat), ensemble_dernier));
            
         }
         break;

      case STAR:
         ajouter_elements(ensemble_dernier, dernier(fils_gauche(rat), ensemble_dernier));
         break;
         
      default:
         break;
   }
   return ensemble_dernier;
}

Ensemble *suivant(Rationnel *rat, int position)
{
   Ensemble * ensemble_suivant = creer_ensemble(NULL, NULL, NULL);   
   switch(get_etiquette(rat))
   {
      case LETTRE: 
         break;

      case EPSILON:  
         break;

      case UNION:
         ajouter_elements(ensemble_suivant, suivant(fils_gauche(rat), position, ensemble_suivant));
         ajouter_elements(ensemble_suivant, suivant(fils_droit(rat), position, ensemble_suivant));            
         break;

      case CONCAT:
        if (est_dans_l_ensemble(dernier(fils_gauche(rat)),position))
        {
           ajouter_elements(ensemble_suivant, suivant(fils_gauche(rat), position, ensemble_suivant));
           ajouter_elements(ensemble_suivant, suivant(fils_droit(rat), position, ensemble_suivant));
           ajouter_elements(ensemble_suivant, premier(fils_droit(rat)));
        }
        else
        {
            ajouter_elements(ensemble_suivant, suivant(fils_gauche(rat), position, ensemble_suivant));
            ajouter_elements(ensemble_suivant, suivant(fils_droit(rat), position, ensemble_suivant));
        }
         break;

      case STAR:

    
        if (est_dans_l_ensemble(dernier(rat), position))
        {
           ajouter_elements(ensemble_suivant, suivant(fils_gauche(rat), position, ensemble_suivant));
           ajouter_elements(ensemble_suivant, premier(rat));
        }
        else
        {
            ajouter_elements(ensemble_suivant, suivant(fils_gauche(rat), position, ensemble_suivant));
        }
         break;
         
      default:
         break;
   }
   return ensemble_suivant;
}
char lettre_position(Rationnel *rat, int position){
  
   switch(get_etiquette(rat))
   {
      case LETTRE:
        return get_lettre(rat);
        break;

      case CONCAT:

      case UNION:
        if (position < get_position_min(fils_droit(rat)))
        {
          return lettre_position(fils_gauche(rat), position);
        }
        else
        {
          return lettre_position(fils_droit(rat), position);          
        }
        break;

      case STAR:
          return lettre_position(fils_gauche(rat), position);
         break;
      default:
        ERREUR("ERREUR");
        break;
   }

}
Automate *Glushkov(Rationnel *rat)
{
      numeroter_rationnel(rat);
      Automate * automate_final = creer_automate();
       
       Ensemble * ensemble_premier = premier(rat);
       Ensemble * ensemble_dernier = dernier(rat);

       Ensemble_iterateur it1;

       ajouter_etat_initial(automate_final, 0);
     for(
       it1 = premier_iterateur_ensemble( ensemble_dernier );
       ! iterateur_ensemble_est_vide( it1 );
       it1 = iterateur_suivant_ensemble( it1 )
       ){
       ajouter_etat_final(automate_final, get_element(it1));
     }
     
     
     if (contient_mot_vide(rat))
     {
       ajouter_etat_final(automate_final , 0);
     }

     for(
       it1 = premier_iterateur_ensemble(ensemble_premier);
       ! iterateur_ensemble_est_vide( it1 );
       it1 = iterateur_suivant_ensemble( it1 )
       ){
       ajouter_transition(automate_final,0 , lettre_position(rat, get_element(it1)), get_element(it1));

     }



     for (int i = 1; i <= get_position_max(rat);++i)
     {
       
       for(
         it1 = premier_iterateur_ensemble(suivant(rat, i));
         ! iterateur_ensemble_est_vide( it1 );
         it1 = iterateur_suivant_ensemble( it1 )
         ){

         ajouter_transition(automate_final,i , lettre_position(rat, get_element(it1)), get_element(it1));
       }
       
     }
     liberer_ensemble(ensemble_premier);
     liberer_ensemble(ensemble_dernier);
   return automate_final;
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

