#include "../include/commands.h"
#include "../include/user.h"
#include "../include/contact.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

void processCommands(int connfd)
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
        printf("From client: %s\n", command);
        if (strcmp(command, "login") == 0) {
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
        } else if (strcmp(command, "add") == 0) {
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
            printf("\nName: %s", buff);
            read(connfd, buffN, sizeof(buffN));
            printf("\nNumber: %s\n", buffN);
            char* number = buffN;
            addContact(name, number);
            bzero(buff, MAX);
            bzero(buffN, MAX);
        } else if (strcmp(command, "register") == 0) {
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
        } else if (strcmp(command, "delete") == 0) {
            if (!authenticated) {
                write(connfd, "Authentication required", sizeof("Authentication required"));
                continue;
            }
            write(connfd, "User authenticated", sizeof("User authenticated"));
            read(connfd, buff, sizeof(buff));
            char* name = (char *) &buff;
            printf("Name: %s\n", buff);
            Contact* contact = deleteContact(name);
            bzero(buff, MAX);
            if (contact == NULL) {
                write(connfd, "Contact not found", sizeof("Contact not found"));
            } else {
                write(connfd, "Contact deleted", sizeof("Contact deleted"));
            }
        } else if (strcmp(command, "print") == 0) {
            printContact(buff);
            write(connfd, buff, sizeof(buff));
        } else if (strcmp(command, "modify") == 0) {
            if (!authenticated) {
                write(connfd, "Authentication required", sizeof("Authentication required"));
                continue;
            }
            write(connfd, "User authenticated", sizeof("User authenticated"));
            read(connfd, buff, sizeof(buff));
            char *name = (char *) &buff;
            printf("Name: %s\n", buff);
            read(connfd, buffN, sizeof(buffN));
            char* number = buffN;
            Contact* contact = changeContactNumber(name, number);
            if (contact == NULL) {
                write(connfd, "Contact not found", sizeof("Contact not found"));
            } else {
                write(connfd, "Contact modified", sizeof("Contact modified"));
            }
        } else if (strcmp(command, "close") == 0) {
            printf("\nClient Exit...\n");
            break;
        }
    }
}