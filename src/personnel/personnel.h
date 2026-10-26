#ifndef PERSONNEL_H
#define PERSONNEL_H

#include <gtk/gtk.h>

#define MAX_PERSONNEL 200
#define FICHIER_PERSONNEL PACKAGE_DATA_DIR "/../../data/personnel.txt"

typedef struct {
    int  id;
    char nom[30];
    char prenom[30];
    int  jour, mois, annee;
    char telephone[20];
    char email[50];
    char login[30];
    char fonction[30];
    char base[30];
    int  disponible;
    int  heures_vol;
} Personnel;

int  lire_personnel(Personnel tab[], int max);
void afficher_personnel(GtkWidget *treeview);

#endif
