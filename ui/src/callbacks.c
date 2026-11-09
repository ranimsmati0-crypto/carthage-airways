#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>


#include "vols.h"
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

#define CREER_FORMULAIRE create_fenetre_ajoute_vol
/* Recalcule et affiche la duree a partir des heures du formulaire */
static void maj_duree (GtkWidget *form)
{
    char h_dep[6], h_arr[6], duree[10];

    snprintf (h_dep, sizeof h_dep, "%02d:%02d",
              gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (lookup_widget (form, "spin_h_dep_aj"))),
              gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (lookup_widget (form, "spin_min_dep_aj"))));
    snprintf (h_arr, sizeof h_arr, "%02d:%02d",
              gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (lookup_widget (form, "spin_h_arr_aj"))),
              gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (lookup_widget (form, "spin_min_arr_aj"))));
    calculer_duree (h_dep, h_arr, duree);
    gtk_entry_set_text (GTK_ENTRY (lookup_widget (form, "entry_duree_aj")), duree);
}

static void on_spin_heure_changed (GtkSpinButton *sb, gpointer form)
{
    maj_duree (GTK_WIDGET (form));
}

/* Champ Duree en affichage seul, mis a jour quand on change une heure */
static void preparer_formulaire (GtkWidget *form)
{
    static const char *spins[] = { "spin_h_dep_aj", "spin_min_dep_aj",
                                   "spin_h_arr_aj", "spin_min_arr_aj" };
    int i;

    gtk_editable_set_editable (GTK_EDITABLE (lookup_widget (form, "entry_duree_aj")), FALSE);
    gtk_label_set_text (GTK_LABEL (lookup_widget (form, "label_duree_aj")), "Duree (auto)");
    for (i = 0; i < 4; i++)
        g_signal_connect (lookup_widget (form, spins[i]), "value-changed",
                          G_CALLBACK (on_spin_heure_changed), form);
    maj_duree (form);
}

void
on_btn_ajouter_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *principale = gtk_widget_get_toplevel (GTK_WIDGET (button));
  GtkWidget *formulaire = CREER_FORMULAIRE ();
    preparer_formulaire (formulaire);

  g_object_set_data (G_OBJECT (formulaire), "mode", "ajout");
  g_object_set_data (G_OBJECT (formulaire), "principale", principale);
  gtk_window_set_transient_for (GTK_WINDOW (formulaire), GTK_WINDOW (principale));
  gtk_widget_show (formulaire);
}

void
on_check_escale_aj_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GtkWidget *champ = lookup_widget (GTK_WIDGET (togglebutton), "entry_escale_aj");

  gtk_widget_set_sensitive (champ, gtk_toggle_button_get_active (togglebutton));
}

void
on_btn_annuler_aj_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}


static void message_vol (GtkWidget *parent, GtkMessageType type, const char *texte);

static void spin_set (GtkWidget *form, const char *nom, int valeur)
{
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (lookup_widget (form, nom)), valeur);
}

static void texte_set (GtkWidget *form, const char *nom, const char *texte)
{
    gtk_entry_set_text (GTK_ENTRY (lookup_widget (form, nom)), texte);
}

/* Remplit le formulaire avec les donnees d'un vol existant */
static void remplir_formulaire (GtkWidget *form, const Vol *v)
{
    char tmp[16];
    int j, m, a, h, mn;

    snprintf (tmp, sizeof tmp, "%d", v->id);
    texte_set (form, "entry_id_aj", tmp);
    gtk_editable_set_editable (GTK_EDITABLE (lookup_widget (form, "entry_id_aj")), FALSE);

    texte_set (form, "entry_depart_aj", v->depart);
    texte_set (form, "entry_destination_aj", v->destination);
    texte_set (form, "entry_compagnie_aj", v->compagnie);
    texte_set (form, "entry_aeroport_aj", v->aeroport);
    texte_set (form, "entry_duree_aj", v->duree);

    if (sscanf (v->date, "%d/%d/%d", &j, &m, &a) == 3) {
        spin_set (form, "spin_jour_dep_aj", j);
        spin_set (form, "spin_mois_dep_aj", m);
        spin_set (form, "spin_annee_dep_aj", a);
    }
    if (sscanf (v->h_depart, "%d:%d", &h, &mn) == 2) {
        spin_set (form, "spin_h_dep_aj", h);
        spin_set (form, "spin_min_dep_aj", mn);
    }
    if (sscanf (v->date_arrivee, "%d/%d/%d", &j, &m, &a) == 3) {
        spin_set (form, "spin_jour_arr_aj", j);
        spin_set (form, "spin_mois_arr_aj", m);
        spin_set (form, "spin_annee_arr_aj", a);
    }
    if (sscanf (v->h_arrivee, "%d:%d", &h, &mn) == 2) {
        spin_set (form, "spin_h_arr_aj", h);
        spin_set (form, "spin_min_arr_aj", mn);
    }
}

