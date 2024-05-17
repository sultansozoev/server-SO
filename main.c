#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

typedef struct Rubrica {
    int number;
    char* name;
    struct Rubrica * next;
} Rubrica;
Rubrica *head;

void delete(const char* name) {
    if (head->name == name) {
        head = head->next;
    }
    Rubrica* current = head;
    Rubrica* previous = head;
    while (current != NULL) {
        if (current->name == name) {
            previous->next = current->next;
            free(current);
            return;
        }
        current = current->next;
    }
}

Rubrica* search(const char* name)
{
    Rubrica* current = head;
    while (current != NULL) {
        if (current->name == name)
            return current;
        current = current->next;
    }
    return NULL;
}

void addContact(char* name, int number)
{
    Rubrica* new = (Rubrica*) malloc(sizeof(Rubrica));
    Rubrica* last_node = head;
    new->name = malloc(strlen(name) + 1);
    strcpy(new->name, name);
    new->number = number;
    new->next = NULL;

    if (head == NULL) {
        head = new;
        return;
    }
    while (last_node->next != NULL) last_node = last_node->next;
    last_node->next = new;
}

char* getList() {
    Rubrica* current = head;
    char* list;
    while (current != NULL) {
        current = current->next;
    }
    return list;
}

void func(int connfd)
{
    while (1) {
        char command[MAX];
        char buff[MAX];
        char buffN[MAX];
        bzero(command, MAX);
        bzero(buff, MAX);
        read(connfd, command, sizeof(buff));

        if (command[0] == 0) {
            printf("\nClient Exit...");
            break;
        }
        printf("From client: %s\t", command);
        if (strcmp(command, "inserire") == 0) {
            read(connfd, buff, sizeof(buff));
            char* name = (char *) &buff;
            printf("\nNome: %s", buff);
            read(connfd, buffN, sizeof(buffN));
            printf("\nNumero: %s\n", buffN);
            int number = atoi(buffN);
            addContact(name, number);
        } else if (strcmp(command, "cancella") == 0) {
            read(connfd, buff, sizeof(buff));
            char* name = (char *) &buff;
            printf("Nome: %s\n", buff);
            delete(name);
        } else if (strcmp(command, "stampare") == 0) {
            Rubrica* current = head;
            bzero(buff, MAX);
            while (current != NULL) {
                char temp[MAX];
                sprintf(temp, "Nome: %s - numero: %d", current->name, current->number);
                strcat(buff, temp);
                strcat(buff, "\n");
                current = current->next;
            }
            write(connfd, buff, sizeof(buff));
            bzero(buff, MAX);
        } else if (strcmp(command, "modifica") == 0) {
            read(connfd, buff, sizeof(buff));
            char* name = (char *) &buff;
            printf("Nome: %s\n", buff);
            Rubrica* contact = search(name);
            if (contact == NULL) {
                write(connfd, "Contatto non trovato", sizeof("Contatto non trovato"));
            } else {
                read(connfd, buff, sizeof(buff));
                int number = atoi(buff);
                contact->number = number;
            }
        } else if (strcmp(command, "chiusura") == 0) {
            printf("\nClient Exit...\n");
            break;
        }
    }
}

int main(void)
{
    int sockfd, connfd;
    socklen_t len;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    while (1) {
        connfd = accept(sockfd, (SA *) &cli, &len);
        if (connfd < 0) {
            printf("server accept failed... retrying\n");
            sleep(1);
            continue;
        } else printf("\nserver accept the client...\n");
        // Function for chatting between client and server
        func(connfd);
        // After chatting close the socket
        close(connfd);
    }
    close(sockfd);
}
