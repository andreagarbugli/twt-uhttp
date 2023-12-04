#include "common.h"

int main()
{
    init_platform();

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

    printf("Welcome to uHTTP server: a simple HTTP server in C\n");
    printf("Listening on port 8080\n");

    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (clientfd < 0)
        {
            fprintf(stderr, "accept() failed: %s\n", strerror(errno));
            return 1;
        }

        // TIMING
        uint64_t start_us = get_time();

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

        // read the file
        char *base_path = "web";
        char *path = "/index.html";
        char fullname[1024];
        snprintf(fullname, sizeof(fullname), "%s%s", base_path, path);

        FILE *fp = fopen(fullname, "r");
        if (fp == NULL)
        {
            fprintf(stderr, "fopen() failed: %s\n", strerror(errno));
            return 1;
        }

        char filebuf[2048];
        len = fread(filebuf, 1, sizeof(filebuf), fp);
        if (len < 0)
        {
            fprintf(stderr, "fread() failed: %s\n", strerror(errno));
            return 1;
        }

        filebuf[len] = '\0';

        char response[4096];
        snprintf(response, sizeof(response),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: text/html\r\n"
                 "Content-Length: %d\r\n"
                 "\r\n"
                 "%s",
                 len, filebuf);

        len = send(clientfd, response, strlen(response), 0);
        if (len < 0)
        {
            fprintf(stderr, "send() failed: %s\n", strerror(errno));
            return 1;
        }

        // TIMING
        uint64_t end_us = get_time();
        float elapsed_ms = (end_us - start_us) / 1000.0f;
        printf("Request took %.2f ms\n", elapsed_ms);

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