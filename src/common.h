#ifndef COMMON_H
#define COMMON_H

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

#else
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#endif

static inline int init_platform()
{
#ifdef _WIN32
    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (result != 0)
    {
        fprintf(stderr, "WSAStartup failed: %d\n", result);
        exit(1);
    }
#endif // _WIN32
    return 0;
}

static inline int uninit_platform()
{
#ifdef _WIN32
    WSACleanup();
#endif // _WIN32
    return 0;
}

static inline uint64_t get_time()
{
#ifdef _WIN32
    FILETIME ft;
    ULARGE_INTEGER ui;
    GetSystemTimeAsFileTime(&ft);
    ui.LowPart = ft.dwLowDateTime;
    ui.HighPart = ft.dwHighDateTime;
    return (long long)(ui.QuadPart / 10); // Convert from 100-nanoseconds to microseconds
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)(tv.tv_sec * 1000000 + tv.tv_usec);
#endif
}

#endif // COMMON_H