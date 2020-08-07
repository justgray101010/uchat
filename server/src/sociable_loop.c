#include "server.h"

void mx_request(t_message *req, int sock) {
    printf("request = %s\n", req->msg);
    if (strcmp(req->msg, "/logout") == 0 
            || strcmp(req->msg, "/quit") == 0)
        mx_close_connection(sock, (char*)req->login);
    if (strcmp(req->msg, "/login") == 0)
        mx_login(sock);
    if (strcmp(req->msg, "/register") == 0)
        mx_register(sock);
    if (strcmp(req->msg, "/edit_prof") == 0)
        mx_edit_prof(sock, (char*)req->login);
    if (strcmp(req->msg, "/profile") == 0)
        mx_profile(sock, req->login);
    if (strcmp(req->msg, "/server") == 0)
        mx_server(sock);
    if (strcmp(req->msg, "/messages") == 0) {
        mx_messages(sock, (char*)req->login);
    }
    if (strcmp(req->msg, "/getallusers") == 0) {
        mx_get_all_users(sock);
    }
    if (strcmp(req->msg, "/addchat") == 0) {
        mx_addchat(sock, (char*)req->login);
    }
    // if (strcmp(req->msg, "/write") == 0) {
    //     mx_write(sock, (char*)req->login);
    // }
    // if (strcmp(req->msg, "/newchat") == 0) {
    //     mx_newchat(sock, (char*)req->login);
    // }
}

void mx_addchat(int sock, char *login) {
    ssize_t n;
    char buf[256];
    char *new_chat;

    n = read(sock, buf, 256);
    mx_db_set_user_chats(login);
    new_chat = strndup(buf, n);
    mx_db_add_user_chat(login, new_chat);
    free(new_chat);
}

void mx_messages(int sock, char *login) {
    t_user_info *user;
    int n_chats = 0;
    char *s_c;

    user = mx_db_get_user_info(login);
    mx_db_get_user_chats(user);
    while (strcmp(user->chats[n_chats++], "") != 0);
    s_c = mx_itoa(n_chats - 1);
    write(sock, s_c, strlen(s_c));
    printf("n chats = %s\n", s_c);
    for (int i = 0; i < n_chats; i++) {
        write(sock, user->chats[i], strlen(user->chats[i]));
        sleep(1);
    }
}

// void mx_write(int sock, char *login) {

// }

// static int arr_size(char **arr) {
//     int result = 0;

//     while (*arr) {
//         if (strlen(*arr) > 0)
//             result++;
//         arr++;
        
//     }
//     return result;
// }

void mx_get_all_users(int sock) {
    // t_check_data *data;
    // char *lenn;
    // char *name;

    write(sock, "2", strlen("2"));
    printf("all users = %s\n", "2");
    sleep(1);
    write(sock, "sss", strlen("sss"));
    sleep(1);
    write(sock, "aaa", strlen("aaa"));
    sleep(1);

    // printf("Here we go again\n");
    // data = mx_db_get_all_users();
    // printf("Here we go again2 = %d\n", arr_size(data->strarr));
    // lenn = mx_itoa(arr_size(data->strarr));
    // write(sock, lenn, strlen(lenn));
    // printf("all users = %s\n", lenn);
    // sleep(1);
    // for (int i = 0; i < data->count; i++) {
    //     name = data->strarr[i];
    //     write(sock, name, strlen(name));
    //     sleep(1);
    // }
    // free(lenn);
    // mx_free_check_data(data);
}

void mx_server(int sock) {
    t_list **online;
    t_list *temp;
    int len;
    char *lenn;
    char *name;

    online = mx_get_online(NULL);
    len = mx_list_size(*online);
    lenn = mx_itoa(len);
    write(sock, lenn, strlen(lenn));
    printf("online = %s\n", lenn);
    sleep(1);
    temp = *online;
    for (int i = 0; i < len; i++) {
        name = temp->data;
        write(sock, name, strlen(name));
        temp = temp->next;
        sleep(1);
    }
    mx_get_all_users(sock);
    free(lenn);
}

void mx_profile(int sock, const char *login) {
    t_user_info *user;

    user = (t_user_info*) malloc(sizeof(t_user_info));
    user = mx_db_get_user_info((char*)login);
    write(sock, user->nickname, strlen(user->nickname));
    //free(user);
}

