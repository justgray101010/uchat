#include "uchat.h"

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;
    
    argc = 0;
    mx_get_ip(argv[1]);
    mx_get_port(argv[2]);
    app = gtk_application_new ("com.github.justgray101010.uchat", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK(mx_login_window), NULL);
    status = g_application_run (G_APPLICATION (app), 0, 0);
    g_object_unref (app);
    return status;
}

void mx_signal_buts(t_gtk_chat *w) {
    g_signal_connect(G_OBJECT(w->window), "destroy",
                    G_CALLBACK(mx_quit), w);
    g_signal_connect(G_OBJECT(w->log_out), "clicked",
                    G_CALLBACK(mx_logout), w);
    g_signal_connect(G_OBJECT(w->settings), "clicked",
                    G_CALLBACK(mx_settings), w);
    g_signal_connect(G_OBJECT(w->profile), "clicked",
                    G_CALLBACK(mx_profile), w);
    g_signal_connect(G_OBJECT(w->server), "clicked",
                    G_CALLBACK(mx_server), w);
    g_signal_connect(G_OBJECT(w->messages), "clicked",
                    G_CALLBACK(mx_messages), w);
}

void mx_create_row_c(t_chats *w, t_row_c *row, char *name, int i, int flag) {
    row->row = gtk_list_box_row_new();
    gtk_list_box_insert((GtkListBox *)w->list_box, row->row, i);
    row->hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    row->name = gtk_button_new_with_label(name);
    row->msg = gtk_label_new("<Time> <Last Message>");
    row->vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    if (flag == 1)
        row->noti = gtk_label_new("New message here!");
    else
        row->noti = gtk_label_new("");
    gtk_container_add(GTK_CONTAINER(row->row), row->hbox);
    gtk_box_pack_start(GTK_BOX(row->hbox), row->vbox,
                       FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row->hbox), row->noti,
                       FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row->vbox), row->name,
                       FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row->vbox), row->msg,
                       FALSE, FALSE, 0);                   
    gtk_widget_set_size_request(row->row, 100, 20);
}

void mx_init_row_c(t_chats *w, int count) {
    for (int i = 0; i < count; i++) {
        t_row_c *row = (t_row_c*) malloc(sizeof(t_row_c));

        mx_create_row_c(w, row, w->chats[i], i, 0);
    }
    g_idle_add ((int (*)(void *))mx_show_widget, w->box);
}

void mx_get_chats(t_gtk_chat *w, int sock, int count) {
    char buf[256];
    ssize_t n;

    w->chat = (t_chats*) malloc(sizeof(t_chats)); 
    w->chat->chats = (char**) malloc(sizeof(char*) * (count + 1));
    for (int i = 0; i < count; i++) {
        n = read(sock, buf, 256);
        w->chat->chats[i] = strndup(buf, n);
        bzero(buf, 256);  
    }
    mx_init_chats(w->chat);
    mx_pack_chats(w);
    mx_init_row_c(w->chat, count);
    g_idle_add ((int (*)(void *))mx_show_widget, w->chat->box);
    gtk_widget_show(w->chat->box);
}

void mx_init_chats(t_chats *w) {
    w->box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    w->hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    w->label = gtk_label_new("Write to:");
    w->entry = gtk_entry_new();
    w->button = gtk_button_new_with_label("Find");
    w->error = gtk_label_new("");
    gtk_widget_set_size_request(w->button, 50, 20);
    w->box_scroll = gtk_box_new(0,0);
    w->win_scroll = gtk_scrolled_window_new(0,0);
    gtk_widget_set_size_request(w->win_scroll, 500, 300);
    w->list_box = gtk_list_box_new();
}

void mx_pack_chats(t_gtk_chat *w) {
    gtk_box_pack_start(GTK_BOX(w->box), w->chat->box, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->chat->hbox), w->chat->label, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->chat->hbox), w->chat->entry, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->chat->hbox), w->chat->button, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->chat->box), w->chat->hbox, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->chat->box), w->chat->error, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->chat->box), w->chat->box_scroll, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->chat->box_scroll), w->chat->win_scroll,
                        1, 0, 10);
    gtk_container_add(GTK_CONTAINER(w->chat->win_scroll), w->chat->list_box);
}

void mx_messages(GtkWidget *widget, t_gtk_chat *w) {
    int sock;
    char buf[256];
    ssize_t n;
    char *s_c;

    (void)widget;
    mx_hide_last_win(w, w->last_window);
    w->last_window = "/messages";
    sock = mx_get_socket(0);
    write(sock, "/messages", strlen("/messages"));
    sleep(1);
    n = read(sock, buf, 256);
    s_c = strndup(buf, n);
    bzero(buf, 256);
    w->n_chats = atoi(s_c);
    printf("n chats = %d", w->n_chats);
    mx_get_chats(w, sock, w->n_chats);
    g_signal_connect(G_OBJECT(w->chat->button), "clicked",
                    G_CALLBACK(mx_find_write), w);
}

int mx_check_chat(char *name) {
    int n;
    ssize_t rd;
    char buf[256];
    int sock;

    sock = mx_get_socket(0);
    write(sock, "/getallusers", strlen("/getallusers"));
    rd = read(sock, buf, 256);
    n = atoi(strndup(buf, rd));
    bzero(buf, 256);
    for (int i = 0; i < n; i++) {
        rd = read(sock, buf, 256);
        if(strcmp(name, strndup(buf, rd)) == 0)
            return 0;
        bzero(buf, 256);
    }
    return -1;
}

