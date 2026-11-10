#include <string.h>
#include <gtk/gtk.h>
#include "design.h"

static const struct {
    const char *id;
    const char *texte;
    const char *icone;
} ENTREES[] = {
    { "tableau",      "Tableau de bord", "pixmaps/tableau.png"      },
    { "vols",         "Vols",            "pixmaps/vols.png"         },
    { "reservations", "Reservations",    "pixmaps/reservations.png" },
    { "passagers",    "Passagers",       "pixmaps/passagers.png"    },
    { "personnel",    "Personnel",       "pixmaps/personnel.png"    },
    { "avions",       "Avions",          "pixmaps/avions.png"       },
    { "aeroports",    "Aeroports",       "pixmaps/aeroports.png"    },
    { "parametres",   "Parametres",      "pixmaps/parametres.png"   }
};
#define NB_ENTREES (sizeof ENTREES / sizeof ENTREES[0])

void design_init (void)
{
    gtk_rc_parse ("data/theme.rc");
}

GtkWidget *design_menu_creer (const char *actif)
{
    GtkWidget *menu, *logo;
    guint i;

    menu = gtk_vbox_new (FALSE, 4);
    gtk_container_set_border_width (GTK_CONTAINER (menu), 8);
    gtk_widget_set_size_request (menu, 190, -1);

    logo = gtk_label_new (NULL);
    gtk_label_set_markup (GTK_LABEL (logo),
        "<span size=\"large\" weight=\"bold\">Carthage Airways</span>");
    gtk_misc_set_alignment (GTK_MISC (logo), 0.0, 0.5);
    gtk_misc_set_padding (GTK_MISC (logo), 4, 12);
    gtk_box_pack_start (GTK_BOX (menu), logo, FALSE, FALSE, 8);

    for (i = 0; i < NB_ENTREES; i++) {
        GtkWidget *bouton = gtk_button_new ();
        GtkWidget *cadre  = gtk_alignment_new (0.0, 0.5, 0.0, 0.0);
        GtkWidget *ligne  = gtk_hbox_new (FALSE, 8);
        GtkWidget *image  = gtk_image_new_from_file (ENTREES[i].icone);
        GtkWidget *texte  = gtk_label_new (ENTREES[i].texte);

        gtk_box_pack_start (GTK_BOX (ligne), image, FALSE, FALSE, 0);
        gtk_box_pack_start (GTK_BOX (ligne), texte, FALSE, FALSE, 0);
        gtk_container_add (GTK_CONTAINER (cadre), ligne);
        gtk_container_add (GTK_CONTAINER (bouton), cadre);

        gtk_button_set_relief (GTK_BUTTON (bouton), GTK_RELIEF_NONE);
        gtk_widget_set_can_focus (bouton, FALSE);

        if (actif != NULL && strcmp (actif, ENTREES[i].id) == 0)
            gtk_widget_set_name (bouton, "carthage_menu_actif");
        else
            gtk_widget_set_name (bouton, "carthage_menu");

        g_object_set_data (G_OBJECT (menu), ENTREES[i].id, bouton);
        gtk_box_pack_start (GTK_BOX (menu), bouton, FALSE, FALSE, 0);
    }

    gtk_widget_show_all (menu);
    return menu;
}

void design_menu_action (GtkWidget *menu, const char *id,
                         GCallback fonction, gpointer donnee)
{
    GtkWidget *bouton = g_object_get_data (G_OBJECT (menu), id);

    if (bouton != NULL)
        g_signal_connect (bouton, "clicked", fonction, donnee);
}
