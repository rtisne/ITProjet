// compilation line
// gcc -g -Wall -std=c11 test.c libautomate.a -o test -lm

#include "automate.h"
#include "rationnel.h"
#include "ensemble.h"
#include "outils.h"
#include "parse.h"
#include "scan.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    Rationnel * rat;
	char * expr = "((a.b.d)*.f+a.b.c+b)*";
    rat = expression_to_rationnel(expr);
	printf("%s", expr);
    numeroter_rationnel(rat);
    rationnel_to_dot(rat,"test.dot");
    printf("%d %d\n", contient_mot_vide(rat), true);

    Ensemble *ep = premier(rat);
    Ensemble *ed = dernier(rat);
    Ensemble *es = suivant(rat, 3);
	char d = lettre_position(rat, 3);
	Automate *a = Glushkov(rat);

    Automate *aMir = miroir(a);
    Automate *aMin = creer_automate_minimal(a);


    if(est_dans_l_ensemble(ep, 1)
        && ! est_dans_l_ensemble(ep, 2)
        && ! est_dans_l_ensemble(ep, 3)
        && est_dans_l_ensemble(ep, 4)
        && est_dans_l_ensemble(ep, 5)
        && ! est_dans_l_ensemble(ep, 6)
        && ! est_dans_l_ensemble(ep, 7)
        && est_dans_l_ensemble(ep, 8))
        printf("premier pass\n");
    else
        printf("premier fail\n");

    if(! est_dans_l_ensemble(ed, 1)
        && ! est_dans_l_ensemble(ed, 2)
        && ! est_dans_l_ensemble(ed, 3)
        && est_dans_l_ensemble(ed, 4)
        && ! est_dans_l_ensemble(ed, 5)
        && ! est_dans_l_ensemble(ed, 6)
        && est_dans_l_ensemble(ed, 7)
        && est_dans_l_ensemble(ed, 8))
        printf("dernier pass\n");
    else
        printf("dernier fail\n");

	if(est_dans_l_ensemble(es, 1)
        && ! est_dans_l_ensemble(es, 2)
        && ! est_dans_l_ensemble(es, 3)
        && est_dans_l_ensemble(es, 4)
        && ! est_dans_l_ensemble(es, 5)
        && ! est_dans_l_ensemble(es, 6)
        && ! est_dans_l_ensemble(es, 7)
        && ! est_dans_l_ensemble(es, 8))
        printf("suivant pass\n");
    else
        printf("suivant fail\n");

    printf("premier\n");
    print_ensemble(ep, NULL);
    printf("\ndernier\n");
    print_ensemble(ed, NULL);
    printf("\nsuivant\n");
	print_ensemble(es, NULL);
    printf("\nlettre position\n %c\n", d);
    printf("automate\n");
    print_automate(a);
    printf("\nautomate mirroir\n");
    print_automate(aMir);
    printf("\nautomate minimal\n");
    print_automate(aMin);

    return 0;
}
