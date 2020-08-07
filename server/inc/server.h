#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include "libmx.h"
#include "../sql/sqlite3.h"

#define MAX_PTHREAD 32
#define MAX_CONNECTION 32


/*Demetreus420 > > >*/
typedef struct s_user_info{
    char *login;
    char *password;//добавил поле для пароля
    char *nickname;
    char *ava_img_path;
    char **tags;
    char **chats;//дописать функции записи и вывода!
}t_user_info;
/*Demetreus420 < < <*/

typedef struct s_message{
    const char *login;
    char *msg;
    char *time;
}t_message;


/*Demetreus420 > > >*/
typedef struct s_check_data{ //структура
    char *first; //для извлечения данных из колбэков при работе с бд
    char *second;
    bool first_re;
    bool second_re;
    t_user_info *user;
    char **strarr;
    char **strarr2;
    int count;
}t_check_data;
/*Demetreus420 < < <*/

int mx_get_client_num(void);
int mx_create_socket(const char *port);
int mx_socket(int domain, int type, int protocol);
void mx_setsockopt(int sock);
void mx_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
void mx_listen(int sockfd, int backlog);
int mx_accept(int sockfd, struct sockaddr *addr, socklen_t addrlen);
void *mx_get_client_addr(struct sockaddr *sa);

t_list **mx_get_online(char *name);
void *mx_client_pthread(void *client_data);
t_user_info *mx_create_user_info(char *username, char *password);
pthread_mutex_t *mx_get_mutex(void);
void mx_kill_mutex(pthread_mutex_t *mutex);
int *mx_get_all_sockets(void);
void mx_sociable_loop(int client_sock, t_user_info *user);
void mx_send_msg(int client_sock, t_message *message);
t_message *mx_create_message(t_user_info *user, char *msg);
void mx_free_user_info(t_user_info * user);

void mx_request(t_message *req, int sock);
void mx_close_connection(int sock, char *login);
t_user_info *mx_login(int sock);
char *mx_check_data(t_check_data *data);
t_user_info *mx_register(int sock);
void mx_set_def_tags(void);
void mx_edit_prof(int sock, char *login);
void mx_profile(int sock, const char *login);
void mx_server(int sock);
void mx_delete_node(t_user_info *user);
void mx_get_all_users(int sock);

/*Demetreus420 > > >*/
void mx_free_check_data(t_check_data *data);
void mx_db_set_user_info(t_user_info *user); //function to get login
void mx_db_set_tables(void);// function to set up database before using it
t_user_info *mx_db_get_user_info(char *nick);
t_check_data *mx_db_auth(t_user_info *user);
t_check_data *mx_db_create_check_data(char *first, char *second);
void mx_db_set_nickname(char *login, char *new_name);//ok
void mx_set_user_info_path(t_user_info *user, char *path);
void mx_set_user_info_nick(t_user_info *user, char *nick);
//tag
//
void mx_db_set_user_tag_table(char *login);
void mx_db_set_user_tags(char *login, char **tags); //ok
char **mx_db_get_user_tags(char *login);//ok
void mx_db_set_tag_table(void);// ok
char **mx_db_get_all_tags(void);//ok
void mx_db_add_tag(const char* tag);//ok
//message table
//
void mx_db_set_chat(char *chat_name);//ok
void mx_db_add_msg(char* chat_name,char *name, char *msg);//ok
t_check_data *mx_db_get_chat(char *chat_name);//ok
//friend table
//
void mx_db_set_friends_table(char *login); //ok
void mx_db_add_friend(char *login, char *amigo);//ok
t_check_data *mx_db_get_friends_arr(char *login);//ok
void mx_db_del_friend(char *login, char *amigo);//ok
/*Demetreus420 < < <*/

void mx_db_set_user_chats(char *login);
void mx_db_add_user_chat(char *login, char *chat_name);
void mx_db_get_user_chats(t_user_info *user);

t_check_data *mx_db_get_all_users(void);
void mx_messages(int sock, char *login);
void mx_addchat(int sock, char *login);

#endif
