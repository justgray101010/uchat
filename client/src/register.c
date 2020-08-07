#include "uchat.h"

void mx_register(GtkWidget*widget, t_widget *log) {
    t_widget2 *reg = (t_widget2*) malloc(sizeof(t_widget2));
    
    (void)widget;
    gtk_widget_hide(log->window);
    mx_init_widget2_struct(reg);
    reg->log_copy = log;
    gtk_window_set_position(GTK_WINDOW(reg->window), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_entry_set_visibility(GTK_ENTRY(reg->password_e), FALSE);
    gtk_entry_set_visibility(GTK_ENTRY(reg->confirm_e), FALSE);
    gtk_window_set_resizable (GTK_WINDOW(reg->window), FALSE);
    gtk_window_set_title (GTK_WINDOW (reg->window), "uChat");
    gtk_window_set_default_size (GTK_WINDOW (reg->window), 300, 200);
    mx_pack_all_widget2_struct(reg);
    gtk_container_add(GTK_CONTAINER(reg->window), reg->box);
    gtk_widget_show_all (reg->window);
    g_signal_connect (reg->reg_button, "clicked", G_CALLBACK(mx_usernew), reg);
}

void mx_init_widget2_struct(t_widget2 *wid) {
    PangoAttrList *attrlist = pango_attr_list_new();
    PangoAttribute *attr = pango_attr_size_new_absolute(24 * PANGO_SCALE);

    wid->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    wid->box = gtk_box_new(1, 0);
    wid->title = gtk_label_new("Create Your uChat Account");
    wid->box_title = gtk_box_new(0, 20);
    pango_attr_list_insert(attrlist, attr);
    gtk_label_set_attributes(GTK_LABEL(wid->title), attrlist);
    pango_attr_list_unref(attrlist);
    wid->login_l = gtk_label_new("Create Login");
    wid->login_e = gtk_entry_new();
    wid->box_log = gtk_box_new(0, 10);
    wid->password_l = gtk_label_new("Create Password");
    wid->password_e = gtk_entry_new();
    wid->box_pw = gtk_box_new(0, 10);
    wid->confirm_l = gtk_label_new("Confirm Password");
    wid->confirm_e = gtk_entry_new();
    wid->box_conf = gtk_box_new(0, 10);
    wid->error_l = gtk_label_new("");
    wid->box_err = gtk_box_new(0, 0);
    wid->reg_button = gtk_button_new_with_label("Register");
    wid->box_regb = gtk_box_new(1, 0);
}

void mx_pack_all_widget2_struct(t_widget2 *reg) {
    gtk_box_pack_start(GTK_BOX(reg->box_title), reg->title, 1, 0, 20);
    gtk_box_pack_start(GTK_BOX(reg->box_log), reg->login_l, 1, 1, 10);
    gtk_box_pack_start(GTK_BOX(reg->box_log), reg->login_e, 1, 1, 10);
    gtk_box_pack_start(GTK_BOX(reg->box_pw), reg->password_l, 0, 1, 10);
    gtk_box_pack_start(GTK_BOX(reg->box_pw), reg->password_e, 0, 1, 10);
    gtk_box_pack_start(GTK_BOX(reg->box_conf), reg->confirm_l, 1, 1, 10);
    gtk_box_pack_start(GTK_BOX(reg->box_conf), reg->confirm_e, 1, 1, 10);
    gtk_box_pack_start(GTK_BOX(reg->box_err), reg->error_l, 1, 0, 0);
    gtk_box_pack_start(GTK_BOX(reg->box_regb), reg->reg_button, 1, 1, 10);
    gtk_box_pack_start(GTK_BOX(reg->box), reg->box_title, 0, 0, 20);
    gtk_box_pack_start(GTK_BOX(reg->box), reg->box_log, 0, 0, 5);
    gtk_box_pack_start(GTK_BOX(reg->box), reg->box_pw, 0, 0, 5);
    gtk_box_pack_start(GTK_BOX(reg->box), reg->box_conf, 0, 0, 5);
    gtk_box_pack_start(GTK_BOX(reg->box), reg->box_err, 0, 0, 10);
    gtk_box_pack_start(GTK_BOX(reg->box), reg->box_regb, 0, 0, 0);
    g_signal_connect(G_OBJECT(reg->window), "destroy",
                    G_CALLBACK(mx_login_window), NULL);
}

void mx_usernew(GtkWidget*widget, t_widget2 *reg) {
    char *login;
    char *password;
    char *confirm;
    int sock;
    t_user_info *user;

    (void)widget;
    sock = mx_connect_to_server(reg);
    if (sock == -1) {
        mx_show_reg_error(-1, reg);
        return ;
    }
    mx_get_socket(sock);
    login = (char*)gtk_entry_get_text((GtkEntry*)reg->login_e);
    password = (char*)gtk_entry_get_text((GtkEntry*)reg->password_e);
    confirm = (char*)gtk_entry_get_text((GtkEntry*)reg->confirm_e);
    if (strlen(login) < 2 || strlen(password) < 3) {
        mx_show_reg_error(-2, reg);
        return ;
    }
    if (strlen(login) > 20 || strlen(password) > 20) {
        mx_show_reg_error(-3, reg);
        return ;
    }
    if (strcmp(password, confirm) != 0) {
        mx_show_reg_error(-4, reg);
        return ;
    }
    write(sock, "/register", strlen("/register"));
    sleep(1);
    write(sock, login, strlen(login));
    sleep(1);
    write(sock, password, strlen(password));
    gtk_widget_hide(reg->window);
    user = mx_create_user_info(login);
    free(reg);
    mx_uchat_window(user);
}

t_user_info *mx_create_user_info(char *username) {
    t_user_info *new_user = (t_user_info*) malloc(sizeof(t_user_info));

    new_user->login = strdup(username);
    new_user->name = strdup(username);
    return new_user;
}

void mx_show_reg_error(int code, t_widget2 *reg) {
    if (code == -1) {
        gtk_label_set_text((GtkLabel*)reg->error_l, "ERROR: Server does not respond");
    }
    if (code == -2) {
        gtk_label_set_text((GtkLabel*)reg->error_l, "ERROR: Too short login or password");
    }
    if (code == -3) {
        gtk_label_set_text((GtkLabel*)reg->error_l, "ERROR: Too long login or password");
    }
    if (code == -4) {
        gtk_label_set_text((GtkLabel*)reg->error_l, "ERROR: Passwords are not equal");
    }
    gtk_widget_show_all(reg->window);
}
