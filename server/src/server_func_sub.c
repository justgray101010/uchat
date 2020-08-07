#include "server.h"

void mx_bind(int sockfd, const struct sockaddr *addr,
             socklen_t addrlen) {
    int res = bind(sockfd, addr, addrlen);

    if (res == -1) {
        perror("server: bind failed");
        exit(EXIT_FAILURE);
    }
}

void mx_listen(int sockfd, int backlog) {
    int res = listen(sockfd, backlog);

    if (res == -1) {
        perror("server: listen failed");
        exit(EXIT_FAILURE);
    }
}

int mx_accept(int sockfd, struct sockaddr *addr, socklen_t addrlen) {
    int res = accept(sockfd, addr, &addrlen);
    
    if (res == -1) {
        perror("server: accept failed");
        exit(EXIT_FAILURE);
    }
    return res;
}

void *mx_get_client_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in*)sa)->sin_addr);
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int *mx_get_all_sockets() {
    static int sockets[MAX_CONNECTION];

    return sockets;
}