void
on_btn_modifier_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *principale = gtk_widget_get_toplevel (GTK_WIDGET (button));
    GtkTreeView *tv = GTK_TREE_VIEW (lookup_widget (GTK_WIDGET (button), "treeview_vols"));
    GtkTreeSelection *sel = gtk_tree_view_get_selection (tv);
    GtkTreeModel *model;
    GtkTreeIter iter;
    GtkWidget *form;
    gchar *id_txt = NULL;
    Vol v;
    int id;

    if (!gtk_tree_selection_get_selected (sel, &model, &iter)) {
        message_vol (principale, GTK_MESSAGE_WARNING,
                     "Selectionne d'abord un vol dans le tableau.");
        return;
    }
    gtk_tree_model_get (model, &iter, 0, &id_txt, -1);
    id = atoi (id_txt);
    g_free (id_txt);

    if (!chercher_vol (id, &v)) {
        message_vol (principale, GTK_MESSAGE_ERROR, "Ce vol est introuvable dans le fichier.");
        return;
    }

    form = CREER_FORMULAIRE ();
    preparer_formulaire (form);
    g_object_set_data (G_OBJECT (form), "mode", "modif");
    g_object_set_data (G_OBJECT (form), "principale", principale);
    gtk_window_set_title (GTK_WINDOW (form), "Modifier un vol");
    gtk_window_set_transient_for (GTK_WINDOW (form), GTK_WINDOW (principale));
    remplir_formulaire (form, &v);
    gtk_widget_show (form);
}


static void message_vol (GtkWidget *parent, GtkMessageType type, const char *texte)
{
  GtkWidget *dialogue = gtk_message_dialog_new (GTK_WINDOW (parent),
                                                GTK_DIALOG_MODAL, type,
                                                GTK_BUTTONS_OK, "%s", texte);
  gtk_dialog_run (GTK_DIALOG (dialogue));
  gtk_widget_destroy (dialogue);
}

void
on_btn_supprimer_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *fenetre = gtk_widget_get_toplevel (GTK_WIDGET (button));
  GtkWidget *treeview = lookup_widget (fenetre, "treeview_vols");
  GtkTreeSelection *selection;
  GtkTreeModel *model;
  GtkTreeIter iter;
  gchar *id_texte = NULL;
  GtkWidget *confirmation;
  gint reponse;
  char question[100];
  int id;

  selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (treeview));
  if (!gtk_tree_selection_get_selected (selection, &model, &iter))
    {
      message_vol (fenetre, GTK_MESSAGE_WARNING,
                   "Veuillez selectionner un vol a supprimer.");
      return;
    }

  gtk_tree_model_get (model, &iter, 0, &id_texte, -1);
  id = atoi (id_texte);
  g_free (id_texte);

  sprintf (question, "Voulez-vous vraiment supprimer le vol %d ?", id);
  confirmation = gtk_message_dialog_new (GTK_WINDOW (fenetre),
                                         GTK_DIALOG_MODAL,
                                         GTK_MESSAGE_QUESTION,
                                         GTK_BUTTONS_YES_NO, "%s", question);
  reponse = gtk_dialog_run (GTK_DIALOG (confirmation));
  gtk_widget_destroy (confirmation);
  if (reponse != GTK_RESPONSE_YES)
    return;

  if (supprimer_vol (id))
    {
      afficher_vols (GTK_TREE_VIEW (treeview), NULL);
      message_vol (fenetre, GTK_MESSAGE_INFO, "Vol supprime avec succes.");
    }
  else
    message_vol (fenetre, GTK_MESSAGE_ERROR, "Impossible de supprimer ce vol.");
}


