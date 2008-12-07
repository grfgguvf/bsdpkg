
/*****************************************************************************/
/*                                                                           */
/*  Copyright (c) 2008, The DragonFly Project.  All rights reserved.         */
/*                                                                           */
/*  This code is derived from software contributed to The DragonFly Project  */
/*  by Saverio Iacovelli <shiftcoder@gmail.com>                              */
/*                                                                           */
/*  Redistribution and use in source and binary forms, with or without       */
/*  modification, are permitted provided that the following conditions       */
/*  are met:                                                                 */
/*                                                                           */
/*  1. Redistributions of source code must retain the above copyright        */
/*     notice, this list of conditions and the following disclaimer.         */
/*  2. Redistributions in binary form must reproduce the above copyright     */
/*     notice, this list of conditions and the following disclaimer in       */
/*     the documentation and/or other materials provided with the            */
/*     distribution.                                                         */
/*  3. Neither the name of The DragonFly Project nor the names of its        */
/*     contributors may be used to endorse or promote products derived       */
/*     from this software without specific, prior written permission.        */
/*                                                                           */
/*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS      */
/*  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT      */
/*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS        */
/*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE          */
/*  COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,    */
/*  INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES (INCLUDING,      */
/*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;         */
/*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED  */
/*  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT       */
/*  OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF       */
/*  SUCH DAMAGE.                                                             */
/*                                                                           */
/*****************************************************************************/





#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>

struct SNotebook {
  GtkNotebook   *notebook;
  GtkEntry      *entry;
  GtkButton     *button;
  const gchar   *label;
  GtkButton     *radio_button;
  GtkTextBuffer *buffer;
};

void warning_message() {

  GtkWidget *message_dialog;

  message_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "\nYou must insert package");
  gtk_window_set_title(GTK_WINDOW(message_dialog), "Warning Message");
  gtk_window_set_position(GTK_WINDOW(message_dialog), GTK_WIN_POS_MOUSE);
  gtk_dialog_run(GTK_DIALOG(message_dialog));
  gtk_widget_destroy(message_dialog);

}

void callback_close(GtkWidget *widget, gpointer callback_data) {

  gtk_widget_destroy(callback_data);

}

void callback_mirror(GtkButton *button, gpointer callback_data) {

  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *separator;
  GtkWidget *scrolled_window;
  GtkWidget *view;
  GtkWidget *save_button;
  GtkWidget *close_button;
  GtkTextBuffer *buffer;
  gchar *contents;
  GError  *error = NULL;

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 250);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  gtk_window_set_title(GTK_WINDOW(window), "mirror.conf");

  vbox = gtk_vbox_new(FALSE, 0);
  hbox = gtk_hbox_new(FALSE, 0);
  separator = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), separator, FALSE, FALSE, 5);

  view = gtk_text_view_new();
  scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_set_border_width(GTK_CONTAINER(scrolled_window), 0);
  gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 2);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window), view);

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
  g_file_get_contents("/usr/pkg/share/pkgsrcgfe/mirror.conf", &contents, NULL, &error);
  gtk_text_buffer_set_text(buffer, contents, -1);

  separator = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(vbox), separator, FALSE, FALSE, 5);
  hbox = gtk_hbox_new(FALSE, 0);
  save_button = gtk_button_new_with_mnemonic("_Save");
  gtk_widget_set_size_request(save_button, 60, 40);
  gtk_box_pack_start(GTK_BOX(hbox), save_button, FALSE, FALSE, 2);
  close_button = gtk_button_new_with_mnemonic("_Close");
  gtk_widget_set_size_request(close_button, 60, 40);
  gtk_box_pack_end(GTK_BOX(hbox), close_button, FALSE, FALSE, 2);
  g_signal_connect(G_OBJECT(close_button), "clicked", G_CALLBACK(callback_close), G_OBJECT(window));
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_MOUSE);

  gtk_widget_show_all(window);

}

void callback_about(GtkWidget *widget, gpointer callback_data) {

  GtkWidget *dialog;

  dialog = gtk_about_dialog_new();
  gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 300);
  gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "PkgSrcGFE");
  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "0.2");
  gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "(c) Saverio Iacovelli");
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "Lightweight GUI for PkgSrc");
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_MOUSE);
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);

}

void callback_toggled(GtkToggleButton *button, struct SNotebook *state) {

  state->radio_button = GTK_BUTTON(button);

}

