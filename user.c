#include "user.h"

#include <stdlib.h>
#include <string.h>

User *users = NULL;

User* findUser(char* username)
{
    User* current = users;
    while (current != NULL) {
        if (strcmp(username, current->username) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void addUser(char* username, char* password)
{
    User* new = (User*) malloc(sizeof(User));
    User* last_node = users;
    new->username = malloc(strlen(username) + 1);
    new->password = malloc(strlen(password) + 1);
    strcpy(new->username, username);
    strcpy(new->password, password);
    new->next = NULL;

    if (users == NULL) {
        users = new;
        return;
    }
    while (last_node->next != NULL) last_node = last_node->next;
    last_node->next = new;
}