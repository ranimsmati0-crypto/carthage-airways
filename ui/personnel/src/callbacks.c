#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"


void
on_btn_test_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  g_print("Le bouton marche !\n");
}

void
on_window_test_destroy                 (GtkObject       *object,
                                        gpointer         user_data)
{
  gtk_main_quit();
}

void
on_window_admin_personnel_destroy      (GtkObject       *object,
                                        gpointer         user_data)
{

}


void
on_admin_menu_licences_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_admin_menu_stats_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_admin_menu_deconnexion_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_admin_btn_rechercher_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_admin_btn_reinitialiser_clicked     (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_admin_btn_ajouter_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_admin_btn_modifier_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_admin_btn_supprimer_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{

}

