#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "vols_ui.h"
static int date_existe(int j, int m, int a)
{
    int jours[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int bissextile = (a % 4 == 0 && a % 100 != 0) || (a % 400 == 0);
    if (bissextile) jours[1] = 29;
    return (m >= 1 && m <= 12 && j >= 1 && j <= jours[m - 1]);
}

static void afficher_message(GtkWidget *parent, const char *texte)
{
    GtkWidget *dlg = gtk_message_dialog_new(
        GTK_WINDOW(gtk_widget_get_toplevel(parent)), GTK_DIALOG_MODAL,
        GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "%s", texte);
    gtk_dialog_run(GTK_DIALOG(dlg));
    gtk_widget_destroy(dlg);
}

static int lire_date(GtkWidget *w, const char *id_check, const char *id_j,
                     const char *id_m, const char *id_a,
                     const char *nom_champ, char *resultat)
{
    resultat[0] = '\0';
    if (!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(w, id_check))))
        return 1;

    GtkSpinButton *sj = GTK_SPIN_BUTTON(lookup_widget(w, id_j));
    GtkSpinButton *sm = GTK_SPIN_BUTTON(lookup_widget(w, id_m));
    GtkSpinButton *sa = GTK_SPIN_BUTTON(lookup_widget(w, id_a));
    gtk_spin_button_update(sj);
    gtk_spin_button_update(sm);
    gtk_spin_button_update(sa);

    int j = gtk_spin_button_get_value_as_int(sj);
    int m = gtk_spin_button_get_value_as_int(sm);
    int a = gtk_spin_button_get_value_as_int(sa);

    if (!date_existe(j, m, a)) {
        char msg[120];
        snprintf(msg, sizeof msg, "%s invalide : %02d/%02d/%04d n'existe pas.",
                 nom_champ, j, m, a);
        afficher_message(w, msg);
        return 0;
    }
    snprintf(resultat, 11, "%02d/%02d/%04d", j, m, a);
    return 1;
}

static int date_en_entier(const char *s)
{
    int j, m, a;
    sscanf(s, "%d/%d/%d", &j, &m, &a);
    return a * 10000 + m * 100 + j;
}

void on_btn_rechercher_clicked(GtkButton *button, gpointer user_data)
{
        GtkWidget *w = gtk_widget_get_toplevel(GTK_WIDGET(button));
    const char *dep  = gtk_entry_get_text(GTK_ENTRY(lookup_widget(w, "entry_depart")));
    const char *dest = gtk_entry_get_text(GTK_ENTRY(lookup_widget(w, "entry_destination")));
    char d_dep[11], d_arr[11];

    if (!lire_date(w, "check_date_depart", "spin_jour_depart", "spin_mois_depart",
                   "spin_annee_depart", "Date de départ", d_dep))
        return;
    if (!lire_date(w, "check_date_arrivee", "spin_jour_arrivee", "spin_mois_arrivee",
                   "spin_annee_arrivee", "Date d'arrivée", d_arr))
        return;

    if (d_dep[0] && d_arr[0] && date_en_entier(d_arr) < date_en_entier(d_dep)) {
        afficher_message(w, "La date d'arrivée ne peut pas être avant la date de départ.");
        return;
    }

    afficher_vols_criteres(GTK_TREE_VIEW(lookup_widget(w, "treeview_vols")),
                           dep, dest, d_dep, d_arr, "");
}
void on_check_date_depart_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    GtkWidget *w = gtk_widget_get_toplevel(GTK_WIDGET(togglebutton));
    gtk_widget_set_sensitive(lookup_widget(w, "box_date_depart"),
                             gtk_toggle_button_get_active(togglebutton));
}

void on_check_date_arrivee_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    GtkWidget *w = gtk_widget_get_toplevel(GTK_WIDGET(togglebutton));
    gtk_widget_set_sensitive(lookup_widget(w, "box_date_arrivee"),
                             gtk_toggle_button_get_active(togglebutton));
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


void
on_check_escale_aj_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_btn_enregistrer_aj_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_btn_annuler_aj_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{

}

