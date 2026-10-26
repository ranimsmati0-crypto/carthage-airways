#include <stdio.h>
#include <string.h>
#include "personnel.h"

enum { COL_ID, COL_NOM, COL_PRENOM, COL_FONCTION, COL_BASE,
       COL_DISPO, COL_HEURES, NB_COL };

int lire_personnel(Personnel tab[], int max)
{
    FILE *f = fopen(FICHIER_PERSONNEL, "r");
    int n = 0;

    if (f == NULL) {
        g_print("Impossible d'ouvrir %s\n", FICHIER_PERSONNEL);
        return 0;
    }
    while (n < max &&
           fscanf(f, "%d;%29[^;];%29[^;];%d;%d;%d;%19[^;];%49[^;];%29[^;];%29[^;];%29[^;];%d;%d",
                  &tab[n].id, tab[n].nom, tab[n].prenom,
                  &tab[n].jour, &tab[n].mois, &tab[n].annee,
                  tab[n].telephone, tab[n].email, tab[n].login,
                  tab[n].fonction, tab[n].base,
                  &tab[n].disponible, &tab[n].heures_vol) == 13)
        n++;
    fclose(f);
    return n;
}

void afficher_personnel(GtkWidget *treeview)
{
    static Personnel tab[MAX_PERSONNEL];
    GtkListStore *store;
    GtkTreeIter iter;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *col;
    const char *titres[NB_COL] = { "ID", "Nom", "Prénom", "Fonction",
                                   "Base d'attache", "Disponibilité",
                                   "Heures de vol" };
    int i, n;

    n = lire_personnel(tab, MAX_PERSONNEL);

    /* les colonnes ne sont créées qu'une seule fois */
    if (gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)) == NULL) {
        for (i = 0; i < NB_COL; i++) {
            renderer = gtk_cell_renderer_text_new();
            col = gtk_tree_view_column_new_with_attributes(titres[i], renderer,
                                                           "text", i, NULL);
            gtk_tree_view_column_set_sort_column_id(col, i);
            gtk_tree_view_column_set_resizable(col, TRUE);
            gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);
        }
    }

    store = gtk_list_store_new(NB_COL, G_TYPE_INT, G_TYPE_STRING,
                               G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
                               G_TYPE_STRING, G_TYPE_INT);

    for (i = 0; i < n; i++) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           COL_ID, tab[i].id,
                           COL_NOM, tab[i].nom,
                           COL_PRENOM, tab[i].prenom,
                           COL_FONCTION, tab[i].fonction,
                           COL_BASE, tab[i].base,
                           COL_DISPO, tab[i].disponible ? "Disponible" : "Indisponible",
                           COL_HEURES, tab[i].heures_vol,
                           -1);
    }
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    g_object_unref(store);
}
