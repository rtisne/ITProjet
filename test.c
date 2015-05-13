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

int main()
{
    Rationnel * rat;
	char * expr = "((a.b.d)*.f+a.b.c+b)*";
    rat = expression_to_rationnel(expr);
	printf("%s\n", expr);
    numeroter_rationnel(rat);
    rationnel_to_dot(rat,"test.dot");
    printf("%d %d\n", contient_mot_vide(rat), true);

    Ensemble *ep = premier(rat);
    Ensemble *ed = dernier(rat);
    Ensemble *es = suivant(rat, 3);
	Automate *a = Glushkov(rat);

    Automate *aMir = miroir(a);
    Automate *aMin = creer_automate_minimal(a);

    Rationnel *rattest = expression_to_rationnel("(b+a.b*.a)*");
    Automate *atest = Glushkov(rattest);
    Automate *atestMin = creer_automate_minimal(atest);

    Systeme sys1 = systeme(a);
    Systeme sys2 = systeme(aMir);
    Systeme sys3 = systeme(aMin);
    Systeme sys4 = systeme(atestMin);
    Systeme sys5 = systeme(atestMin);
    sys5 = resoudre_systeme(sys5, taille_ensemble(get_etats(atestMin)));

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
    printf("\nautomate\n");
    print_automate(a);
    printf("\nautomate mirroir\n");
    print_automate(aMir);
    printf("\nautomate minimal\n");
    print_automate(aMin);
    printf("\nsysteme automate\n");
    print_systeme(sys1, taille_ensemble(get_etats(a)));
    printf("\nsysteme automate mirroir\n");
    print_systeme(sys2, taille_ensemble(get_etats(aMir)));
    printf("\nsysteme automate minimal\n");
    print_systeme(sys3, taille_ensemble(get_etats(aMin)));
    printf("\nsysteme automate autre\n");
    print_automate(atestMin);
    printf("\n");
    print_systeme(sys4, taille_ensemble(get_etats(atestMin)));
    printf("\nsysteme automate autre resolu\n");
    print_systeme(sys5, taille_ensemble(get_etats(atestMin)));
    print_rationnel(Arden(atestMin));
    printf("\n");

    printf("\nmeme langage\n");
    printf("(a.b)*.a et a.(b.a)*\t");
    if(meme_langage("(a.b)*.a", "a.(b.a)*"))
        printf("meme langage pass\n");
    else
        printf("meme langage fail\n");

    printf("(a*.b*)* et (a+b)*\t");
    if(meme_langage("(a*.b*)*", "(a+b)*"))
        printf("meme langage pass\n");
    else
        printf("meme langage fail\n");

    printf("(a*.b*)* et (a+b*)*\t");
    if(meme_langage("(a*.b*)*", "(a+b*)*"))
        printf("meme langage pass\n");
    else
        printf("meme langage fail\n");

    printf("(a*.b*)* et (a*+b*)*\t");
    if(meme_langage("(a*.b*)*", "(a*+b*)*"))
        printf("meme langage pass\n");
    else
        printf("meme langage fail\n");

    printf("(a*.b*)* et (a*+b*)\t");
    if(meme_langage("(a*.b*)*", "(a*+b*)"))
        printf("meme langage pass\n");
    else
        printf("meme langage fail\n");

    printf("(b+a.b*.a)* et b*+b*.a.(b+a.b*.a)*.a.b*\t");
    if(meme_langage("(b+a.b*.a)*", "b*+b*.a.(b+a.b*.a)*.a.b*"))
        printf("meme langage pass\n");
    else
        printf("meme langage fail\n");

    printf("(a.(a.b)*.a.a)*.a.(a.b)* et a+a.a.((b+a.a).a)*.(b+a.a)\t");
    if(meme_langage("(a.(a.b)*.a.a)*.a.(a.b)*", "a+a.a.((b+a.a).a)*.(b+a.a)"))
        printf("meme langage pass\n");
    else
        printf("meme langage fail\n");

    printf("\n");
    print_automate(creer_automate_minimal(Glushkov(expression_to_rationnel("(b+a.b*.a)*"))));
    printf("\n");
    print_automate(creer_automate_minimal(Glushkov(Arden(creer_automate_minimal(Glushkov(expression_to_rationnel("(b+a.b*.a)*")))))));
    printf("\n");

    int taille = taille_ensemble(get_etats(creer_automate_minimal(Glushkov(expression_to_rationnel("(a.(a.b)*.a.a)*.a.(a.b)*")))));
    int taille2 = taille_ensemble(get_etats(creer_automate_minimal(Glushkov(Arden(creer_automate_minimal(Glushkov(expression_to_rationnel("(a.(a.b)*.a.a)*.a.(a.b)*"))))))));

    printf("\n");
    print_automate(creer_automate_minimal(Glushkov(expression_to_rationnel("(a.(a.b)*.a.a)*.a.(a.b)*"))));
    printf("\n");
    print_automate(creer_automate_minimal(Glushkov(Arden(creer_automate_minimal(Glushkov(expression_to_rationnel("(a.(a.b)*.a.a)*.a.(a.b)*")))))));
    printf("\n");
    printf("\n");
    print_systeme(systeme(creer_automate_minimal(Glushkov(expression_to_rationnel("(a.(a.b)*.a.a)*.a.(a.b)*")))), taille);
    printf("\n");
    print_systeme(systeme(creer_automate_minimal(Glushkov(Arden(creer_automate_minimal(Glushkov(expression_to_rationnel("(a.(a.b)*.a.a)*.a.(a.b)*"))))))), taille2);
    printf("\n");
    printf("\n");
    print_systeme(resoudre_systeme(systeme(creer_automate_minimal(Glushkov(expression_to_rationnel("(a.(a.b)*.a.a)*.a.(a.b)*")))), taille), taille);
    printf("\n");
    print_systeme(resoudre_systeme(systeme(creer_automate_minimal(Glushkov(Arden(creer_automate_minimal(Glushkov(expression_to_rationnel("(a.(a.b)*.a.a)*.a.(a.b)*"))))))), taille2), taille2);
    printf("\n");

    return 0;
}
