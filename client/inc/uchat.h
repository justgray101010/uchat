#ifndef UCHAT_H
#define UCHAT_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <gtk/gtk.h>
#include "libmx.h"

typedef struct s_client_context {
    const char *login;
    const char *password;
} t_client_context;

typedef struct s_user_info {
    char *login;
    char *name;
    char *img;
    char **tags;
} t_user_info;

//struct for all widgets on login
typedef struct s_widget_login {
    GtkWidget *window;
    GtkWidget *uchatl;//uchat label
    GtkWidget *box_uchatl;
    GtkWidget *box;//all boxes here
    GtkWidget *box_logbut;//box log button
    GtkWidget *box_logerrl;//box error login
    GtkWidget *box_log;//box log
    GtkWidget *login_e;//entry
    GtkWidget *login_l;//label
    GtkWidget *box_pw;//box password
    GtkWidget *password_e;//entry
    GtkWidget *password_l;//label
    GtkWidget *login_button;
    GtkWidget *login_error;
    GtkWidget *reg_button;
    GtkWidget *box_regb;
} t_widget;

typedef struct s_widget_register {
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *title;
    GtkWidget *box_title;
    GtkWidget *login_l;
    GtkWidget *login_e;
    GtkWidget *box_log;
    GtkWidget *password_l;
    GtkWidget *password_e;
    GtkWidget *box_pw;
    GtkWidget *confirm_l;
    GtkWidget *confirm_e;
    GtkWidget *box_conf;
    GtkWidget *error_l;
    GtkWidget *box_err;
    GtkWidget *reg_button;
    GtkWidget *box_regb;
    t_widget *log_copy;
} t_widget2;

typedef struct s_profile {
    GtkWidget *login;
    GtkWidget *name;
    GtkWidget *avatar;
    GtkWidget *tags;
    GtkWidget *box1;
    GtkWidget *box2;
    GtkWidget *box3;
} t_prof;

typedef struct s_settings {
    int dark_mode;
    int feng;
    int frus;
    GtkWidget *box;
    GtkWidget *box1;
    GtkWidget *title1;
    GtkWidget *hbox1;
    GtkWidget *theme;
    GtkWidget *switcher;
    GtkWidget *lang;
    GtkWidget *eng;
    GtkWidget *rus;
    GtkWidget *box2;
    GtkWidget *title2;
    GtkWidget *hbox2;
    GtkWidget *name_l;
    GtkWidget *name_e;
    GtkWidget *hbox3;
    GtkWidget *photo_l;
    GtkWidget *photo_e;
    GtkWidget *tags_b;
    GtkWidget *confirm;
} t_set;

typedef struct s_row {
    GtkWidget *row;
    GtkWidget *box1;
    GtkWidget *ebox;
    GtkWidget *name;
    GtkWidget *status;
} t_row;


typedef struct s_serv {
    GtkWidget *box;
    GtkWidget *label;
    GtkWidget *box_scroll;
    GtkWidget *win_scroll;
    GtkWidget *list_box;
    t_row *row;
} t_serv;

typedef struct s_msg {
    GtkWidget *row;
    GtkWidget *box;
    GtkWidget *name;
    GtkWidget *msg;
    GtkWidget *time;
} t_msg;

typedef struct s_chat {
    char *name_lm;
    char *text_lm;
    char *time_lm;
    GtkWidget *box;
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkWidget *name;
    GtkWidget *box_scroll;
    GtkWidget *win_scroll;
    GtkWidget *list_box;
    GtkWidget *entry;
    GtkWidget *button;
    t_msg *msg;
} t_chat;

typedef struct s_row_c {
    GtkWidget *row;
    GtkWidget *box;
    GtkWidget *vbox;
    GtkWidget *name;
    GtkWidget *msg;
    GtkWidget *hbox;
    GtkWidget *noti;
} t_row_c;


