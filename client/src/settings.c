#include "uchat.h"

static void activate_cb(GObject *switcher, GParamSpec *pspec, 
                        t_gtk_chat *w) {
    (void)pspec;
    if (gtk_switch_get_active (GTK_SWITCH (switcher))) {
        GtkCssProvider *cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_path(cssProvider, "dark_theme.css", NULL);
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                               GTK_STYLE_PROVIDER(cssProvider),
                               GTK_STYLE_PROVIDER_PRIORITY_USER);
        sleep(1);
        gtk_widget_show(w->set->box);
    }  
    else {
        GtkCssProvider *cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_path(cssProvider, "theme.css", NULL);
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                               GTK_STYLE_PROVIDER(cssProvider),
                               GTK_STYLE_PROVIDER_PRIORITY_USER);
        sleep(1);
        gtk_widget_show(w->set->box);
    }
}

static void mx_on_eng(GtkWidget *widget, t_gtk_chat *w) {
    (void)widget;
    w->set->feng = 0;
    w->set->frus = 1;
    gtk_button_set_label(GTK_BUTTON(w->profile), "Profile");
    gtk_button_set_label(GTK_BUTTON(w->friends), "Friends");
    gtk_button_set_label(GTK_BUTTON(w->messages), "Messages");
    gtk_button_set_label(GTK_BUTTON(w->server), "Server");
    gtk_button_set_label(GTK_BUTTON(w->connect), "Connect");
    gtk_button_set_label(GTK_BUTTON(w->settings), "Settings");
    gtk_button_set_label(GTK_BUTTON(w->log_out), "Log out");
    gtk_label_set_text(GTK_LABEL(w->set->title1), "Visual Changes");
    gtk_label_set_text(GTK_LABEL(w->set->theme), "Night mode");
    gtk_label_set_text(GTK_LABEL(w->set->lang), "Language");
    gtk_label_set_text(GTK_LABEL(w->set->title2), "Profile Setings");
    gtk_label_set_text(GTK_LABEL(w->set->name_l), "Change name");
    gtk_label_set_text(GTK_LABEL(w->set->photo_l), "Change image");
    gtk_button_set_label(GTK_BUTTON(w->set->tags_b), "Choose tags");
    gtk_button_set_label(GTK_BUTTON(w->set->confirm), "Accept");
}

static void mx_on_rus(GtkWidget *widget, t_gtk_chat *w) {
    (void)widget;
    w->set->feng = 1;
    w->set->frus = 0;
    gtk_button_set_label(GTK_BUTTON(w->profile), "Профиль");
    gtk_button_set_label(GTK_BUTTON(w->friends), "Друзья");
    gtk_button_set_label(GTK_BUTTON(w->messages), "Сообщения");
    gtk_button_set_label(GTK_BUTTON(w->server), "Сервер");
    gtk_button_set_label(GTK_BUTTON(w->connect), "Соединить");
    gtk_button_set_label(GTK_BUTTON(w->settings), "Настройки");
    gtk_button_set_label(GTK_BUTTON(w->log_out), "Выйти");
    gtk_label_set_text(GTK_LABEL(w->set->title1), "Визуальные Изменения");
    gtk_label_set_text(GTK_LABEL(w->set->theme), "Ночной Мод");
    gtk_label_set_text(GTK_LABEL(w->set->lang), "Язык");
    gtk_label_set_text(GTK_LABEL(w->set->title2), "Настройки Профиля");
    gtk_label_set_text(GTK_LABEL(w->set->name_l), "Изменить Имя");
    gtk_label_set_text(GTK_LABEL(w->set->photo_l), "Изменить фото");
    gtk_button_set_label(GTK_BUTTON(w->set->tags_b), "Выбрать Теги");
    gtk_button_set_label(GTK_BUTTON(w->set->confirm), "Принять");
}

void mx_settings(GtkWidget *widget, t_gtk_chat *w) {
    int sock;

    (void)widget;
    mx_hide_last_win(w, w->last_window);
    w->last_window = "/settings";
    sock = mx_get_socket(0);
    write(sock, "/settings", strlen("/settings"));
    mx_init_set(w->set);
    mx_pack_set(w->set);
    gtk_box_pack_start(GTK_BOX(w->box), w->set->box, 1, 0, 10);
    g_signal_connect(GTK_SWITCH(w->set->switcher), 
                    "notify::active", 
                    G_CALLBACK (activate_cb), 
                    w);
    g_signal_connect(GTK_TOGGLE_BUTTON(w->set->eng), "toggled", 
                    G_CALLBACK (mx_on_eng), w);
    g_signal_connect(GTK_TOGGLE_BUTTON (w->set->rus), "toggled", 
                    G_CALLBACK (mx_on_rus), w);
    g_signal_connect(G_OBJECT(w->set->confirm), "clicked",
                    G_CALLBACK(mx_confirm), w);
    gtk_widget_show_all(w->set->box);
}

void mx_init_set(t_set *s) {
    s->box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    s->box1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    s->title1 = gtk_label_new("Visual Settings");
    s->hbox1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    s->theme = gtk_label_new("Night Mode");
    s->switcher = gtk_switch_new();
    s->lang = gtk_label_new("Language");
    s->eng = gtk_radio_button_new_with_label (NULL, "English");
    s->rus = gtk_radio_button_new_with_label_from_widget
            (GTK_RADIO_BUTTON(s->eng), "Russian");
    s->box2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    s->title2 = gtk_label_new("Profile Settings");
    s->hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    s->name_l = gtk_label_new("Change Name");
    s->name_e = gtk_entry_new();
    s->hbox3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    s->photo_l = gtk_label_new("Choose Photo");
    s->photo_e = gtk_entry_new();
    s->tags_b = gtk_button_new_with_label("Tags");
    s->confirm = gtk_button_new_with_label("Confirm");
}

void mx_pack_set(t_set *w) {
    gtk_box_pack_start(GTK_BOX(w->hbox1), w->theme, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->hbox1), w->switcher, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->box1), w->title1, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->box1), w->hbox1, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->box1), w->lang, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->box1), w->eng, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->box1), w->rus, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->hbox2), w->name_l, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->hbox2), w->name_e, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->hbox3), w->photo_l, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->hbox3), w->photo_e, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->box2), w->title2, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->box2), w->hbox2, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->box2), w->hbox3, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->box2), w->tags_b, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->box2), w->confirm, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->box), w->box1, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->box), w->box2, 1, 0, 10);
}

void mx_confirm(GtkWidget *widget, t_gtk_chat *w) {
    int sock;
    char *new_name;

    (void)widget;
    sock = mx_get_socket(0);
    write(sock, "/edit_prof", strlen("/edit_prof"));
    sleep(1);
    new_name = (char*)gtk_entry_get_text((GtkEntry*)w->set->name_e);
    write(sock, new_name, strlen(new_name));
    sleep(1);
    gtk_entry_set_text(GTK_ENTRY(w->set->name_e), "");
    gtk_entry_set_text(GTK_ENTRY(w->set->photo_e), "");
    gtk_widget_show(w->set->box);
}
