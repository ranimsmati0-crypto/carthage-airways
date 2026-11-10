#include <gtk/gtk.h>
#include "design.h"

/* Fenetre de demonstration du design commun.
   Usage : ./demo_design [page_active]   (ex : ./demo_design avions) */
int main (int argc, char *argv[])
{
    GtkWidget *fenetre, *hbox, *menu, *contenu;

    gtk_init (&argc, &argv);
    design_init ();

    fenetre = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (fenetre), "Demo du design commun");
    gtk_window_set_default_size (GTK_WINDOW (fenetre), 900, 560);
    g_signal_connect (fenetre, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    hbox = gtk_hbox_new (FALSE, 0);
    menu = design_menu_creer (argc > 1 ? argv[1] : "vols");
    contenu = gtk_label_new ("Ici : le contenu de votre module");

    gtk_box_pack_start (GTK_BOX (hbox), menu, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (hbox), contenu, TRUE, TRUE, 0);
    gtk_container_add (GTK_CONTAINER (fenetre), hbox);
    gtk_widget_show_all (fenetre);

    gtk_main ();
    return 0;
}
