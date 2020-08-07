#include "server.h"

t_user_info *mx_register(int sock) {
    char buf[256];
    t_user_info *user;
    ssize_t n;
    char *login;
    char *password;

    bzero(buf, 256);
    n = read(sock, buf, 256);
    login = strndup(buf, n);
    bzero(buf, 256);
    n = read(sock, buf, 256);
    password = strndup(buf, n);
    user = mx_create_user_info(login, password);
    user->nickname = strdup(user->login);
    free(login);
    free(password);
    mx_db_set_user_info(user);
    return user;
}

void mx_sociable_loop(int client_sock, t_user_info *user) {
    pthread_mutex_t *mutex = mx_get_mutex();
    ssize_t n;
    int count = 0;

    while (count < 10) {
        char buf[256];

        pthread_mutex_lock(mutex);
        n = read(client_sock, buf, 256);
        t_message *new_message = mx_create_message(user, strndup(buf,n));
        if (strcmp("", new_message->msg) == 0) 
            count++;
        mx_request(new_message, client_sock);
        free(new_message);
        bzero(buf, 256);
        pthread_mutex_unlock(mutex);
    }
    mx_kill_mutex(mutex);
    mx_close_connection(client_sock, user->login);
}

pthread_mutex_t *mx_get_mutex() {
    static pthread_mutex_t mutex;
    
    pthread_mutex_init(&mutex, NULL);
    return &mutex;
}

void mx_kill_mutex(pthread_mutex_t *mutex) {
    pthread_mutex_destroy(mutex);
}

t_user_info *mx_create_user_info(char *username, char *password) {
    t_user_info *new_user = (t_user_info*) malloc(sizeof(t_user_info));
    
    new_user->chats = malloc(40 * sizeof(char*));
    for (int i = 0; i < 40; i++)
        new_user->chats[i] = mx_strnew(40);
    new_user->tags = malloc(40 * sizeof(char*));
    for (int i = 0; i < 40; i++)
        new_user->tags[i] = mx_strnew(40);
    new_user->nickname = mx_strnew(20);
    new_user->ava_img_path = mx_strnew(120);
    new_user->password = mx_strnew(mx_strlen(password));
    new_user->login = mx_strnew(mx_strlen(username));
    strncpy(new_user->login, username, mx_strlen(username));
    strncpy(new_user->password, password, mx_strlen(password));
    return new_user;
}


