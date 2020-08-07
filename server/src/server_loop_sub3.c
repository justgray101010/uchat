#include "server.h"

void mx_edit_prof(int sock, char *login) {
    char buf[256];
    ssize_t n;
    char *new_name;

    bzero(buf, 256);
    n = read(sock, buf, 256);
    sleep(1);
    new_name = strndup(buf, n);
    mx_db_set_nickname(login, new_name);
    free(new_name);
}

void mx_delete_node(t_user_info *user) {
    t_list **online;
    t_list *temp;
    t_list *prev;

    online = mx_get_online(NULL);
    temp = *online;
    if (temp != NULL && strcmp(user->nickname, temp->data) == 0) {
            *online = temp->next;
            free(temp);
            return ;
    }
    while (temp != NULL && strcmp(user->nickname, temp->data) != 0) {
        prev = temp; 
        temp = temp->next; 
    }
    if (temp == NULL)
        return ;
    prev->next = temp->next;
    free(temp);
}

void mx_close_connection(int sock, char *login) {
    t_user_info *user;

    if (login != NULL) {
        user = (t_user_info*) malloc(sizeof(t_user_info));
        user = mx_db_get_user_info(login);
        mx_delete_node(user);
    }
    printf("connection closed\n");
    close(sock);
    pthread_exit(NULL);
}

t_user_info *mx_login(int sock) {
    char buf[256];
    t_user_info *user;
    t_check_data *data;
    char *login;
    char *password;
    ssize_t n;

    user = (t_user_info*) malloc(sizeof(t_user_info));
    bzero(buf, 256);
    n = read(sock, buf, 256);
    login = strndup(buf, n);
    bzero(buf, 256);
    n = read(sock, buf, 256);
    password = strndup(buf, n);
    user = mx_create_user_info(login, password);
    free(login);
    free(password);
    data = mx_db_auth(user);
    write(sock, mx_check_data(data), strlen(mx_check_data(data)));
    sleep(1);
    if (strcmp(mx_check_data(data), "accepted") == 0) {
        user = mx_db_get_user_info(user->login);
        write(sock, user->nickname, strlen(user->nickname));
        return user;
    }
    return NULL;
}

char *mx_check_data(t_check_data *data) {
    if (data->first_re == TRUE && data->second_re == TRUE)
        return "accepted";
    if (data->first_re == TRUE && data->second_re == FALSE)
        return "wrong password";
    return "incorrect input";
}

