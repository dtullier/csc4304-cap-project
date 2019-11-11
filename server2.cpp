/*
	Server Side
    Currently exists separately from the other server but will
    be combined in Milestone 3
*/

//Written by Devin Tullier 
//Adapted from https://www.geeksforgeeks.org/socket-programming-cc/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    valread = read(new_socket, buffer, 1024);
    printf("Incoming Request: %s\n", buffer);

    FILE *f;

    int words = 1;
    char c;
    f = fopen("alert.xml", "r");
    while ((c = getc(f)) != EOF)
    {
        fscanf(f, "%s", buffer);
        if (isspace(c) || c == '\t' || c == '\n')
            words++;
    }

    write(new_socket, &words, sizeof(int));
    rewind(f);

    char ch;
    while (ch != EOF)
    {

        fscanf(f, "%s", buffer);
        write(new_socket, buffer, 512);
        ch = fgetc(f);
    }
    printf("The file was sent successfully \n");

    return 0;
}