void callback_run(GtkButton *button, struct SNotebook *state) {

  GtkWidget *view;
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *label;
  GtkWidget *window;
  GtkWidget *scrolled_window;
  GtkWidget *separator;
  GtkWidget *ok_button;
  GtkWidget *child;
  GtkTextBuffer *buffer;
  GtkTextIter   start, end, iter;
  GError *error = NULL;
  gint       num;
  gchar *command;
  char *opt;
  char *output;
  char *mirror;
  char *line;
  int size = 100;
  int nbyte = 0;
  int fd;
  

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 250);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  vbox = gtk_vbox_new(FALSE, 0);
  hbox = gtk_hbox_new(FALSE, 0);
  separator = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), separator, FALSE, FALSE, 5);

  view = gtk_text_view_new();
  scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_set_border_width(GTK_CONTAINER(scrolled_window), 0);
  gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 2);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window), view);

  separator = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(vbox), separator, FALSE, FALSE, 5);
  hbox = gtk_hbox_new(FALSE, 0);
  ok_button = gtk_button_new_with_mnemonic("_Ok");
  gtk_widget_set_size_request(ok_button, 60, 40);
  gtk_box_pack_end(GTK_BOX(hbox), ok_button, FALSE, FALSE, 2);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);
  g_signal_connect(G_OBJECT(ok_button), "clicked", G_CALLBACK(callback_close), G_OBJECT(window));

  num = gtk_notebook_get_current_page(state->notebook);
  child = gtk_notebook_get_nth_page(state->notebook, num);

  if (strcmp(gtk_notebook_get_tab_label_text(state->notebook, child), "Search") ==  0) {
    if ((g_strcmp0(gtk_entry_get_text(state->entry), "") != 0)) {
      if (g_strcmp0(gtk_button_get_label(state->radio_button), "query info") == 0)
        command = g_strconcat("pkg_search ", "-k ", gtk_entry_get_text(state->entry), NULL);
      if (g_strcmp0(gtk_button_get_label(state->radio_button), "show description") == 0)
        command = g_strconcat("pkg_search ", "-s ", gtk_entry_get_text(state->entry), NULL);
      if (g_strcmp0(gtk_button_get_label(state->radio_button), "verbose mode") == 0)
        command = g_strconcat("pkg_search ", "-v ", gtk_entry_get_text(state->entry), NULL);
    }
    else {
      warning_message();
      return;
    }
  }
  if (g_strcmp0(gtk_notebook_get_tab_label_text(state->notebook, child), " Add  ") ==  0) {
    if (g_strcmp0(gtk_entry_get_text(state->entry), "") != 0) {
      if (g_file_test("/usr/pkg/share/pkgsrcgfe/mirror.conf", G_FILE_TEST_EXISTS)) {
        fd = g_open("/usr/pkg/share/pkgsrcgfe/mirror.conf", O_RDONLY);
        if (fd < 0) perror("fd < 0");
        lseek(fd, 0, SEEK_SET);

        line = (char*)malloc(size);
        bzero(line, 100);
        nbyte = read(fd, line, size);

        mirror = (char*)malloc(size);
        bzero(mirror, 100);
        mirror = strncat(mirror, line, nbyte - 1);
      }
      if (g_strcmp0(gtk_button_get_label(state->radio_button), "force installation") == 0)
        command = g_strconcat("pkg_add ", "-f ", mirror, gtk_entry_get_text(state->entry), NULL);
      else if (g_strcmp0(gtk_button_get_label(state->radio_button), "only report") == 0)
        command = g_strconcat("pkg_add ", "-n ", mirror, gtk_entry_get_text(state->entry), NULL);
      else command = g_strconcat("pkg_add ", mirror, gtk_entry_get_text(state->entry), NULL);
    }
    else {
      warning_message();
      return;
    }
  }
  if (g_strcmp0(gtk_notebook_get_tab_label_text(state->notebook, child), " Info ") ==  0) {
    if (g_strcmp0(gtk_entry_get_text(state->entry), "") != 0) {
      command = g_strconcat("pkg_info ", gtk_entry_get_text(state->entry), NULL);
    }
    else {
      if (g_strcmp0(gtk_button_get_label(state->radio_button), "all package") == 0) command = g_strconcat("pkg_info ", "-a ", NULL);
      if (g_strcmp0(gtk_button_get_label(state->radio_button), "for all users") == 0) command = g_strconcat("pkg_info ", "-u ", NULL);
    }
  }
  if (g_strcmp0(gtk_notebook_get_tab_label_text(state->notebook, child), "Delete") ==  0) {
    if (g_strcmp0(gtk_entry_get_text(state->entry), "") != 0) {
      if (g_strcmp0(gtk_button_get_label(state->radio_button), "verbose") == 0)
        command = g_strconcat("pkg_delete ", "-v ", gtk_entry_get_text(state->entry), NULL);
      else
        command = g_strconcat("pkg_delete ", gtk_entry_get_text(state->entry), NULL);
    }
    else {
      warning_message();
      return;
    }
  }

  g_spawn_command_line_sync(command, &output, NULL, NULL, &error);
  g_free(command);

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
  gtk_text_buffer_set_text(buffer, output, -1);

  gtk_widget_show_all(window);

}




