// TCP - HTTP (HTML 파일 불러오기)

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "WS2_32.lib")

#define MAX 10000

using namespace std;

int main()
{
    WSADATA wsaData;
    SOCKET    SS;
    SOCKET    CS;

    int        nlen;
    struct    sockaddr_in    SA;
    struct    sockaddr_in    CA;

    long    nRet;
    BOOL    bValid = 1;

    char    szInBuf[MAX];

    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
    {
        return 1;
    }

    SS = socket(AF_INET, SOCK_STREAM, 0);

    if (SS == INVALID_SOCKET)
    {
        printf("Socket Error No : %d", WSAGetLastError());
        return 1;
    }

    SA.sin_family = AF_INET;
    SA.sin_port = htons(19934);
    SA.sin_addr.S_un.S_addr = INADDR_ANY;

    setsockopt(SS, SOL_SOCKET, SO_REUSEADDR, (const char*)&bValid, sizeof(bValid));

    if (bind(SS, (struct sockaddr*)&SA, sizeof(SA)) != 0)
    {
        printf("Bind Error No : %d", WSAGetLastError());
        return 1;
    }

    if (listen(SS, 5) != 0)
    {
        printf("Listen Error No : %d", WSAGetLastError());
        return 1;
    }

    while (1)
    {
        nlen = sizeof(CA);
        CS = accept(SS, (struct sockaddr*)&CA, &nlen);

        if (CS == INVALID_SOCKET)
        {
            printf("Accept Error No : %d", WSAGetLastError());
            return 1;
        }

        FILE* fp = fopen("index.html", "rb");

        char Buffer[MAX] = { 0, };
        while (!feof(fp))
        {
            size_t ReadSize = fread(Buffer, sizeof(char), sizeof(Buffer), fp);

            _snprintf(Buffer, sizeof(Buffer),
                "HTTP/1.0 200 OK\r\n"
                //"Content-Length: 311\r\n"
                "Content-Type: text/html\r\n"
                "\r\n");
        };
            
        fclose(fp);

        memset(szInBuf, 0, sizeof(szInBuf));
        recv(CS, szInBuf, sizeof(szInBuf), 0);

        cout << sizeof(szInBuf) << '\n';

        cout << szInBuf << "----------------------------------------------------------------\n";

        send(CS, Buffer, sizeof(Buffer), 0);   

        closesocket(CS);
    }
    WSACleanup();

    return 0;
}