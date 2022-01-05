#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
//zmena
int login(char meno[], char heslo[]) {
    int found;
    found = 0;
    FILE *f;
    f = fopen("/tmp/PosSemTest/login.txt", "r");
    char fileMeno[50];
    char fileHeslo[50];
    bzero(fileMeno, 50);
    bzero(fileHeslo, 50);
    meno[strcspn(meno, "\n")] = 0;
    heslo[strcspn(heslo, "\n")] = 0;
    while(!feof(f)) {
        fscanf(f, "%s", fileMeno);
        printf("%s \n", fileMeno);
        fscanf(f, "%s", fileHeslo);
        printf("%s \n", fileHeslo);
        if (strncmp(fileMeno, meno, strlen(meno)) == 0 && (strncmp(fileHeslo, heslo, strlen(heslo))) == 0) {
            found = 1;
            printf("Našiel som");
            break;
        }
    }

    fclose(f);
    return found;
}

int checkName(char meno[]) {
    int found;
    found = 0;
    FILE *f;
    f = fopen("/tmp/PosSemTest/login.txt", "r");
    char fileMeno[50];
    char fileHeslo[50];
    bzero(fileMeno, 50);
    bzero(fileHeslo, 50);
    meno[strcspn(meno, "\n")] = 0;
    while(!feof(f)) {
        fscanf(f, "%s", fileMeno);
        printf("%s \n", fileMeno);
        fscanf(f, "%s", fileHeslo);
        printf("%s \n", fileHeslo);
        if (strncmp(fileMeno, meno, strlen(meno)) == 0) {
            found = 1;
            printf("Našiel som");
            break;
        }
    }
    fclose(f);
    return found;
}

void addUser(char meno[], char heslo[]) {
    FILE *f;
    f = fopen("/tmp/PosSemTest/login.txt", "a");
    fprintf(f, "%s %s", meno, heslo);
    fclose(f);

    f = fopen("/tmp/PosSemTest/contacts.txt", "a");
    fprintf(f, "\n");
    fprintf(f, "%s X", meno);
    fclose(f);

}

void removeUser(char meno[]) {
    FILE *f, *n;
    f = fopen("/tmp/PosSemTest/login.txt", "r");
    n = fopen("/tmp/PosSemTest/loginnew.txt", "w+");
    char fileMeno[50];
    char fileHeslo[50];
    meno[strcspn(meno, "\n")] = 0;
    while(!feof(f)) {
        bzero(fileMeno, 50);
        bzero(fileHeslo, 50);
        fscanf(f, "%s", fileMeno);
        if (strncmp(fileMeno, "0", 1) == 0) {
            break;
        }
        if (strncmp(fileMeno, meno, strlen(meno)) != 0) {
            //printf("Našiel som");
            fscanf(f, "%s", fileHeslo);
            fileHeslo[strcspn(fileHeslo, "\n")] = 0;
            fprintf(n, "%s %s", fileMeno, fileHeslo);
            fprintf(n,"\n");
            printf("%s %s\n", fileMeno, fileHeslo);
        } else {
            fscanf(f, "%s", fileHeslo);
        }
    }
    fclose(f);
    fclose(n);
    remove("/tmp/PosSemTest/login.txt");
    rename("/tmp/PosSemTest/loginnew.txt", "/tmp/PosSemTest/login.txt");

    char kontakty[1024];
    bzero(kontakty, 1024);
    f = fopen("/tmp/PosSemTest/contacts.txt", "r");
    n = fopen("/tmp/PosSemTest/contactsnew.txt", "w+");
    while(!feof(f)) {
        bzero(fileMeno, 50);
        fscanf(f, "%s", fileMeno);
        if (strncmp(fileMeno, "0", 1) == 0) {
            break;
        }
        if (strncmp(fileMeno, meno, strlen(meno)) != 0) {
            //printf("Našiel som");
            fgets(kontakty, 1024, f);
            fprintf(n, "%s %s", fileMeno, kontakty);
            fprintf(n,"\n");
            printf("%s %s\n", fileMeno, kontakty);
        } else {
            fgets(kontakty, 1024, f);
        }
    }
    fclose(f);
    fclose(n);
    remove("/tmp/PosSemTest/contacts.txt");
    rename("/tmp/PosSemTest/contactsnew.txt", "/tmp/PosSemTest/contacts.txt");
}

