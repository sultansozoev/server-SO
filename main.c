#include "contact.h"
#include "user.h"

#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

void commands(int connfd)
{
    char username[MAX];
    char password[MAX];
    int authenticated = 0;

    while (1) {
        char command[MAX];
        char buff[MAX];
        char buffN[MAX];
        bzero(command, MAX);
        bzero(buff, MAX);
        bzero(buffN, MAX);
        read(connfd, command, sizeof(buff));

        if (command[0] == 0) {
            printf("\nClient Exit...");
            break;
        }
        printf("From client: %s\t", command);
        if (strcmp(command, "accedere") == 0) {
            read(connfd, username, sizeof(username));
            read(connfd, password, sizeof(password));

            User* user = findUser(username);
            if (user != NULL && strcmp(password, user->password) == 0) {
                authenticated = 1;
                printf("%s authenticated successfully\n", username);
                write(connfd, "User authenticated successfully", sizeof("User authenticated successfully"));
            } else {
                printf("Authentication failed for client\n");
                write(connfd, "Authentication failed", sizeof("Authentication failed"));
                continue;
            }
        } else if (strcmp(command, "inserire") == 0) {
            if (!authenticated) {
                write(connfd, "Authentication required", sizeof("Authentication required"));
                continue;
            }
            write(connfd, "User authenticated", sizeof("User authenticated"));
            read(connfd, buff, sizeof(buff));
            char* name = (char *) &buff;
            if (findContact(name) != NULL) {
                printf("Contact already exists\n");
                write(connfd, "Contact already exists", sizeof("Contact already exists"));
                continue;
            } else {
                write(connfd, "Contact available", sizeof("Contact available"));
            }
            printf("\nNome: %s", buff);
            read(connfd, buffN, sizeof(buffN));
            printf("\nNumero: %s\n", buffN);
            int number = atoi(buffN);
            addContact(name, number);
            bzero(buff, MAX);
            bzero(buffN, MAX);
        } else if (strcmp(command, "registrare") == 0) {
            bzero(username, MAX);
            read(connfd, username, sizeof(username));
            bzero(password, MAX);
            read(connfd, password, sizeof(password));

            if (findUser(username) != NULL) {
                printf("Username already exists\n");
                write(connfd, "Username already exists", sizeof("Username already exists"));
                continue;
            }
            addUser(username, password);
            printf("User registered successfully\n");
            write(connfd, "User registered successfully", sizeof("User registered successfully"));
        } else if (strcmp(command, "cancella") == 0) {
            if (!authenticated) {
                write(connfd, "Authentication required", sizeof("Authentication required"));
                continue;
            }
            write(connfd, "User authenticated", sizeof("User authenticated"));
            read(connfd, buff, sizeof(buff));
            char* name = (char *) &buff;
            printf("Nome: %s\n", buff);
            Contact* contact = deleteContact(name);
            bzero(buff, MAX);
            if (contact == NULL) {
                write(connfd, "Contatto non trovato", sizeof("Contatto non trovato"));
            } else {
                write(connfd, "Contatto trovato", sizeof("Contatto trovato"));
            }
        } else if (strcmp(command, "stampare") == 0) {
            printContact(buff);
            write(connfd, buff, sizeof(buff));
        } else if (strcmp(command, "modifica") == 0) {
            if (!authenticated) {
                write(connfd, "Authentication required", sizeof("Authentication required"));
                continue;
            }
            write(connfd, "User authenticated", sizeof("User authenticated"));
            read(connfd, buff, sizeof(buff));
            char *name = (char *) &buff;
            printf("Nome: %s\n", buff);
            read(connfd, buffN, sizeof(buffN));
            int number = atoi(buffN);
            Contact* contact = changeNumber(name, number);
            if (contact == NULL) {
                write(connfd, "Contatto non trovato", sizeof("Contatto non trovato"));
            } else {
                write(connfd, "Contatto trovato", sizeof("Contatto trovato"));
            }
        } else if (strcmp(command, "chiusura") == 0) {
            printf("\nClient Exit...\n");
            break;
        }
    }
}

void *clientHandler(void *arg)
{
    int connfd = *((int *)arg);
    free(arg);
    commands(connfd);
    close(connfd);
    return NULL;
}

int main(void)
{
    int sockfd;
    socklen_t len;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    while (1) {
        int *connfd = malloc(sizeof(int));
        *connfd = accept(sockfd, (SA *) &cli, &len);
        if (*connfd < 0) {
            printf("server accept failed... retrying\n");
            sleep(1);
            continue;
        } else printf("\nserver accept the client...\n");

        pthread_t clientThread;
        if (pthread_create(&clientThread, NULL, clientHandler, connfd) != 0) {
            printf("Failed to create thread\n");
            return 1;
        }
    }
}
