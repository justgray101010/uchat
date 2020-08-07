#include "server.h"

void mx_set_def_tags() {
    mx_db_add_tag("CS:GO");
    mx_db_add_tag("Dota2");
    mx_db_add_tag("World of Tanks");
    mx_db_add_tag("Apex Legends");
    mx_db_add_tag("PUBG");
    mx_db_add_tag("Age of Empires 2 Definitive Editon");
    mx_db_add_tag("Warcraft Refordged");
    mx_db_add_tag("Just Chatting");
    mx_db_add_tag("GTA V");
}

int mx_create_socket(const char *port) {
    int sock = mx_socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = {0};

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(port));
    mx_setsockopt(sock);
    mx_bind(sock, (struct sockaddr *) &server_addr,
            sizeof(server_addr));
    mx_listen(sock, MAX_CONNECTION);
    return sock;
}

int mx_socket(int domain, int type, int protocol) {
    int res = socket(domain, type, protocol);

    if (res == -1) {
        perror("server: socket failed");
        exit(EXIT_FAILURE);
    }
    return res;
}

void mx_setsockopt(int sock) {
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
                   "yes", sizeof(int)) == -1) {
        perror("server: setsockopt failed");
        exit(EXIT_FAILURE);
    }
}