void getContacts(char meno[], int newsockfd) {
    FILE *f;
    int n;
    f = fopen("/tmp/PosSemTest/contacts.txt", "r");
    char fileMeno[50];
    bzero(fileMeno, 50);
    char trash[1024];
    meno[strcspn(meno, "\n")] = 0;
    while(!feof(f)) {
        fscanf(f, "%s", fileMeno);
        if (strncmp(fileMeno, meno, strlen(meno)) == 0) {
            printf("Našiel som");
            while (1==1) {
                bzero(fileMeno, 50);
                fscanf(f, "%s", fileMeno);
                if (strncmp(fileMeno, "X", 1) != 0) {
                    if (strcspn(fileMeno, "#") == ((int)strlen(fileMeno)-1)) {
                        fileMeno[strcspn(fileMeno, "#")] = 0;
                        strcat(fileMeno, " je nepotvrdeny");
                    }
                    n = write(newsockfd, fileMeno, strlen(fileMeno) + 1);
                    usleep(5);
                } else {
                    strcpy(fileMeno, "Done");
                    n = write(newsockfd, fileMeno, strlen(fileMeno) + 1);
                    break;
                }
            }
            break;
        }
        fgets(trash, 1024, f);
    }
    fclose(f);
}

int findContact(char meno[], char kontaktovany[]) {
    FILE *f;
    int found = 2;
    f = fopen("/tmp/PosSemTest/contacts.txt", "r");
    char fileMeno[50];
    bzero(fileMeno, 50);
    char trash[1024];
    meno[strcspn(meno, "\n")] = 0;
    kontaktovany[strcspn(kontaktovany, "\n")] = 0;
    while(!feof(f)) {
        fscanf(f, "%s", fileMeno);
        if (strncmp(fileMeno, meno, strlen(meno)) == 0) {
            printf("Našiel som usera");
            while (1==1) {
                bzero(fileMeno, 50);
                fscanf(f, "%s", fileMeno);
                if (strncmp(fileMeno, kontaktovany, 50) == 0) {
                    found = 1;
                    break;
                } else if (strncmp(fileMeno, "X", 1) == 0){
                    found = 0;
                    break;
                }
            }
            break;
        }
        fgets(trash, 1024, f);
    }
    fclose(f);
    return found;
}

void sendFriendRequest(char meno[], char kontaktovany[]) {
    FILE *f, *n;
    char kontakty[1024];
    bzero(kontakty, 1024);
    char fileMeno[50];
    bzero(fileMeno, 50);
    char trash[1024];
    meno[strcspn(meno, "\n")] = 0;
    kontaktovany[strcspn(kontaktovany, "\n")] = 0;

    f = fopen("/tmp/PosSemTest/contacts.txt", "r");
    n = fopen("/tmp/PosSemTest/contactsnew.txt", "w+");
    while(!feof(f)) {
        bzero(fileMeno, 50);
        bzero(kontakty, 1024);
        fscanf(f, "%s", fileMeno);
        if (strncmp(fileMeno, "0", 1) == 0) {
            break;
        }
        if (strncmp(fileMeno, kontaktovany, strlen(meno)) == 0) {
            printf("Našiel som usera");
                strcat(meno, "#");
                fgets(kontakty, 1024, f);
                fprintf(n, "%s %s %s", fileMeno, meno, kontakty);
        } else {
            fgets(kontakty, 1024, f);
            fprintf(n, "%s %s", fileMeno, kontakty);
        }
    }
    fclose(f);
    fclose(n);
    remove("/tmp/PosSemTest/contacts.txt");
    rename("/tmp/PosSemTest/contactsnew.txt", "/tmp/PosSemTest/contacts.txt");
}

