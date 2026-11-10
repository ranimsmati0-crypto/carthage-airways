#ifndef DESIGN_H
#define DESIGN_H

#include <gtk/gtk.h>

/* Charge le theme commun. A appeler une fois, juste apres gtk_init. */
void design_init (void);

/* Cree le menu llateral commun.
   "actif" = page courante : "tableau", "vols", "reservations", "passagers",
   "personnel", "avions", "aeroports" ou "parametres". */
GtkWidget *design_menu_creer (const char *actif);

/* Branche une action sur un bouton du menu (meme identifiants que ci-dessus). */
void design_menu_action (GtkWidget *menu, const char *id,
                         GCallback fonction, gpointer donnee);

#endif
