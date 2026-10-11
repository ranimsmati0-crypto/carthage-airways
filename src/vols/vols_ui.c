#include <stdio.h>
#include <gtk/gtk.h>
#include "vols.h"
#include "vols_ui.h"

#define MAX_VOLS 200

enum { COL_ID, COL_DEPART, COL_DEST, COL_DATE, COL_HDEP,
       COL_HARR, COL_DUREE, COL_AVION, COL_STATUT, NB_COL };

static void creer_colonnes(GtkTreeView *tv)
{
    const char *titres[NB_COL] = {"ID", "Depart", "Destination", "Date",
        "Heure depart", "Heure arrivee", "Duree", "Avion", "Statut"};
    int i;

    for (i = 0; i < NB_COL; i++) {
        GtkCellRenderer *r = gtk_cell_renderer_text_new();
        GtkTreeViewColumn *c = gtk_tree_view_column_new_with_attributes(
            titres[i], r, "text", i, NULL);
        gtk_tree_view_append_column(tv, c);
    }
}

void afficher_vols(GtkTreeView *tv, const char *filtre)
{
    Vol tab[MAX_VOLS];
    GtkListStore *store;
    GtkTreeIter iter;
    char id[12], duree[10];
    int i, n;

    if (gtk_tree_view_get_column(tv, 0) == NULL)
        creer_colonnes(tv);

    store = gtk_list_store_new(NB_COL,
        G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
        G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
        G_TYPE_STRING);

    n = charger_vols(tab, MAX_VOLS, filtre);
    for (i = 0; i < n; i++) {
        sprintf(id, "%d", tab[i].id);
        calculer_duree(tab[i].h_depart, tab[i].h_arrivee, duree);
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
            COL_ID, id,
            COL_DEPART, tab[i].depart,
            COL_DEST, tab[i].destination,
            COL_DATE, tab[i].date,
            COL_HDEP, tab[i].h_depart,
            COL_HARR, tab[i].h_arrivee,
            COL_DUREE, duree,
            COL_AVION, tab[i].avion,
            COL_STATUT, tab[i].statut,
            -1);
    }
    gtk_tree_view_set_model(tv, GTK_TREE_MODEL(store));
    g_object_unref(store);
}