void getFriendRequests(char meno[], int newsockfd) {
    FILE *f;
    int n;
    f = fopen("/tmp/PosSemTest/contacts.txt", "r");
    char fileMeno[50];
    bzero(fileMeno, 50);
    char trash[1024];
    meno[strcspn(meno, "\n")] = 0;
    while(!feof(f)) {
        fscanf(f, "%s", fileMeno);
        if (strncmp(fileMeno, meno, strlen(meno)) == 0) {
            while (1==1) {
                bzero(fileMeno, 50);
                fscanf(f, "%s", fileMeno);
                if (strncmp(fileMeno, "X", 1) != 0) {
                    if (strcspn(fileMeno, "#") == ((int)strlen(fileMeno)-1)) {
                        fileMeno[strcspn(fileMeno, "#")] = 0;
                        n = write(newsockfd, fileMeno, strlen(fileMeno) + 1);
                        usleep(5);
                    }
                } else {
                    strcpy(fileMeno, "Done");
                    n = write(newsockfd, fileMeno, strlen(fileMeno) + 1);
                    break;
                }
            }
            break;
        }
        fgets(trash, 1024, f);
    }
    fclose(f);
}

void confirmFriendRequest(char meno[], char kontaktovany[]) {
    FILE *f, *n;
    char kontakty[1024];
    char kontaktProfile[50];
    bzero(kontakty, 1024);
    char fileMeno[50];
    bzero(fileMeno, 50);
    meno[strcspn(meno, "\n")] = 0;
    kontaktovany[strcspn(kontaktovany, "\n")] = 0;
    strcpy(kontaktProfile, kontaktovany);
    strcat(kontaktovany, "#");
    f = fopen("/tmp/PosSemTest/contacts.txt", "r");
    n = fopen("/tmp/PosSemTest/contactsnew.txt", "w+");
    while(!feof(f)) {
        bzero(fileMeno, 50);
        bzero(kontakty, 1024);
        fscanf(f, "%s", fileMeno);
        fprintf(n,"%s ", fileMeno);
        if (strncmp(fileMeno, "0", 1) == 0) {
            break;
        }
        if (strncmp(fileMeno, meno, strlen(meno)) == 0) {
            printf("Našiel som usera");
            while (1 == 1) {
                fscanf(f, "%s", fileMeno);
                if (strncmp(fileMeno, "X", 1) != 0) {
                    if (strcspn(fileMeno, "#") == ((int) strlen(fileMeno) - 1)) {
                        if (strncmp(fileMeno, kontaktovany, strlen(kontaktovany)) == 0) {
                            fileMeno[strcspn(fileMeno, "#")] = 0;
                        }
                    }
                    fprintf(n, "%s ", fileMeno);
                } else {
                    fprintf(n, "%s ", fileMeno);
                    fprintf(n, "\n");
                    break;
                }
            }
        } else if(strncmp(fileMeno, kontaktProfile, strlen(kontaktProfile))==0) {
            fprintf(n, "%s", meno);
            fgets(kontakty, 1024, f);
            fprintf(n, "%s", kontakty);
        } else {
            fgets(kontakty, 1024, f);
            fprintf(n, "%s ", kontakty);
            fprintf(n, "\n");
        }
    }
    fclose(f);
    fclose(n);
    remove("/tmp/PosSemTest/contacts.txt");
    rename("/tmp/PosSemTest/contactsnew.txt", "/tmp/PosSemTest/contacts.txt");
}

