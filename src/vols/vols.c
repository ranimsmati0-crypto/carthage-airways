#include <stdio.h>
#include <string.h>
#include "vols.h"

#define FICHIER "data/vols.txt"
#define TEMP    "data/vols.tmp"
#define LECTURE "%d;%29[^;];%29[^;];%10[^;];%5[^;];%5[^;];%19[^;];%14[^\n]\n"
#define ECRITURE "%d;%s;%s;%s;%s;%s;%s;%s\n"

int vol_existe(int id)
{
    FILE *f = fopen(FICHIER, "r");
    Vol v;
    int trouve = 0;

    if (f == NULL)
        return 0;
    while (fscanf(f, LECTURE, &v.id, v.depart, v.destination, v.date,
                  v.h_depart, v.h_arrivee, v.avion, v.statut) == 8)
    {
        if (v.id == id)
            trouve = 1;
    }
    fclose(f);
    return trouve;
}

int ajouter_vol(Vol v)
{
    FILE *f;

    if (vol_existe(v.id))
        return 0;
    f = fopen(FICHIER, "a");
    if (f == NULL)
        return 0;
    fprintf(f, ECRITURE, v.id, v.depart, v.destination, v.date,
            v.h_depart, v.h_arrivee, v.avion, v.statut);
    fclose(f);
    return 1;
}

int modifier_vol(int id, Vol nouveau)
{
    FILE *f = fopen(FICHIER, "r");
    FILE *t = fopen(TEMP, "w");
    Vol v;
    int trouve = 0;

    if (f == NULL || t == NULL) {
        if (f) fclose(f);
        if (t) fclose(t);
        return 0;
    }
    nouveau.id = id;
    while (fscanf(f, LECTURE, &v.id, v.depart, v.destination, v.date,
                  v.h_depart, v.h_arrivee, v.avion, v.statut) == 8)
    {
        if (v.id == id) {
            v = nouveau;
            trouve = 1;
        }
        fprintf(t, ECRITURE, v.id, v.depart, v.destination, v.date,
                v.h_depart, v.h_arrivee, v.avion, v.statut);
    }
    fclose(f);
    fclose(t);
    remove(FICHIER);
    rename(TEMP, FICHIER);
    return trouve;
}

int supprimer_vol(int id)
{
    FILE *f = fopen(FICHIER, "r");
    FILE *t = fopen(TEMP, "w");
    Vol v;
    int trouve = 0;

    if (f == NULL || t == NULL) {
        if (f) fclose(f);
        if (t) fclose(t);
        return 0;
    }
    while (fscanf(f, LECTURE, &v.id, v.depart, v.destination, v.date,
                  v.h_depart, v.h_arrivee, v.avion, v.statut) == 8)
    {
        if (v.id == id)
            trouve = 1;
        else
            fprintf(t, ECRITURE, v.id, v.depart, v.destination, v.date,
                    v.h_depart, v.h_arrivee, v.avion, v.statut);
    }
    fclose(f);
    fclose(t);
    remove(FICHIER);
    rename(TEMP, FICHIER);
    return trouve;
}

int chercher_vol(int id, Vol *resultat)
{
    FILE *f = fopen(FICHIER, "r");
    Vol v;
    int trouve = 0;

    if (f == NULL)
        return 0;
    while (!trouve &&
           fscanf(f, LECTURE, &v.id, v.depart, v.destination, v.date,
                  v.h_depart, v.h_arrivee, v.avion, v.statut) == 8)
    {
        if (v.id == id) {
            *resultat = v;
            trouve = 1;
        }
    }
    fclose(f);
    return trouve;
}

void calculer_duree(const char *h_dep, const char *h_arr, char *duree)
{
    int h1, m1, h2, m2, minutes;

    sscanf(h_dep, "%d:%d", &h1, &m1);
    sscanf(h_arr, "%d:%d", &h2, &m2);
    minutes = (h2 * 60 + m2) - (h1 * 60 + m1);
    if (minutes < 0)
        minutes += 24 * 60;
    sprintf(duree, "%02dh%02d", minutes / 60, minutes % 60);
}
