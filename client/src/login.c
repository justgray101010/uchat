#include "uchat.h"

void mx_login_window() {
    t_widget *log_w = (t_widget*) malloc(sizeof(t_widget));
    
    mx_init_widget_struct(log_w);
    gtk_window_set_position(GTK_WINDOW(log_w->window), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_entry_set_visibility(GTK_ENTRY(log_w->password_e), FALSE);
    gtk_window_set_resizable (GTK_WINDOW(log_w->window), FALSE);
    gtk_window_set_title (GTK_WINDOW (log_w->window), "uChat");
    gtk_window_set_default_size (GTK_WINDOW (log_w->window), 300, 200);
    mx_pack_all_widgets(log_w);
    gtk_container_add(GTK_CONTAINER(log_w->window), log_w->box);
    g_signal_connect (log_w->login_button, "clicked", G_CALLBACK(mx_check_login), log_w);
    g_signal_connect (log_w->reg_button, "clicked", G_CALLBACK(mx_register), log_w);
    gtk_widget_show_all (log_w->window);
    gtk_main();
}

void mx_init_widget_struct(t_widget *log_w) {
    PangoAttrList *attrlist = pango_attr_list_new();
    PangoAttribute *attr = pango_attr_size_new_absolute(24 * PANGO_SCALE);

    log_w->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    log_w->uchatl = gtk_label_new("uChat");
    pango_attr_list_insert(attrlist, attr);
    gtk_label_set_attributes(GTK_LABEL(log_w->uchatl), attrlist);
    pango_attr_list_unref(attrlist);
    log_w->box_uchatl = gtk_box_new(0, 0);
    log_w->box = gtk_box_new(1, 0);
    log_w->box_logbut = gtk_box_new(1, 0);
    log_w->box_logerrl = gtk_box_new(0, 0);
    log_w->box_log = gtk_box_new(0, 10);
    log_w->login_e = gtk_entry_new();
    log_w->login_l = gtk_label_new("Login");
    log_w->box_pw = gtk_box_new(0, 10);
    log_w->password_e = gtk_entry_new();
    log_w->password_l = gtk_label_new("Password");
    log_w->login_button = gtk_button_new_with_label("Log In");
    log_w->login_error = gtk_label_new("");
    log_w->reg_button = gtk_button_new_with_label("I Am New Here");
    log_w->box_regb = gtk_box_new(1, 0);

    //CSS стили {
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "theme.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                               GTK_STYLE_PROVIDER(cssProvider),
                               GTK_STYLE_PROVIDER_PRIORITY_USER);
//          }
}

void mx_pack_all_widgets(t_widget *all) {
    gtk_widget_set_size_request(all->login_l, 100, 40);
    gtk_widget_set_size_request(all->password_l, 100, 40);
    gtk_widget_set_size_request(all->login_e, 20, 10);
    gtk_widget_set_size_request(all->password_e, 20, 10);
    gtk_widget_set_size_request(all->login_button, 20, 10);
    gtk_widget_set_size_request(all->reg_button, 20, 10);
    gtk_box_pack_start(GTK_BOX(all->box_uchatl), all->uchatl, TRUE, FALSE, 20);
    gtk_box_pack_start(GTK_BOX(all->box_log), all->login_l, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(all->box_log), all->login_e, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(all->box_pw), all->password_l, FALSE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(all->box_pw), all->password_e, FALSE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(all->box_logerrl), all->login_error,
                    FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(all->box_logbut), all->login_button,
                    TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(all->box_regb), all->reg_button, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(all->box), all->box_uchatl, FALSE, FALSE, 20);
    gtk_box_pack_start(GTK_BOX(all->box), all->box_log, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(all->box), all->box_pw, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(all->box), all->box_logbut, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(all->box), all->box_regb, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(all->box), all->box_logerrl, FALSE, FALSE, 10);
    g_signal_connect(G_OBJECT(all->window), "destroy",
                    G_CALLBACK(gtk_main_quit), NULL);
}

void mx_check_login(GtkWidget*widget, t_widget *log_w) {
    t_client_context *client = (t_client_context*) malloc(sizeof(t_client_context));
    int sock;
    ssize_t n;
    char buf[256];
    t_user_info *user;
    char *checkval;

    (void)widget;
    client->login = (char*)gtk_entry_get_text((GtkEntry*)log_w->login_e);
    client->password = (char*)gtk_entry_get_text((GtkEntry*)log_w->password_e);
    sock = mx_connect_to_server();
    if (sock == -1) {
        gtk_label_set_text((GtkLabel*)log_w->login_error, "ERROR: Server does not respond");
        gtk_widget_show_all(log_w->window);
        return ;
    }
    mx_get_socket(sock);
    write(sock, "/login", strlen("/login"));
    sleep(1);
    write(sock, client->login, strlen(client->login));
    sleep(1);
    write(sock, client->password, strlen(client->password));
    n = read(sock, buf, 256);
    checkval = strndup(buf, n);
    bzero(buf, 256);
    if (strcmp(checkval, "accepted") == 0) {
        user = mx_create_user_info((char*)client->login);
        n = read(sock, buf, 256);
        user->name = strndup(buf, n);
        gtk_widget_hide(log_w->window);
        free(log_w);
        mx_uchat_window(user);
    }
    else {
        if (strcmp(checkval, "wrong password") == 0) {
            gtk_label_set_text((GtkLabel*)log_w->login_error, "ERROR: Incorrect Password");
        }
        if (strcmp(checkval, "incorrect input") == 0) {
            gtk_label_set_text((GtkLabel*)log_w->login_error, "ERROR: Incorrect Input");
        }
    }
}