typedef struct s_chats {
    char **chats;
    GtkWidget *box;
    GtkWidget *hbox;
    GtkWidget *label;
    GtkWidget *entry;
    GtkWidget *button;
    GtkWidget *error;
    GtkWidget *box_scroll;
    GtkWidget *win_scroll;
    GtkWidget *list_box;
    t_row_c *row_c;
} t_chats;


typedef struct s_uchat_window {
    int n_sms;
    int n_chats;
    int dark_mode;
    int feng;
    int frus;
    char *last_window;
    t_user_info *user;
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *box_menu;
    GtkWidget *separator;
    GtkWidget *profile;
    GtkWidget *friends;
    GtkWidget *messages;
    GtkWidget *server;
    GtkWidget *connect;
    GtkWidget *settings;
    GtkWidget *log_out;
    t_prof *prof;
    t_set *set;
    t_msg *msg;
    t_serv *serv;
    t_chats *chat;
    t_chat *cht;
} t_gtk_chat;

typedef struct s_message {
    const char *login;
    char msg[256];
    char *time;
} t_message;

int mx_socket(int domain, int type, int protocol);
int mx_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
void mx_inet_pton(int af, const char *src, void *dst);
char *mx_get_ip(const char *input);
char *mx_get_port(const char *input);

void mx_send_msg(int client_sock, t_user_info *user, char *msg);
t_message *mx_create_message(t_user_info *user, char *msg);
t_user_info *mx_create_user_info(char *username);

void mx_login_window(GtkApplication *app);
void mx_init_widget_struct(t_widget *log_w);
void mx_pack_all_widgets(t_widget *all);
void mx_check_login(GtkWidget*widget, t_widget *log_w);
void mx_register(GtkWidget*widget, t_widget *log_w);
void mx_init_widget2_struct(t_widget2 *wid);
void mx_pack_all_widget2_struct(t_widget2 *reg);

void mx_usernew(GtkWidget*widget, t_widget2 *reg);
int mx_connect_to_server();
void mx_uchat_window(t_user_info *user);
void mx_init_gtkchat(t_gtk_chat *w, t_user_info *user);
void mx_pack_chatwidgets(t_gtk_chat *wid);

int mx_get_socket(int s);
void mx_quit(GtkWidget *widget, t_gtk_chat *w);

void mx_signal_buts(t_gtk_chat *w);
int mx_logout(GtkWidget *widget, t_gtk_chat *w);
void mx_show_reg_error(int code, t_widget2 *reg);

void mx_settings(GtkWidget *widget, t_gtk_chat *w);
void mx_init_set(t_set *s);
void mx_pack_set(t_set *w);
void mx_confirm(GtkWidget *widget, t_gtk_chat *w);
void mx_profile(GtkWidget *widget, t_gtk_chat *w);

void mx_server(GtkWidget *widget, t_gtk_chat *w);
void mx_init_serv(t_serv *s, int feng);
void mx_pack_serv(t_gtk_chat *w);
void mx_show_users(t_gtk_chat *w, int count, int sock);
bool mx_check_online(char **on, int n, char *name);
void mx_show_users2(t_gtk_chat *w, char **on, int n, char **off, int m);
void mx_create_row(t_gtk_chat *w, t_row *row, char *name, int i, int flag);
int mx_show_widget(GtkWidget *widget);
void mx_hide_last_win(t_gtk_chat *w, char *last);

void mx_messages(GtkWidget *widget, t_gtk_chat *w);
void mx_get_chats(t_gtk_chat *w, int sock, int count);
void mx_init_chats(t_chats *w);
void mx_pack_chats(t_gtk_chat *w);
void mx_init_row_c(t_chats *w, int count);

void mx_find_write(GtkWidget *widget, t_gtk_chat *w);
int mx_check_chat(char *name);
void mx_create_chat(t_gtk_chat *w, char *name);
void mx_init_chat(char *name, t_chat *c);
void mx_pack_chat(t_gtk_chat *w, t_chat *c);
void mx_add_msg(GtkWidget *widget, t_gtk_chat *w);
void mx_create_msg(t_gtk_chat *w, char *name, char *txt, t_msg *msg1);

#endif