void removeFriend(char meno[], char kontaktovany[]) {
    FILE *f, *n;
    char kontakty[1024];
    bzero(kontakty, 1024);
    char fileMeno[50];
    bzero(fileMeno, 50);
    meno[strcspn(meno, "\n")] = 0;
    kontaktovany[strcspn(kontaktovany, "\n")] = 0;
    f = fopen("/tmp/PosSemTest/contacts.txt", "r");
    n = fopen("/tmp/PosSemTest/contactsnew.txt", "w+");
    while(!feof(f)) {
        bzero(fileMeno, 50);
        bzero(kontakty, 1024);
        fscanf(f, "%s", fileMeno);
        fprintf(n,"%s ", fileMeno);
        if (strncmp(fileMeno, "0", 1) == 0) {
            break;
        }
        if (strncmp(fileMeno, meno, strlen(meno)) == 0) {
            printf("Našiel som usera");
            while (1 == 1) {
                fscanf(f, "%s", fileMeno);
                if (strncmp(fileMeno, "X", 1) != 0) {
                        if (strncmp(fileMeno, kontaktovany, strlen(kontaktovany)) != 0) {
                            fprintf(n, "%s ", fileMeno);
                        }
                } else {
                    fprintf(n, "%s ", fileMeno);
                    fprintf(n, "\n");
                    break;
                }
            }
        } else if(strncmp(fileMeno, kontaktovany, strlen(kontaktovany))==0) {
            while (1 == 1) {
                fscanf(f, "%s", fileMeno);
                if (strncmp(fileMeno, "X", 1) != 0) {
                    if (strncmp(fileMeno, meno, strlen(meno) != 0)) {
                        fprintf(n, "%s ", fileMeno);
                    }
                } else {
                    fprintf(n, "%s ", fileMeno);
                    fprintf(n, "\n");
                    break;
                }
            }
        } else {
            fgets(kontakty, 1024, f);
            fprintf(n, "%s ", kontakty);
            fprintf(n, "\n");
        }
    }
    fclose(f);
    fclose(n);
    remove("/tmp/PosSemTest/contacts.txt");
    rename("/tmp/PosSemTest/contactsnew.txt", "/tmp/PosSemTest/contacts.txt");
}