void
on_btn_affecter_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{

}


#include <string.h>
#include <strings.h>
#include <ctype.h>

/* Lit un champ texte obligatoire : espaces retires, pas de ';', taille limitee. */
static int champ_valide (GtkWidget *form, const char *nom, const char *libelle,
                         char *dest, size_t taille)
{
    char tmp[200], msg[160];
    const char *txt;
    int n;

    snprintf (tmp, sizeof tmp, "%s",
              gtk_entry_get_text (GTK_ENTRY (lookup_widget (form, nom))));
    n = strlen (tmp);
    while (n > 0 && tmp[n - 1] == ' ')
        tmp[--n] = '\0';
    for (txt = tmp; *txt == ' '; txt++)
        ;

    msg[0] = '\0';
    if (*txt == '\0')
        snprintf (msg, sizeof msg, "Le champ \"%s\" est obligatoire.", libelle);
    else if (strchr (txt, ';'))
        snprintf (msg, sizeof msg, "Le champ \"%s\" ne doit pas contenir de ';'.", libelle);
    else if (strlen (txt) >= taille)
        snprintf (msg, sizeof msg, "Le champ \"%s\" est trop long (%d caracteres max).",
                  libelle, (int) taille - 1);

    if (msg[0] != '\0') {
        message_vol (form, GTK_MESSAGE_WARNING, msg);
        gtk_widget_grab_focus (lookup_widget (form, nom));
        return 0;
    }
    strcpy (dest, txt);
    return 1;
}

/* Minutes ecoulees depuis une date de reference (pour comparer deux instants) */
static long long minutes_abs (int j, int m, int a, int h, int mn)
{
    int y = a - (m <= 2);
    long long era = y / 400;
    int yoe = y - era * 400;
    int doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + j - 1;
    long long doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return (era * 146097 + doe) * 1440 + h * 60 + mn;
}

static int spin_val (GtkWidget *form, const char *nom)
{
    return gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (lookup_widget (form, nom)));
}

