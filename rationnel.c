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
//   assert (get_etiquette(rat) == LETTRE);
   return rat->lettre;
}

int get_position_min(Rationnel* rat)
{
//   assert (get_etiquette(rat) == LETTRE);
   return rat->position_min;
}

int get_position_max(Rationnel* rat)
{
//   assert (get_etiquette(rat) == LETTRE);
   return rat->position_max;
}

void set_position_min(Rationnel* rat, int valeur)
{
//   assert (get_etiquette(rat) == LETTRE);
   rat->position_min = valeur;
   return;
}

void set_position_max(Rationnel* rat, int valeur)
{
//   assert (get_etiquette(rat) == LETTRE);
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
   switch(get_etiquette(rat))
   {
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

int numeroter_rationnel_recursif(Rationnel *rat)
{
    if(!rat)
        return 0;

    switch(get_etiquette(rat))
    {
        case EPSILON:
            // le max du rationnel = min du rationnel
            set_position_max(rat, get_position_min(rat));
            break;

        case LETTRE:
            // le max du rationnel = min du rationnel
            set_position_max(rat, get_position_min(rat));
            break;

        case UNION:
            // le min du fils gauche du rationnel = le min du rationnel
            set_position_min(fils_gauche(rat), get_position_min(rat));
            // le min du fils droit du rationnel = le max du fils gauche du rationnel + 1
            set_position_min(fils_droit(rat), numeroter_rationnel_recursif(fils_gauche(rat)) + 1);
            // le max du rationnel = le max du fils droit
            set_position_max(rat, numeroter_rationnel_recursif(fils_droit(rat)));
            break;

        case CONCAT:
            // le min du fils gauche du rationnel = le min du rationnel
            set_position_min(fils_gauche(rat), get_position_min(rat));
            // le min du fils droit du rationnel = le max du fils gauche du rationnel + 1
            set_position_min(fils_droit(rat), numeroter_rationnel_recursif(fils_gauche(rat)) + 1);
            // le max du rationnel = le max du fils droit
            set_position_max(rat, numeroter_rationnel_recursif(fils_droit(rat)));
            break;

        case STAR:
            // le min du fils du rationnel = le min du rationnel
            set_position_min(fils(rat), get_position_min(rat));
            // le max du rationnel = le max du fils droit
            set_position_max(rat, numeroter_rationnel_recursif(fils(rat)));
            break;

        default:
            assert(false);
            break;
    }
    return get_position_max(rat);
}

void numeroter_rationnel(Rationnel *rat)
{
    if(!rat)
        return;

    set_position_min(rat, 1);
    numeroter_rationnel_recursif(rat);
}

bool contient_mot_vide(Rationnel *rat)
{
    if(!rat)
        return NULL;

    switch(get_etiquette(rat))
    {
        case EPSILON:
            return true;
            break;

        case LETTRE:
            return false;
            break;

        case UNION:
            return contient_mot_vide(fils_gauche(rat))
            || contient_mot_vide(fils_droit(rat));
            break;

        case CONCAT:
            return contient_mot_vide(fils_gauche(rat))
            && contient_mot_vide(fils_droit(rat));
            break;

        case STAR:
            return true;
            break;

        default:
            assert(false);
            break;
    }
}

void premier_recursif(Ensemble *ensemble_premier, Rationnel *rat)
{
    if(!rat)
        return;

    switch(get_etiquette(rat))
    {
        case EPSILON:
            break;

        case LETTRE:
            ajouter_element(ensemble_premier, get_position_min(rat));
            break;

        case UNION:
            premier_recursif(ensemble_premier, fils_gauche(rat));
            premier_recursif(ensemble_premier, fils_droit(rat));
            break;

        case CONCAT:
            premier_recursif(ensemble_premier, fils_gauche(rat));
			if(contient_mot_vide(fils_gauche(rat)))
				premier_recursif(ensemble_premier, fils_droit(rat));
            break;

        case STAR:
            premier_recursif(ensemble_premier, fils(rat));
            break;

        default:
            assert(false);
            break;
    }
}

Ensemble *premier(Rationnel *rat)
{
    if(!rat)
        return NULL;

    Ensemble *ensemble_premier = creer_ensemble(NULL, NULL, NULL);
    premier_recursif(ensemble_premier, rat);
    return ensemble_premier;
}

void dernier_recursif(Ensemble *ensemble_dernier, Rationnel *rat)
{
    if(!rat)
        return;

    switch(get_etiquette(rat))
    {
        case EPSILON:
            break;

        case LETTRE:
            ajouter_element(ensemble_dernier, get_position_min(rat));
            break;

        case UNION:
            dernier_recursif(ensemble_dernier, fils_gauche(rat));
            dernier_recursif(ensemble_dernier, fils_droit(rat));
            break;

        case CONCAT:
			if(contient_mot_vide(fils_droit(rat)))
				dernier_recursif(ensemble_dernier, fils_gauche(rat));
            dernier_recursif(ensemble_dernier, fils_droit(rat));
            break;

        case STAR:
            dernier_recursif(ensemble_dernier, fils(rat));
            break;

        default:
            assert(false);
            break;
    }
}

Ensemble *dernier(Rationnel *rat)
{
   if(!rat)
        return NULL;

    Ensemble *ensemble_dernier = creer_ensemble(NULL, NULL, NULL);
    dernier_recursif(ensemble_dernier, rat);
    return ensemble_dernier;
}

Ensemble *suivant_recursif(Ensemble *ensemble_suivant, Rationnel *rat, int position)
{
    if(!rat)
        return NULL;

    switch(get_etiquette(rat))
    {
        case EPSILON:
            break;

        case LETTRE:
            break;

        case UNION:
            ajouter_elements(ensemble_suivant, suivant_recursif(ensemble_suivant, fils_gauche(rat), position));
            ajouter_elements(ensemble_suivant, suivant_recursif(ensemble_suivant, fils_droit(rat), position));
            break;

        case CONCAT:
			ajouter_elements(ensemble_suivant, suivant_recursif(ensemble_suivant, fils_gauche(rat), position));
            ajouter_elements(ensemble_suivant, suivant_recursif(ensemble_suivant, fils_droit(rat), position));
            if (est_dans_l_ensemble(dernier(fils_gauche(rat)), position))
				ajouter_elements(ensemble_suivant, premier(fils_droit(rat)));
            break;

        case STAR:
            if (est_dans_l_ensemble(dernier(rat), position))
				ajouter_elements(ensemble_suivant, premier(rat));
			ajouter_elements(ensemble_suivant, suivant_recursif(ensemble_suivant, fils(rat), position));
            break;

        default:
            assert(false);
            break;
    }
    return ensemble_suivant;
}

Ensemble *suivant(Rationnel *rat, int position)
{
    if(!rat)
        return NULL;

    if(position < 1 || position > get_position_max(rat))
        return NULL;

    Ensemble *ensemble_suivant = creer_ensemble(NULL, NULL, NULL);
    suivant_recursif(ensemble_suivant, rat, position);
    return ensemble_suivant;
}

char lettre_position(Rationnel *rat, int position)
{
    switch(get_etiquette(rat))
    {
        case LETTRE:
            return get_lettre(rat);
            break;

        case CONCAT:

        case UNION:
            if (position < get_position_min(fils_droit(rat)))
                return lettre_position(fils_gauche(rat), position);
            else
                return lettre_position(fils_droit(rat), position);
            break;

        case STAR:
            return lettre_position(fils(rat), position);
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
        )
    {
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
        )
    {
        ajouter_transition(automate_final,0 , lettre_position(rat, get_element(it1)), get_element(it1));
    }

    for(int i = 1; i <= get_position_max(rat);++i)
    {

        for(
            it1 = premier_iterateur_ensemble(suivant(rat, i));
            ! iterateur_ensemble_est_vide( it1 );
            it1 = iterateur_suivant_ensemble( it1 )
            )
        {
            ajouter_transition(automate_final,i , lettre_position(rat, get_element(it1)), get_element(it1));
        }
    }

    liberer_ensemble(ensemble_premier);
    liberer_ensemble(ensemble_dernier);

    return automate_final;
}

bool comparer_automates(const Automate * a1, const Automate * a2)
{
    // on retourne faux si les etats, les alphabets, les etats initiaux, les etats finaux
    // et le nombre de transitions des deux automates ne sont pas identique
    if((comparer_ensemble(get_etats(a1), get_etats(a2)) != 0) ||
       (comparer_ensemble(get_alphabet(a1), get_alphabet(a2)) != 0) ||
       (comparer_ensemble(get_initiaux(a1), get_initiaux(a2)) != 0) ||
       (comparer_ensemble(get_finaux(a1), get_finaux(a2)) != 0) ||
       (nombre_de_transitions(a1) != nombre_de_transitions(a2)))
        return false;

    // on compare les transitions des deux automates
    Table_iterateur it_a1;
    Ensemble_iterateur it_fins;
    for(it_a1 = premier_iterateur_table(a1->transitions);
		! iterateur_est_vide(it_a1);
		it_a1 = iterateur_suivant_table(it_a1))
	{
	    Cle *cle = (Cle *)get_cle(it_a1);
        int origine = cle->origine;
        char lettre = cle->lettre;

        const Ensemble *fins = (Ensemble *)get_valeur(it_a1);

         for(it_fins = premier_iterateur_ensemble(fins);
            ! iterateur_ensemble_est_vide(it_fins);
            it_fins = iterateur_suivant_ensemble(it_fins))
        {
            int fin = (int)get_element(it_fins);
            if(!est_une_transition_de_l_automate(a2, origine, lettre, fin))
                return false;
        }
	}
    return true;
}

bool meme_langage(const char *expr1, const char* expr2)
{
    // pour verifier si les epxressions rationnelles décrivent le même language il faut comparer leur automate minimal
    // récupération des expressions rationnelles
    Rationnel * r1 = expression_to_rationnel(expr1);
    Rationnel * r2 = expression_to_rationnel(expr2);

    // numérotation des expressions rationnelles
    numeroter_rationnel(r1);
    numeroter_rationnel(r2);

    // récupération des automates de Glushkov à partir des expressions rationnelles
    Automate * g1 = Glushkov(r1);
    Automate * g2 = Glushkov(r2);

    // on récupère les automates minimaux à partir des automates de Glushkov
    Automate * a1 = creer_automate_minimal(g1);
    Automate * a2 = creer_automate_minimal(g2);

    // on compare les deux automates
    bool res = comparer_automates(a1, a2);

    liberer_automate(a1);
    liberer_automate(a2);
    liberer_automate(g1);
    liberer_automate(g2);

    return res;
}

void action_lettre_system(int origine, char lettre, int fin, void* data)
{
    Systeme * sys = (Systeme *)data;
    (*sys)[origine][fin] = Lettre(lettre);
}

Systeme systeme(Automate *automate)
{
    int nbEtat = taille_ensemble(get_etats(automate));
    // le nombre d'equation du systeme = nombre d'etats
    Systeme s = malloc(nbEtat * sizeof(*s));
    for(int i = 0; i < nbEtat; ++i)
    {
        // le nombre d'etats + epsilon
        s[i] = malloc((nbEtat + 1) * sizeof(*s[i]));
        for(int j = 0 ; j <= nbEtat; ++j)
            s[i][j] = NULL;
    }

    // on complete le systeme en parcourant chaque transition de l'automate
    pour_toute_transition(automate, action_lettre_system, &s);

    // on ajoute les epsilons au systeme en parcourant les états finaux de l'automate
    Ensemble_iterateur it;
    Ensemble * fins = (Ensemble*) get_finaux(automate);
    for(
        it = premier_iterateur_ensemble( fins );
        ! iterateur_ensemble_est_vide( it );
        it = iterateur_suivant_ensemble( it )
    ){
        int ligne = (int)get_element(it);
        s[ligne][nbEtat] = Epsilon();
    }

    return s;
}

void print_ligne(Rationnel **ligne, int n)
{
    for(int j = 0; j <= n; j++)
    {
        print_rationnel(ligne[j]);
        if(j < n)
            printf("X%d\t+\t", j);
    }
    printf("\n");
}

void print_systeme(Systeme systeme, int n)
{
    for(int i = 0; i <= n-1; i++)
    {
        printf("X%d\t= ", i);
        print_ligne(systeme[i], n);
    }
}

Rationnel **resoudre_variable_arden(Rationnel **ligne, int numero_variable, int n)
{
    if(!ligne[numero_variable])
        return ligne;
    Rationnel* rat = ligne[numero_variable];
    ligne[numero_variable] = NULL;
    rat = Star(rat);
    ligne[n] = ligne[n] ? Union(ligne[n], rat) : rat;
    return ligne;
}

Rationnel **substituer_variable(Rationnel **ligne, int numero_variable, Rationnel **valeur_variable, int n)
{
    Rationnel* rat = ligne[numero_variable];
    if(!rat)
        return ligne;
    ligne[numero_variable] = NULL;
    for(int i = 0; i <= n; i++)
        if (valeur_variable[i])
            ligne[i] = ligne[i] ? Union(ligne[i], Concat(rat, valeur_variable[i])) : Concat(rat, valeur_variable[i]);
    return ligne;
}

Systeme resoudre_systeme(Systeme systeme, int n)
{
    for(int i = 0; i < n; i++)
    {
        resoudre_variable_arden(systeme[i], i, n);
        for(int j = 0; j < n; j++)
            if(j != i)
                substituer_variable(systeme[j], i, systeme[i], n);
    }
    return systeme;
}

Rationnel *Arden(Automate *automate)
{
   int nbEtat = taille_ensemble(get_etats(automate));
   Systeme s = systeme(automate);
   s = resoudre_systeme(s, nbEtat);
   return NULL;
}
