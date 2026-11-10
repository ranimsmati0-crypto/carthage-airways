# Design commun Carthage Airways

Tous les modules utilisent le meme theme et le meme menu lateral.

## Fichiers partages (ne pas les modifier sans prevenir l'equipe)
- `data/theme.rc`            theme sombre (couleurs, boutons, tableaux)
- `pixmaps/*.png`            icones du menu
- `src/common/design.c/.h`   menu lateral + chargement du theme
- `src/common/demo_design.c` fenetre de demonstration (`make demo_design && ./demo_design avions`)

## Palette
| Role        | Couleur   |
|-------------|-----------|
| Fond        | `#0F1B2D` |
| Surface     | `#1B2B45` |
| Accent      | `#2F80ED` |
| Texte       | `#E6EDF7` |
| Page active | `#1F3A63` |

## Utiliser le design dans votre module
1. Dans Glade, la fenetre contient une `GtkHBox` nommee `hbox_principal`
   avec, dedans, votre contenu (titre, formulaires, tableau...). Ne dessinez PAS le menu.
2. Dans le makefile, ajoutez a CFLAGS `-Isrc/common` et a OBJ `src/common/design.o`.
3. Dans main.c, remplacez `gtk_rc_parse ("data/theme.rc");` par `design_init ();`
   (puis `#include "design.h"`).
4. Apres `create_fenetre_...()`, ajoutez :

       GtkWidget *hbox = lookup_widget (fenetre, "hbox_principal");
       GtkWidget *menu = design_menu_creer ("avions");   /* votre page */
       gtk_box_pack_start (GTK_BOX (hbox), menu, FALSE, FALSE, 0);
       gtk_box_reorder_child (GTK_BOX (hbox), menu, 0);

   Identifiants de page : tableau, vols, reservations, passagers, personnel,
   avions, aeroports, parametres.
5. Pour la navigation : `design_menu_action (menu, "vols", G_CALLBACK (ma_fonction), NULL);`

## Regles communes
- Titre de fenetre : "Gestion des xxx - Administrateur" ou "... - Passager".
- Noms de widgets en minuscules avec underscore, sans espace.
- Meme dossier pour les icones : `pixmaps/` a la racine du depot.
