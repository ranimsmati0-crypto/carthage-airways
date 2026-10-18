#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "vols.h"

#define FICHIER  "data/vols.txt"
#define TEMP     "data/vols.tmp"
#define LECTURE  "%d;%29[^;];%29[^;];%10[^;];%5[^;];%10[^;];%5[^;];%19[^;];%14[^\n]\n"
#define ECRITURE "%d;%s;%s;%s;%s;%s;%s;%s;%s\n"
#define NB_CHAMPS 9

#define LIRE_ARGS(v) &(v).id, (v).depart, (v).destination, (v).date, \
                     (v).h_depart, (v).date_arrivee, (v).h_arrivee, \
                     (v).avion, (v).statut
#define ECRIRE_ARGS(v) (v).id, (v).depart, (v).destination, (v).date, \
                       (v).h_depart, (v).date_arrivee, (v).h_arrivee, \
                       (v).avion, (v).statut

int vol_existe(int id)
{
    FILE *f = fopen(FICHIER, "r");
    Vol v;
    int trouve = 0;

    if (f == NULL)
        return 0;
    while (fscanf(f, LECTURE, LIRE_ARGS(v)) == NB_CHAMPS) {
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
    fprintf(f, ECRITURE, ECRIRE_ARGS(v));
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
    while (fscanf(f, LECTURE, LIRE_ARGS(v)) == NB_CHAMPS) {
        if (v.id == id) {
            v = nouveau;
            trouve = 1;
        }
        fprintf(t, ECRITURE, ECRIRE_ARGS(v));
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
    while (fscanf(f, LECTURE, LIRE_ARGS(v)) == NB_CHAMPS) {
        if (v.id == id)
            trouve = 1;
        else
            fprintf(t, ECRITURE, ECRIRE_ARGS(v));
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
    while (!trouve && fscanf(f, LECTURE, LIRE_ARGS(v)) == NB_CHAMPS) {
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

int charger_vols(Vol tab[], int max, const char *filtre)
{
    FILE *f = fopen(FICHIER, "r");
    Vol v;
    int n = 0;

    if (f == NULL)
        return 0;
    while (n < max && fscanf(f, LECTURE, LIRE_ARGS(v)) == NB_CHAMPS) {
        if (filtre == NULL || filtre[0] == '\0'
            || strstr(v.depart, filtre)
            || strstr(v.destination, filtre)
            || strstr(v.date, filtre)
            || strstr(v.date_arrivee, filtre))
            tab[n++] = v;
    }
    fclose(f);
    return n;
}

static int contient(const char *texte, const char *critere)
{
    char t[64], c[64];
    int i;

    if (critere == NULL || critere[0] == '\0')
        return 1;
    for (i = 0; i < 63 && texte[i]; i++)
        t[i] = tolower((unsigned char)texte[i]);
    t[i] = '\0';
    for (i = 0; i < 63 && critere[i]; i++)
        c[i] = tolower((unsigned char)critere[i]);
    c[i] = '\0';
    return strstr(t, c) != NULL;
}

int charger_vols_criteres(Vol tab[], int max, const char *depart,
                          const char *destination, const char *date,
                          const char *date_arrivee, const char *heure)
{
    FILE *f = fopen(FICHIER, "r");
    Vol v;
    int n = 0;

    if (f == NULL)
        return 0;
    while (n < max && fscanf(f, LECTURE, LIRE_ARGS(v)) == NB_CHAMPS) {
        if (contient(v.depart, depart)
            && contient(v.destination, destination)
            && contient(v.date, date)
            && contient(v.date_arrivee, date_arrivee)
            && contient(v.h_depart, heure))
            tab[n++] = v;
    }
    fclose(f);
    return n;
}