void *userInteraction(int newsockfd) {
    int n;
    char buffer[256];
    char vyzva[50];
    char search[50];
    bzero(buffer, 256);
    int logged = 0;
    int action = 0;
    char username[50];
    int on = 1;
    while(on==1) {
        //login/register
        while (logged == 0) {
            n = read(newsockfd, buffer, 255);
            if (strncmp("1", buffer, 1) == 0) {
                bzero(vyzva, 50);
                strcpy(vyzva, "Meno: ");
                n = write(newsockfd, vyzva, strlen(vyzva) + 1);
                n = read(newsockfd, buffer, 255);
                char meno[50];
                bzero(meno, 50);
                strcpy(meno, buffer);
                strcpy(username, buffer);
                printf("%s \n", meno);
                bzero(buffer, 256);
                bzero(vyzva, 50);
                strcpy(vyzva, "Heslo: ");
                n = write(newsockfd, vyzva, strlen(vyzva) + 1);
                n = read(newsockfd, buffer, 255);
                char heslo[50];
                bzero(heslo, 50);
                strcpy(heslo, buffer);
                printf("%s \n", heslo);
                logged = login(meno, heslo);
                if (logged == 0) {
                    strcpy(vyzva, "Uzivatel nenajdeny: ");
                    n = write(newsockfd, vyzva, strlen(vyzva) + 1);
                } else {
                    strcpy(vyzva, "Vitaj ");
                    strcat(vyzva, meno);
                    n = write(newsockfd, vyzva, strlen(vyzva) + 1);
                }
            } else if (strncmp("2", buffer, 1) == 0) {
                bzero(vyzva, 50);
                strcpy(vyzva, "Zadajte meno: ");
                n = write(newsockfd, vyzva, strlen(vyzva) + 1);
                n = read(newsockfd, buffer, 255);
                char meno[50];
                bzero(meno, 50);
                strcpy(meno, buffer);
                strcpy(username, buffer);
                printf("%s \n", meno);
                int a = 10;
                bzero(buffer, 256);
                bzero(vyzva, 50);
                strcpy(vyzva, "Zadajte heslo: ");
                n = write(newsockfd, vyzva, strlen(vyzva) + 1);
                n = read(newsockfd, buffer, 255);
                char heslo[50];
                bzero(heslo, 50);
                strcpy(heslo, buffer);
                printf("%s \n", heslo);
                if (checkName(meno) == 0) {
                    addUser(meno, heslo);
                    logged = login(meno, heslo);
                    strcpy(vyzva, "Vitaj ");
                    strcat(vyzva, meno);
                    n = write(newsockfd, vyzva, strlen(vyzva) + 1);
                } else {
                    strcpy(vyzva, "Uzivatel s takým meno už existuje :(");
                    n = write(newsockfd, vyzva, strlen(vyzva) + 1);
                }
            }
        }

        while (logged == 1) {
            bzero(buffer, 256);
            n = read(newsockfd, buffer, 255);
            if (n < 0) {
                perror("Error reading from socket");
                return 4;
            }
            action = atoi(buffer);
            switch (action) {
                case 1:
                    while (1 == 1) {
                        n = read(newsockfd, buffer, 255);
                        if (strncmp("end", buffer, 3) == 0) {
                            break;
                        }
                        const char *msg = "I got your message";
                        n = write(newsockfd, msg, strlen(msg) + 1);

                        if (n < 0) {
                            perror("Error writing to socket");
                            return 5;
                        }
                    }
                    break;
                case 2:
                    getContacts(username, newsockfd);
                    break;
                case 3:
                    n = read(newsockfd, buffer, 255);
                    strcpy(search, buffer);
                    if (findContact(username, search) == 1) {
                        strcpy(buffer,"Uzivatel uz je v kontaktoch\n");
                        n = write(newsockfd, buffer, strlen(buffer));
                    } else {
                        sendFriendRequest(username, search);
                        strcpy(buffer,"Uzivatelovi bola poslana ziadost\n");
                        n = write(newsockfd, buffer, strlen(buffer));
                    }
                    break;
                case 4:
                    getFriendRequests(username, newsockfd);
                    bzero(buffer, 256);
                    n = read(newsockfd, buffer, 255);
                    if(strncmp(buffer, "0", 1)!=0) {
                        confirmFriendRequest(username, buffer);
                    }
                    break;
                case 5:
                    getContacts(username, newsockfd);
                    n = read(newsockfd, buffer, 255);
                    if(strncmp(buffer, "0", 1)!=0) {
                        removeFriend(username, buffer);
                    }
                    break;
                case 6:
                    logged = 0;
                    break;
                case 7:
                    removeUser(username);
                    logged = 0;
                    break;
                case 8:
                    logged = 0;
                    on = 0;
                    break;
                default:
                    break;
            }
        }
    }
    close(newsockfd);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd;
    socklen_t cli_len;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    char buffer[256];
/*
    char kkk[200];
    strcpy(kkk,"JanoX");
    kkk[strcspn(kkk, "X")] = 0;
    strcat(kkk, " je nepotvrdeny");
    printf("%s", kkk);*/

    if (argc < 2)
    {
        fprintf(stderr,"usage %s port\n", argv[0]);
        return 1;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 1;
    }

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error binding socket address");
        return 2;
    }

    listen(sockfd, 5);
    cli_len = sizeof(cli_addr);
    bzero(buffer, 256);

    while(1==1) {
        newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &cli_len);
        if (newsockfd < 0)
        {
            perror("ERROR on accept");
            return 3;
        }
        pthread_t client;
        pthread_create(&client, NULL, &userInteraction, newsockfd);
    }
    close(sockfd);

    return 0;
}


/*
        for (int i = 0; buffer[i] != '\0'; ++i) {
            buffer[i] += 3;
        }
        printf("Here is caesar message: %s\n", buffer);
        */