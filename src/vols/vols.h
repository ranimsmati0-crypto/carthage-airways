#ifndef VOLS_H
#define VOLS_H

typedef struct {
    int  id;
    char depart[30];
    char destination[30];
    char date[11];
    char h_depart[6];
    char h_arrivee[6];
    char avion[20];
    char statut[15];
} Vol;

int  vol_existe(int id);
int  ajouter_vol(Vol v);
int  modifier_vol(int id, Vol nouveau);
int  supprimer_vol(int id);
int  chercher_vol(int id, Vol *resultat);
void calculer_duree(const char *h_dep, const char *h_arr, char *duree);

int  charger_vols(Vol tab[], int max, const char *filtre);

int  charger_vols_criteres(Vol tab[], int max, const char *depart,
                          const char *destination, const char *date,
                          const char *heure);

#endif
