#include "server.h"

void mx_set_user_info_path(t_user_info *user, char *path) {
    strncpy(user->ava_img_path, path, mx_strlen(path));
}

void mx_set_user_info_nick(t_user_info *user, char *nick) {
    strncpy(user->nickname, nick, mx_strlen(nick));
}

t_message *mx_create_message(t_user_info *user, char *msg) {
    t_message *new_message = (t_message*) malloc(sizeof(t_message));
    time_t current_time;
    struct tm * time_info;
    char *time_string = (char*) malloc(sizeof(char) * 9);

    time(&current_time);
    time_info = localtime(&current_time);
    strftime(time_string, sizeof(time_string), "%H:%M:%S", time_info);
    new_message->login = user->login;
    new_message->msg = strdup(msg);
    new_message->time = time_string;
    return new_message;    
}

void mx_free_user_info(t_user_info * user) {
    free(user->login);
    free(user->password);
    free(user->nickname);
    free(user->ava_img_path);
    for (int i = 0; i < 40; i++) {
        free(user->tags[i]);
        free(user->chats[i]);
    }
    free(user);
}

/*Demetreus420 > > >*/
t_check_data *mx_db_create_check_data(char *first, char *second) {
    t_check_data *new = (t_check_data*) malloc(sizeof(t_check_data));
    
    new->strarr2 = malloc(40 * sizeof(char*));
    for (int i = 0; i < 40; i++)
        new->strarr2[i] = mx_strnew(40);
    new->strarr = malloc(40 * sizeof(char*));
    for (int i = 0; i < 40; i++)
        new->strarr[i] = mx_strnew(40);
    new->first = mx_strnew(40);
    new->second = mx_strnew(40);
    new->first_re = false;
    new->second_re = false;
    new->first = mx_strncpy(new->first,
                            first, mx_strlen(first));
    new->second = mx_strncpy(new->second,
                             second, mx_strlen(second));
    return new;
}

void mx_free_check_data(t_check_data *data) {
    for (int i = 0; i < 40; i++) {
        free(data->strarr[i]);
        free(data->strarr2[i]);
    }
    free(data->first);
    free(data->second);
    //mx_free_user_info(data->user);
    free(data);
}

