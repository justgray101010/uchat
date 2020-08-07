#include "uchat.h"

bool mx_check_online(char **on, int n, char *name) {
    for (int i = 0; i < n; i++) {
        if (strcmp(on[i], name) == 0) {
            return FALSE;
        }
    }
    return TRUE;
}

void mx_create_row(t_gtk_chat *w, t_row *row, char *name, int i, int flag) {
    row->row = gtk_list_box_row_new();
    gtk_list_box_insert((GtkListBox *)w->serv->list_box, row->row, i);
    row->box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    row->ebox = gtk_event_box_new ();
    row->name = gtk_label_new(name);
    if (flag == 1)
        row->status = gtk_label_new("online");
    else
        row->status = gtk_label_new("offline");
    gtk_container_add(GTK_CONTAINER(row->row), row->box1);
    gtk_container_add (GTK_CONTAINER (row->ebox), row->name);
    gtk_box_pack_start(GTK_BOX(row->box1), row->ebox,
                       FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row->box1), row->status,
                       FALSE, FALSE, 0);
    gtk_widget_set_size_request(row->row, 100, 20);
    //gtk_widget_set_events (row->ebox, GDK_BUTTON_PRESS_MASK);
    // g_signal_connect(G_OBJECT(row->ebox), "button_press_event",
    //                     G_CALLBACK(mx_profile), w);
    // gtk_widget_realize (row->ebox);
}

int mx_show_widget(GtkWidget *widget) {
    gtk_widget_show_all(widget);
    return 0;
}

void mx_show_users2(t_gtk_chat *w, char **on, int n, char **off, int m) {
    for (int i = 0; i < n; i++) {
        t_row *row = (t_row*) malloc(sizeof(t_row));

        mx_create_row(w, row, on[i], i, 1);   
    }
    if (m != 0) {
        for (int i = 0; i < m; i++) {
            t_row *row = (t_row*) malloc(sizeof(t_row));

            mx_create_row(w, row, off[i], n + i, 0);
        }
    }
    g_idle_add ((int (*)(void *))mx_show_widget, w->serv->box);
}

void mx_show_users(t_gtk_chat *w, int count, int sock) {
    char buf[256];
    ssize_t n;
    char **online;
    char **offline;
    int nn;
    char *len;
    int j = 0;
    
    online = (char**) malloc(sizeof(char*) * (count + 1));
    for (int i = 0; i < count; i++) {
        n = read(sock, buf, 256);
        online[i] = strndup(buf, n);
        bzero(buf, 256);
    }
    n = read(sock, buf, 256);
    len = strndup(buf, n);
    nn = atoi(len);
    free(len);
    offline = (char**) malloc(sizeof(char*) * (nn - count + 1));
    if (nn - count > 0) {
        for (int i = 0; i < nn; i++) {
            char *temp;

            n = read(sock, buf, 256);
            temp = strndup(buf, n);
            bzero(buf, 256);
            if (mx_check_online(online, count, temp)) {
                offline[j] = strdup(temp);
                j++;
            }
        }
    }
    online[count] = NULL;
    offline[j] = NULL;
    bzero(buf, 256);
    mx_show_users2(w, online, count, offline, j);
}

void mx_server(GtkWidget *widget, t_gtk_chat *w) {
    int sock;
    char buf[256];
    ssize_t n;
    int count;
    char *len;

    (void)widget;
    mx_hide_last_win(w, w->last_window);
    w->last_window = "/server";
    sock = mx_get_socket(0);
    mx_init_serv(w->serv, 0);
    mx_pack_serv(w);
    write(sock, "/server", strlen("/server"));
    n = read(sock, buf, 256);
    len = strndup(buf, n);
    bzero(buf, 256);
    count = atoi(len);
    printf("online = %s\n", len);
    mx_show_users(w, count, sock);
    free(len);
}

void mx_init_serv(t_serv *s, int feng) {
    s->box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    if (feng == 0)
        s->label = gtk_label_new("Users:");
    else
        s->label = gtk_label_new("Пользователи:");
        gtk_widget_set_size_request(s->label, 80, 20);
    s->box_scroll = gtk_box_new(0,0);
    s->win_scroll = gtk_scrolled_window_new(0,0);
    gtk_widget_set_size_request(s->win_scroll, 600, 400);
    s->list_box = gtk_list_box_new();
}

void mx_pack_serv(t_gtk_chat *w) {
    gtk_box_pack_start(GTK_BOX(w->box), w->serv->box, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->serv->box), w->serv->label, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->serv->box), w->serv->box_scroll, 1, 0, 10);
    gtk_box_pack_start(GTK_BOX(w->serv->box_scroll), w->serv->win_scroll,
                        1, 0, 10);
    gtk_container_add(GTK_CONTAINER(w->serv->win_scroll), w->serv->list_box);
}
