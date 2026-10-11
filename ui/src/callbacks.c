#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "vols_ui.h"
#include "vols_ui.h"


void
on_btn_rechercher_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *fenetre = gtk_widget_get_toplevel(GTK_WIDGET(button));
  GtkWidget *entree  = lookup_widget(fenetre, "entry_recherche");
  GtkWidget *tableau = lookup_widget(fenetre, "treeview_vols");

  afficher_vols(GTK_TREE_VIEW(tableau),
                gtk_entry_get_text(GTK_ENTRY(entree)));


}


void
on_btn_ajouter_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_btn_modifier_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_btn_supprimer_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_btn_affecter_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{

}

