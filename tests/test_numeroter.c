#include <automate.h>
#include <rationnel.h>
#include <ensemble.h>
#include <outils.h>
#include <parse.h>
#include <scan.h>

void print(const intptr_t c)
{
    printf("%c", (char)c);
}

int main()
{
    Rationnel* rat;
    Rationnel* rat2;
    rat = expression_to_rationnel("c+d.(a.b*)");
    print_ensemble(dernier(rat), &print);
    printf("\n");
    rationnel_to_dot(rat, "rat.dot");

    rat2 = expression_to_rationnel("c.d*");
    if(contient_mot_vide(rat2))
        printf("effaçable \n");
    else
        printf("non effaçable \n");
}
