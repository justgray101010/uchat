#include "server.h"

int mx_get_client_num() {
    static int client_num = 0;

    return client_num;
}

t_list **mx_get_online(char *name) {
    static t_list *list;

    if (!name) {
        return &list;
    }
    if (list == NULL) {
        list = mx_create_node(name);
    }
    else {
        mx_push_back(&list, name);
    }
    return NULL;
}

void *mx_client_pthread(void *client_data) {
    char buf[256];
    int *client_d = client_data;
    t_user_info *user;

    bzero(buf, 256);
    read(*client_d, buf, 256);
    if (strcmp(buf, "/register") == 0) {
        user = mx_register(*client_d);
    }
    if (strcmp(buf, "/login") == 0) {
        user = mx_login(*client_d);
    }
    if (user == NULL) {
        mx_close_connection(*client_d, NULL);
        pthread_exit(NULL);
    }
    mx_get_online(user->nickname);
    mx_sociable_loop(*client_d, user);
    close(*client_d);
    pthread_exit(NULL);
}

void mx_main_server(char *port) {
    mx_db_set_tables();
    mx_db_set_tag_table();
    mx_set_def_tags();
    int client_count = mx_get_client_num();
    int sock = mx_create_socket(port);
    struct sockaddr_storage client_addr;
    int *client_d = mx_get_all_sockets();
    pthread_t pthr[MAX_PTHREAD];
    char ip[INET6_ADDRSTRLEN];

    if (sock < 0) {
        fprintf(stderr, "server: failed create socket\n");
    }
    printf("Server created!\n");
    while(1) {
        client_d[client_count] = mx_accept(sock, (struct sockaddr*) &client_addr, 
                            sizeof(client_addr));
        inet_ntop(client_addr.ss_family, 
                    mx_get_client_addr((struct sockaddr*) &client_addr),
                    ip, sizeof(client_addr));
        printf("server: got %d connection from %s\n", client_count + 1, ip);
        if (pthread_create(&pthr[client_count], NULL, mx_client_pthread,
                            &client_d[client_count]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
        if (pthread_detach(pthr[client_count++]) != 0) {
            perror("pthread_detach");
            exit(EXIT_FAILURE);
        }
    }
    close(sock);
}

int main(int argc, char *argv[]) {
    int port;
    
    if (argc != 2) {
        printf("Error: usage: ./uchat_server PORT\n");
        exit(1);
    }
    port = atoi(argv[1]);
    if (port <= 0) {
        printf("Error: incorrect PORT input\n");
        exit(1);
    }
    mx_main_server(argv[1]);
    return 0;
}

