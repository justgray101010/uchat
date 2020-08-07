#include "uchat.h"

void mx_uchat_window(t_user_info *user) {
    t_gtk_chat *wid = (t_gtk_chat*) malloc(sizeof(t_gtk_chat));
    
    mx_init_gtkchat(wid, user);
    wid->last_window = "/profile";
    gtk_window_set_position(GTK_WINDOW(wid->window), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_window_set_resizable (GTK_WINDOW(wid->window), FALSE);
    gtk_window_set_title (GTK_WINDOW (wid->window), "uChat");
    gtk_window_set_default_size (GTK_WINDOW (wid->window), 800, 600);
    mx_pack_chatwidgets(wid);
    gtk_container_add(GTK_CONTAINER(wid->window), wid->box);
    gtk_widget_show_all (wid->window);
}

void mx_init_gtkchat(t_gtk_chat *w, t_user_info *user) {
    char buf[256] = "Login: ";

    w->n_sms = 0;
    w->set = NULL;
    w->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    w->box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    w->box_menu = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(w->box_menu, 100, 200);
    w->separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    w->profile = gtk_button_new_with_label("Profile");
    w->friends = gtk_button_new_with_label("Friends");
    w->messages = gtk_button_new_with_label("Messages");
    w->server = gtk_button_new_with_label("Server");
    w->connect = gtk_button_new_with_label("Connect");
    w->settings = gtk_button_new_with_label("Settings");
    w->log_out = gtk_button_new_with_label("Log out");
    strcat(buf, user->login);
    w->prof = (t_prof*) malloc(sizeof(t_prof));
    w->serv = (t_serv*) malloc(sizeof(t_serv));
    w->set = (t_set*) malloc(sizeof(t_set)); 
    w->prof->login = gtk_label_new(buf);
    bzero(buf, 256);
    strcat(buf, "Name: ");
    strcat(buf, user->name);
    w->prof->name = gtk_label_new(buf);
    w->prof->avatar = gtk_image_new_from_file("media/img/client_default.png");
    w->prof->tags = gtk_label_new("No tags yet");
    w->prof->box1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    w->prof->box2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    w->prof->box3 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
}

void mx_pack_chatwidgets(t_gtk_chat *wid) {
    gtk_box_pack_start(GTK_BOX(wid->box_menu), wid->profile, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(wid->box_menu), wid->friends, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(wid->box_menu), wid->messages, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(wid->box_menu), wid->server, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(wid->box_menu), wid->connect, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(wid->box_menu), wid->settings, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(wid->box_menu), wid->log_out, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(wid->prof->box1), wid->prof->login, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(wid->prof->box1), wid->prof->name, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(wid->prof->box2), wid->prof->avatar, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(wid->prof->box2), wid->prof->box1, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(wid->prof->box3), wid->prof->box2, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(wid->prof->box3), wid->prof->tags, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(wid->box), wid->box_menu, 1, 0, 0);
    gtk_box_pack_start(GTK_BOX(wid->box), wid->separator, 1, 0, 0);
    gtk_box_pack_start(GTK_BOX(wid->box), wid->prof->box3, 1, 0, 30);
    mx_signal_buts(wid);
}

void mx_profile(GtkWidget *widget, t_gtk_chat *w) {
    char *name;
    int sock;
    char buf[256];
    ssize_t n;

    (void)widget;
    mx_hide_last_win(w, w->last_window);
    w->last_window = "/profile";
    sock = mx_get_socket(0);
    write(sock, "/profile", strlen("/profile"));
    sleep(1);
    n = read(sock, buf, 256);
    name = strndup(buf, n);
    bzero(buf, 256);
    strcpy(buf, "Name: ");
    strcat(buf, name);
    gtk_label_set_text((GtkLabel*)w->prof->name, buf);
    gtk_widget_show(w->prof->box3);
}

void mx_quit(GtkWidget *widget, t_gtk_chat *w) {
    int sock;

    (void)widget;
    gtk_widget_destroy(w->window);
    sock = mx_get_socket(0);
    write(sock, "/quit", strlen("/quit"));
    free(w);
    exit(0);
}

int mx_logout(GtkWidget *widget, t_gtk_chat *w) {
    int sock;

    (void)widget;
    sock = mx_get_socket(0);
    write(sock, "/logout", strlen("/logout"));
    gtk_widget_hide(w->window);
    sleep(1);
    mx_login_window(NULL);
    gtk_widget_destroy(w->window);
    return 0;
}
