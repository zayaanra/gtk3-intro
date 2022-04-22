/* gcc -Wall -Wextra -o intro intro.c $(pkg-config gtk+-3.0 --cflags --libs) */

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Widgets {
  GtkWidget *window;
  GtkWidget *incButton;
  GtkWidget *decButton;
  GtkWidget *resetButton;
  GtkWidget *closeButton;
  GtkWidget *dialog;
  GtkWidget *counterLabel;
} Widgets;

void set_label(Widgets *widgets, gint counter);
void up_btn_clicked(__attribute__((unused)) GtkWidget *upButton, Widgets *widgets);
void down_btn_clicked(__attribute__((unused)) GtkWidget *upButton, Widgets *widgets);
gboolean check_entry(GtkWidget *entry, gint counter);
void goal_reached(Widgets *widgets);
void reset_btn_clicked(__attribute__((unused)) GtkWidget *resetButton, Widgets *widgets);
void close_btn_clicked(__attribute__((unused)) GtkWidget *closeButton, Widgets *widgets);


static GtkWidget *entry;
//static GtkBuilder *builder;
/* Updates the label depending on what button user pressed */
void set_label(Widgets *widgets, gint counter) {
  /* Allocates a new label based on what counter is and sets the string to whatever the new label is.
     Free the newly allocated memory afterwards. */
  gchar *newLabel;
  newLabel = g_strdup_printf("%d", counter);
  gtk_label_set_text(GTK_LABEL(widgets->counterLabel), newLabel);
  g_free(newLabel);
}

/* Increments the counter */
void up_btn_clicked(__attribute__((unused)) GtkWidget *upButton, Widgets *widgets) {
  /* Get what the label says, cast it to a gint and increment by 1 */
  gint counter = (gint) atoi(gtk_label_get_text(GTK_LABEL(widgets->counterLabel)));
  counter += 1;
  set_label(widgets, counter);
  if(check_entry(entry, counter) == TRUE) {
    goal_reached(widgets);
  }
}

/* Decrements the counter */
void down_btn_clicked(__attribute__((unused)) GtkWidget *downButton, Widgets *widgets) {
  /* Get what the label says, cast it to a gint and decrement by 1 */
  gint counter = (gint) atoi(gtk_label_get_text(GTK_LABEL(widgets->counterLabel))); 
  counter -= 1;
  set_label(widgets, counter);
  if(check_entry(entry, counter) == TRUE) {
    goal_reached(widgets);
  }
}

/* Checks what the current text inside entry is and sees if it matches the current counter */
gboolean check_entry(GtkWidget *entry, gint counter) {
  return (gint) atoi(gtk_entry_get_text(GTK_ENTRY(entry))) == counter;
}

void goal_reached(Widgets *widgets) {
  gtk_window_set_transient_for(GTK_WINDOW(widgets->dialog), GTK_WINDOW(widgets->window));
  gtk_widget_show_all(widgets->dialog);
  
}

void reset_btn_clicked(__attribute__((unused)) GtkWidget *resetButton, Widgets *widgets) {
  set_label(widgets, 0);
}
void close_btn_clicked(__attribute__((unused)) GtkWidget *closeButton, Widgets *widgets) {
  gtk_widget_destroy(widgets->dialog);
  g_signal_connect_swapped(widgets->dialog, "response", G_CALLBACK(gtk_widget_destroy), widgets->dialog);
}

int main(int argc, char *argv[]) {
  Widgets *widgets = g_malloc(sizeof(Widgets));
  
  GtkBuilder *builder = NULL;
  GError *error = NULL;

  /* Init gtk */
  gtk_init(&argc, &argv);

  /* Create builder and check if it was graceful */
  builder = gtk_builder_new();
  if(gtk_builder_add_from_file(builder, "app.glade", &error) == 0) {
    g_print("gtk_builder_add_from_file FAILED\n");
    g_print("%s\n", error->message);
    return -1;
  }

  /* Create widgets */
  widgets->window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
  widgets->incButton = GTK_WIDGET(gtk_builder_get_object(builder, "upButton"));
  widgets->decButton = GTK_WIDGET(gtk_builder_get_object(builder, "downButton"));
  widgets->resetButton = GTK_WIDGET(gtk_builder_get_object(builder, "reset"));
  widgets->closeButton = GTK_WIDGET(gtk_builder_get_object(builder, "close"));
  widgets->dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialogBox"));
  entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry"));
  widgets->counterLabel = GTK_WIDGET(gtk_builder_get_object(builder, "counter"));

  /* Connect signals for widgets */
  g_signal_connect(G_OBJECT(widgets->incButton), "clicked", G_CALLBACK(up_btn_clicked), widgets);
  g_signal_connect(G_OBJECT(widgets->decButton), "clicked", G_CALLBACK(down_btn_clicked), widgets);
  g_signal_connect(G_OBJECT(widgets->resetButton), "clicked", G_CALLBACK(reset_btn_clicked), widgets);
  g_signal_connect(G_OBJECT(widgets->closeButton), "clicked", G_CALLBACK(close_btn_clicked), widgets);
  g_signal_connect(G_OBJECT(widgets->window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

  /* Show all widgets within window and run main loop */
  gtk_widget_show_all(widgets->window);
  gtk_main();
  
  g_free(widgets);
  
  return 0;
}
