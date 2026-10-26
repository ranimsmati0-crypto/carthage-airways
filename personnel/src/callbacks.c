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
