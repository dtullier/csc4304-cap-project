/*
	Client Side
    Currently exists separately from the other client but will be 
    combined for Milestone 3
*/

//Written by Devin Tullier
//Adapted from https://www.geeksforgeeks.org/socket-programming-cc/

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    const char *request = "Request for alert";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(sock, request, strlen(request), 0);
    int ch = 0;

    int words;
    read(sock, &words, sizeof(int));
    while (ch != words)
    {
        read(sock, buffer, 512);
        printf(" %s", buffer);
        ch++;
    }
    close(sock);

    printf("%s\n", buffer);
    return 0;
}