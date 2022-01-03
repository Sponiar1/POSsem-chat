#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int sockfd, n, logged;
    struct sockaddr_in serv_addr;
    struct hostent* server;

    char buffer[256];

    if (argc < 3)
    {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        return 1;
    }

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "Error, no such host\n");
        return 2;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy(
            (char*)server->h_addr,
            (char*)&serv_addr.sin_addr.s_addr,
            server->h_length
    );
    serv_addr.sin_port = htons(atoi(argv[2]));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 3;
    }

    if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error connecting to socket");
        return 4;
    }



    while (logged == 0) {
        printf("Vitajte zvolte akciu: \n");
        printf("Prihlasit sa - 1\n");
        printf("Registrovat sa - 2\n");
        fgets(buffer, 255, stdin);
        n = write(sockfd, buffer, strlen(buffer));

        if (strncmp("1", buffer, 1) == 0) {
            bzero(buffer, 256);
            n = read(sockfd, buffer, 255);
            printf("%s", buffer);
            fgets(buffer, 255, stdin);
            n = write(sockfd, buffer, strlen(buffer));

            bzero(buffer, 256);
            n = read(sockfd, buffer, 255);
            printf("%s", buffer);
            fgets(buffer, 255, stdin);
            n = write(sockfd, buffer, strlen(buffer));
            n = read(sockfd, buffer, 255);
        }
        if (strncmp("2", buffer, 1) == 0) {
            bzero(buffer, 256);
            n = read(sockfd, buffer, 255);
            printf("%s", buffer);
            fgets(buffer, 255, stdin);
            n = write(sockfd, buffer, strlen(buffer));

            bzero(buffer, 256);
            n = read(sockfd, buffer, 255);
            printf("%s", buffer);
            fgets(buffer, 255, stdin);
            n = write(sockfd, buffer, strlen(buffer));
            n = read(sockfd, buffer, 255);
        }
        printf("%s", buffer);
        if(strncmp("V", buffer, 1) == 0) {
            logged = 1;
        }
    }

    while (1==1) {
        printf("Please enter a message: ");
        bzero(buffer, 256);
        fgets(buffer, 255, stdin);

        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) {
            perror("Error writing to socket");
            return 5;
        }

        if(strncmp("end", buffer, 3) == 0) {
            break;
        }

        bzero(buffer, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0) {
            perror("Error reading from socket");
            return 6;
        }

        printf("%s\n", buffer);

    }
    close(sockfd);

    return 0;
}