int main(int argc, char* argv[]) {

  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *valign;
  GtkWidget *page;
  GtkWidget *halign;
  GtkWidget *label;
  GtkWidget *tab_label;
  GtkWidget *entry;
  GtkWidget *separator;
  GtkWidget *mirror_button;
  GtkWidget *first_radio_button;
  GtkWidget *second_radio_button;
  GtkWidget *third_radio_button;
  GtkWidget *radio_button;
  GtkWidget *about_button;
  GtkWidget *run_button;
  GtkWidget *close_button;
  GtkWidget *notebook;
  GSList    *group;
  int        i;

  struct  SNotebook state;


  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "PkgSrc Graphical Front End");
  gtk_window_set_default_size(GTK_WINDOW(window), 450, 300);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  vbox = gtk_vbox_new(FALSE, 0);
  valign = gtk_alignment_new(0, 1, 0, 0);
  gtk_container_add(GTK_CONTAINER(vbox), valign);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);
  halign = gtk_alignment_new(1, 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(hbox), halign, FALSE, FALSE, 0);
  label = gtk_label_new("Package name: ");
  gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
  entry = gtk_entry_new();
  state.entry = GTK_ENTRY(entry);
  gtk_entry_set_editable(GTK_ENTRY(entry), TRUE);
  gtk_box_pack_start(GTK_BOX(hbox), entry, TRUE, TRUE, 0);
  mirror_button = gtk_button_new_with_mnemonic("_Mirror");
  gtk_widget_set_size_request(mirror_button, 80, 10);
  gtk_box_pack_start(GTK_BOX(hbox), mirror_button, FALSE, TRUE, 4);
  g_signal_connect(G_OBJECT(mirror_button), "clicked", G_CALLBACK(callback_mirror), NULL);

  separator = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(vbox), separator, TRUE, TRUE, 0);

  halign = gtk_alignment_new(1, 0, 0, 0);
  notebook = gtk_notebook_new();
  state.notebook = GTK_NOTEBOOK(notebook);
  gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_TOP);
  gtk_notebook_set_tab_border(GTK_NOTEBOOK(notebook), 5);
  gtk_notebook_set_show_border(GTK_NOTEBOOK(notebook), TRUE);
  for (i = 1; i <= 4; i++) {
    switch (i) {
      case 1: page = gtk_vbox_new(TRUE, 0);
              tab_label = gtk_label_new("Search");
              first_radio_button = gtk_radio_button_new_with_label_from_widget(NULL, NULL);
              g_signal_connect(G_OBJECT(first_radio_button), "toggled", G_CALLBACK(callback_toggled), &state);
              group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(first_radio_button));
              gtk_box_pack_start(GTK_BOX(page), first_radio_button, FALSE, FALSE, 0);
              radio_button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(first_radio_button), "query info");
              g_signal_connect(G_OBJECT(radio_button), "toggled", G_CALLBACK(callback_toggled), &state);
              group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio_button));
              gtk_box_pack_start(GTK_BOX(page), radio_button, TRUE, TRUE, 2);
              radio_button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio_button), "show description");
              g_signal_connect(G_OBJECT(radio_button), "toggled", G_CALLBACK(callback_toggled), &state);
              group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio_button));
              gtk_box_pack_start(GTK_BOX(page), radio_button, TRUE, TRUE, 2);
              radio_button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio_button), "verbose mode");
              g_signal_connect(G_OBJECT(radio_button), "toggled", G_CALLBACK(callback_toggled), &state);
              gtk_box_pack_start(GTK_BOX(page), radio_button, TRUE, FALSE, 2);
              gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page, tab_label);
              break;
      case 2: page = gtk_vbox_new(TRUE, 0);
              tab_label = gtk_label_new(" Add  ");
              second_radio_button = gtk_radio_button_new_with_label_from_widget(NULL, NULL);
              g_signal_connect(G_OBJECT(second_radio_button), "toggled", G_CALLBACK(callback_toggled), &state);
              group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(second_radio_button));
              gtk_box_pack_start(GTK_BOX(page), second_radio_button, FALSE, FALSE, 0);
              radio_button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(second_radio_button), "force installation");
              g_signal_connect(G_OBJECT(radio_button), "toggled", G_CALLBACK(callback_toggled), &state);
              group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio_button));
              gtk_box_pack_start(GTK_BOX(page), radio_button, TRUE, TRUE, 0);
              radio_button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio_button), "only report");
              g_signal_connect(G_OBJECT(radio_button), "toggled", G_CALLBACK(callback_toggled), &state);
              gtk_box_pack_start(GTK_BOX(page), radio_button, TRUE, TRUE, 0);
              gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page, tab_label);
              break;
      case 3: page = gtk_vbox_new(TRUE, 0);
              tab_label = gtk_label_new(" Info ");
              third_radio_button = gtk_radio_button_new_with_label_from_widget(NULL, NULL);
              g_signal_connect(G_OBJECT(third_radio_button), "toggled", G_CALLBACK(callback_toggled), &state);
              group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(third_radio_button));
              gtk_box_pack_start(GTK_BOX(page), third_radio_button, FALSE, FALSE, 0);
              radio_button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(third_radio_button), "all package");
              g_signal_connect(G_OBJECT(radio_button), "toggled", G_CALLBACK(callback_toggled), &state);
              group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio_button));
              gtk_box_pack_start(GTK_BOX(page), radio_button, TRUE, TRUE, 0);
              radio_button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio_button), "for all users");
              g_signal_connect(G_OBJECT(radio_button), "toggled", G_CALLBACK(callback_toggled), &state);
              gtk_box_pack_start(GTK_BOX(page), radio_button, TRUE, TRUE, 0);
              gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page, tab_label);
              break;
      case 4: page = gtk_vbox_new(TRUE, 0);
              tab_label = gtk_label_new("Delete");
              radio_button = gtk_check_button_new_with_label("verbose");
              g_signal_connect(G_OBJECT(radio_button), "toggled", G_CALLBACK(callback_toggled), &state);
              gtk_container_add(GTK_CONTAINER(page), radio_button);
              gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page, tab_label);
              break;
    }

  }



  gtk_box_pack_start(GTK_BOX(vbox), halign, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), notebook, TRUE, TRUE, 0);

  separator = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(vbox), separator, TRUE, FALSE, 0);

  halign = gtk_alignment_new(1, 0, 0, 0);
  hbox = gtk_hbox_new(TRUE, 0);
  about_button = gtk_button_new_with_mnemonic("_About");
  gtk_widget_set_size_request(about_button, 40, 20);
  gtk_box_pack_start(GTK_BOX(hbox), about_button, TRUE, TRUE, 2);
  g_signal_connect(G_OBJECT(about_button), "clicked", G_CALLBACK(callback_about), NULL);
  close_button = gtk_button_new_with_mnemonic("_Close");
  gtk_widget_set_size_request(close_button, 40, 20);
  gtk_box_pack_end(GTK_BOX(hbox), close_button, TRUE, TRUE, 2);
  g_signal_connect(G_OBJECT(close_button), "clicked", G_CALLBACK(gtk_main_quit), NULL);
  run_button = gtk_button_new_with_mnemonic("_Run");
  gtk_widget_set_size_request(run_button, 40, 20);
  gtk_box_pack_end(GTK_BOX(hbox), run_button, TRUE, TRUE, 2);
  g_signal_connect(G_OBJECT(run_button), "clicked", G_CALLBACK(callback_run), &state);

  gtk_box_pack_start(GTK_BOX(hbox), halign, FALSE, FALSE, 2);
  gtk_container_add(GTK_CONTAINER(vbox), hbox);

  g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), G_OBJECT(window));
  gtk_widget_show_all(window);
  gtk_widget_hide(first_radio_button);
  gtk_widget_hide(second_radio_button);
  gtk_widget_hide(third_radio_button);

  gtk_main();

  return 0;

}