void
on_btn_enregistrer_aj_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *form = gtk_widget_get_toplevel (GTK_WIDGET (button));
    GtkWidget *principale = g_object_get_data (G_OBJECT (form), "principale");
    Vol v;
    char id_txt[40], duree[10], msg[160], *fin;
    Vol ancien;
    const char *mode = g_object_get_data (G_OBJECT (form), "mode");
    int modif = (mode != NULL && strcmp (mode, "modif") == 0);
    long id;
    int jd, md, ad, hd, mnd, ja, ma, aa, ha, mna, i;
    long long t_dep, t_arr;

    memset (&v, 0, sizeof v);

    /* --- ID --- */
    if (!champ_valide (form, "entry_id_aj", "ID du vol", id_txt, sizeof id_txt))
        return;
    id = strtol (id_txt, &fin, 10);
    if (*fin != '\0' || id <= 0 || id > 999999) {
        message_vol (form, GTK_MESSAGE_WARNING, "L'ID doit etre un nombre entier positif.");
        gtk_widget_grab_focus (lookup_widget (form, "entry_id_aj"));
        return;
    }
    if (!modif && vol_existe ((int) id)) {
        message_vol (form, GTK_MESSAGE_WARNING, "Un vol avec cet ID existe deja.");
        gtk_widget_grab_focus (lookup_widget (form, "entry_id_aj"));
        return;
    }
    v.id = (int) id;

    /* --- Champs texte --- */
    if (!champ_valide (form, "entry_depart_aj", "Ville de depart", v.depart, sizeof v.depart)
     || !champ_valide (form, "entry_destination_aj", "Destination", v.destination, sizeof v.destination)
     || !champ_valide (form, "entry_compagnie_aj", "Compagnie", v.compagnie, sizeof v.compagnie)
     || !champ_valide (form, "entry_aeroport_aj", "Aeroport", v.aeroport, sizeof v.aeroport))
        return;

    if (strcasecmp (v.depart, v.destination) == 0) {
        message_vol (form, GTK_MESSAGE_WARNING,
                     "La ville de depart et la destination doivent etre differentes.");
        return;
    }

    /* Code aeroport : 3 lettres, mis en majuscules */
    if (strlen (v.aeroport) != 3) {
        message_vol (form, GTK_MESSAGE_WARNING,
                     "Le code aeroport doit avoir 3 lettres (ex : TUN).");
        gtk_widget_grab_focus (lookup_widget (form, "entry_aeroport_aj"));
        return;
    }
    for (i = 0; i < 3; i++) {
        if (!isalpha ((unsigned char) v.aeroport[i])) {
            message_vol (form, GTK_MESSAGE_WARNING,
                         "Le code aeroport ne doit contenir que des lettres.");
            gtk_widget_grab_focus (lookup_widget (form, "entry_aeroport_aj"));
            return;
        }
        v.aeroport[i] = toupper ((unsigned char) v.aeroport[i]);
    }

    /* --- Dates, heures et duree (saisies a la main) --- */
    jd  = spin_val (form, "spin_jour_dep_aj");
    md  = spin_val (form, "spin_mois_dep_aj");
    ad  = spin_val (form, "spin_annee_dep_aj");
    hd  = spin_val (form, "spin_h_dep_aj");
    mnd = spin_val (form, "spin_min_dep_aj");
    ja  = spin_val (form, "spin_jour_arr_aj");
    ma  = spin_val (form, "spin_mois_arr_aj");
    aa  = spin_val (form, "spin_annee_arr_aj");
    ha  = spin_val (form, "spin_h_arr_aj");
    mna = spin_val (form, "spin_min_arr_aj");

    if (!date_existe (jd, md, ad)) {
        message_vol (form, GTK_MESSAGE_WARNING, "La date de depart n'existe pas.");
        return;
    }
    if (!date_existe (ja, ma, aa)) {
        message_vol (form, GTK_MESSAGE_WARNING, "La date d'arrivee n'existe pas.");
        return;
    }

    t_dep = minutes_abs (jd, md, ad, hd, mnd);
    t_arr = minutes_abs (ja, ma, aa, ha, mna);
    if (t_arr <= t_dep) {
        message_vol (form, GTK_MESSAGE_WARNING,
                     "L'arrivee doit etre apres le depart.");
        return;
    }

    if (hd == ha && mnd == mna) {
        message_vol (form, GTK_MESSAGE_WARNING,
                     "L'heure d'arrivee doit etre differente de l'heure de depart.");
        return;
    }

    snprintf (v.date, sizeof v.date, "%02d/%02d/%04d", jd, md, ad);
    snprintf (v.h_depart, sizeof v.h_depart, "%02d:%02d", hd, mnd);
    snprintf (v.date_arrivee, sizeof v.date_arrivee, "%02d/%02d/%04d", ja, ma, aa);
    snprintf (v.h_arrivee, sizeof v.h_arrivee, "%02d:%02d", ha, mna);
    calculer_duree (v.h_depart, v.h_arrivee, duree);
    snprintf (v.duree, sizeof v.duree, "%s", duree);

    /* Valeurs par defaut : l'avion sera affecte plus tard */
    if (modif && chercher_vol (v.id, &ancien)) {
        strcpy (v.avion, ancien.avion);
        strcpy (v.statut, ancien.statut);
    } else {
        strcpy (v.avion, "Non affecte");
        strcpy (v.statut, "Prevu");
    }


    if (modif ? !modifier_vol (v.id, v) : !ajouter_vol (v)) {
        message_vol (form, GTK_MESSAGE_ERROR, "Erreur : le vol n'a pas pu etre enregistre.");
        return;
    }

    gtk_widget_destroy (form);
    if (principale != NULL) {
        afficher_vols (GTK_TREE_VIEW (lookup_widget (principale, "treeview_vols")), NULL);
        snprintf (msg, sizeof msg, "Le vol %d a ete %s (duree : %s).",
                   v.id, modif ? "modifie" : "ajoute", duree);
        message_vol (principale, GTK_MESSAGE_INFO, msg);
    }
}