void mx_find_write(GtkWidget *widget, t_gtk_chat *w) {
    char *name;
    t_chats *tmp;

    tmp = w->chat;
    (void)widget;
    name = (char*) gtk_entry_get_text(GTK_ENTRY(tmp->entry));
    for (int i = 0; i < w->n_chats; i++) {
        if (strcmp(name, tmp->chats[i]) == 0) {
            //open chat
            //return ;
        }
    }
    if (mx_check_chat(name) == 0) {
        mx_create_chat(w, name);
    }
    else { 
        gtk_label_set_text(GTK_LABEL(w->chat->error),
                        "There is no user with this name");
    }
}

void mx_create_chat(t_gtk_chat *w, char *name) {
    int sock;
    t_chat *cht;
    
    mx_hide_last_win(w, w->last_window);
    w->last_window = "/chat";
    cht = (t_chat*) malloc(sizeof(t_chat));
    mx_init_chat(name, cht);
    mx_pack_chat(w, cht);
    g_idle_add ((int (*)(void *))mx_show_widget, cht->box);
    w->cht = cht;
    sock = mx_get_socket(0);
    write(sock, "/addchat", strlen("/addchat"));
    sleep(1);
    write(sock, name, strlen(name));
    sleep(1);
}

void mx_init_chat(char *name, t_chat *c) {
    c->box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    c->vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    c->name = gtk_label_new(name);
    gtk_widget_set_size_request(c->name, 80, 10);
    c->box_scroll = gtk_box_new(0,0);
    c->win_scroll = gtk_scrolled_window_new(0,0);
    gtk_widget_set_size_request(c->win_scroll, 500, 300);
    c->list_box = gtk_list_box_new();
    c->hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    c->entry = gtk_entry_new();
    c->button = gtk_button_new_with_label("Enter");
    gtk_widget_set_size_request(c->button, 20, 20);
}

void mx_pack_chat(t_gtk_chat *w, t_chat *c) {
    gtk_box_pack_start(GTK_BOX(w->box), c->box, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(c->box), c->vbox, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(c->vbox), c->name, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(c->vbox), c->box_scroll, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(c->box_scroll), c->win_scroll, 1, 0, 10);
    gtk_container_add(GTK_CONTAINER(c->win_scroll), c->list_box);
    gtk_box_pack_start(GTK_BOX(c->hbox), c->entry, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(c->hbox), c->button, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(c->vbox), c->hbox, 1, 0, 10);
    g_signal_connect(G_OBJECT(c->button), "clicked",
                    G_CALLBACK(mx_add_msg), w);
}

void mx_add_msg(GtkWidget *widget, t_gtk_chat *w) {
    char *text;
    char *to;
    int sock;
    t_msg *msg;

    (void)widget;
    sock = mx_get_socket(0);
    write(sock, "/write", strlen("/write"));
    sleep(1);
    to = (char*) gtk_label_get_text(GTK_LABEL(w->cht->name));
    write(sock, to, strlen(to));
    sleep(1);
    text = (char*) gtk_entry_get_text(GTK_ENTRY(w->cht->entry));
    gtk_entry_set_text(GTK_ENTRY(w->cht->entry), "");
    write(sock, text, strlen(text));
    sleep(1);
    msg = (t_msg*) malloc(sizeof(t_msg));
    printf("1\n");
    mx_create_msg(w ,w->user->name, text, msg);
    printf("1\n");
    g_idle_add ((int (*)(void *))mx_show_widget, w->cht->box);
    printf("1\n");
    (w->n_sms)++;
}

void mx_create_msg(t_gtk_chat *w, char *name, char *txt, t_msg *msg1) {
    time_t current_time;
    struct tm * time_info;
    char *time_string = (char*) malloc(sizeof(char) * 9);

    time(&current_time);
    time_info = localtime(&current_time);
    strftime(time_string, sizeof(time_string), "%H:%M:%S", time_info);
    printf("1\n");
    msg1->row = gtk_list_box_row_new();
    printf("1\n");
    gtk_list_box_insert((GtkListBox *)w->cht->list_box, msg1->row, w->n_sms);
    printf("1\n");
    msg1->box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    msg1->name = gtk_label_new(name);
    msg1->msg = gtk_label_new(txt);
    msg1->time = gtk_label_new(time_string);
    printf("1\n");
    gtk_container_add(GTK_CONTAINER(msg1->row), msg1->box);
    printf("1\n");
    gtk_box_pack_start(GTK_BOX(msg1->box), msg1->name,
                       FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(msg1->box), msg1->msg,
                       FALSE, FALSE, 0);
                       printf("1\n");
    gtk_box_pack_start(GTK_BOX(msg1->box), msg1->time,
                       FALSE, FALSE, 0);  
    gtk_widget_set_size_request(msg1->row, 100, 100);
}

void mx_hide_last_win(t_gtk_chat *w, char *last) {
    if (strcmp(last, "/profile") == 0)
        gtk_widget_hide(w->prof->box3);
    if (strcmp(last, "/settings") == 0)
        gtk_widget_hide(w->set->box);
    if (strcmp(last, "/server") == 0)
        gtk_widget_hide(w->serv->box);
    if (strcmp(last, "/messages") == 0)
        gtk_widget_hide(w->chat->box);
    if (strcmp(last, "/chat") == 0)
        gtk_widget_hide(w->cht->box);
}
