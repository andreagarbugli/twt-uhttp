#include "common.h"

int g_running = 1;

void handle_sigint(int sig)
{
    printf("Received SIGINT, shutting down\n");
    g_running = 0;
}

int main()
{
    init_platform();

    signal(SIGINT, handle_sigint);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        fprintf(stderr, "socket() failed: %s\n", strerror(errno));
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8088);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        fprintf(stderr, "bind() failed: %s\n", strerror(errno));
        return 1;
    }

    if (listen(sockfd, 1) < 0)
    {
        fprintf(stderr, "listen() failed: %s\n", strerror(errno));
        return 1;
    }

    while (g_running)
    {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (clientfd < 0)
        {
            fprintf(stderr, "accept() failed: %s\n", strerror(errno));
            return 1;
        }

        char buf[1024];
        int len = recv(clientfd, buf, sizeof(buf), 0);
        if (len < 0)
        {
            fprintf(stderr, "recv() failed: %s\n", strerror(errno));
            return 1;
        }

        buf[len] = '\0';
        printf("Received %d bytes\n", len);
        printf("%s\n", buf);

        const char *response = "HTTP/1.1 200 OK\r\n"
                               "Content-Type: text/plain\r\n"
                               "Content-Length: 12\r\n"
                               "\r\n"
                               "Hello world!";
        len = send(clientfd, response, strlen(response), 0);
        if (len < 0)
        {
            fprintf(stderr, "send() failed: %s\n", strerror(errno));
            return 1;
        }

        printf("Sent %d bytes\n", len);

#ifdef _WIN32
        closesocket(clientfd);
#else
        close(clientfd);
#endif
    }

    // Post-connection code
    uninit_platform();

    return 0;
}