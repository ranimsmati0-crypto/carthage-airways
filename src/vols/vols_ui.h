
#ifndef VOLS_UI_H
#define VOLS_UI_H

#include <gtk/gtk.h>

void afficher_vols(GtkTreeView *tv, const char *filtre);
void afficher_vols_criteres(GtkTreeView *tv, const char *depart,
                            const char *destination, const char *date,
                            const char *date_arrivee, const char *heure);

#endif
