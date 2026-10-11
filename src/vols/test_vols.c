#include <stdio.h>
#include <string.h>
#include "vols.h"

int main(void)
{
    Vol v = {101, "Tunis", "Paris", "15/11/2026", "08:30", "11:00", "TS-ABC", "Prevu"};
    Vol r;
    char duree[10];

    printf("Ajout 101 : %d\n", ajouter_vol(v));
    printf("Ajout 101 en double : %d\n", ajouter_vol(v));
    if (chercher_vol(101, &r)) {
        calculer_duree(r.h_depart, r.h_arrivee, duree);
        printf("Trouve : %s -> %s, duree %s\n", r.depart, r.destination, duree);
    }
    strcpy(v.destination, "Lyon");
    printf("Modification : %d\n", modifier_vol(101, v));
    printf("Suppression : %d\n", supprimer_vol(101));
    return 0;
}
