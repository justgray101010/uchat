#include "uchat.h"

int mx_get_socket(int s) {
    static int sock;

    if (s == 0)
        return sock;
    else
        sock = s;
    return sock;
}

char *mx_get_ip(const char *input) {
    static char *ip;

    if (input == NULL) {
        return ip;
    }
    else {
        ip = strdup(input);
    }
    return NULL;
}

char *mx_get_port(const char *input) {
    static char *port;

    if (input == NULL) {
        return port;
    }
    else {
        port = strdup(input);
    }
    return NULL;
}

int mx_socket(int domain, int type, int protocol) {
    int res = socket(domain, type, protocol);

    if (res == -1) {
        perror("uchat: socket failed");
    }
    return res;
}

int mx_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int res = connect(sockfd, addr, addrlen);

    if (res == -1) {
        perror("uchat: connect failed");
    }
    return res;
}

void mx_inet_pton(int af, const char *src, void *dst) {
    int res = inet_pton(af, src, dst);
    
    if (res == 0) {
        mx_printstr("uchat: inet_pton failed: src does not contain character...\n");
    }
    if (res == -1) {
        perror("uchat: inet_pton failed");
    }
}

int mx_connect_to_server() {
    int sockfd;
    struct sockaddr_in addr = {0};
    int retval = 0;
    char *port = mx_get_port(NULL);
    char *ip = mx_get_ip(NULL);

    sockfd = mx_socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    mx_inet_pton(AF_INET, ip, &addr.sin_addr);
    retval = mx_connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));
    if (retval == -1)
        return -1;
    return sockfd;
}
