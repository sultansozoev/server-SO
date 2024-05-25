#include "contact.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Contact *contacts = NULL;

void addContact(char* name, int number)
{
    Contact* new = (Contact*) malloc(sizeof(Contact));
    Contact* last_node = contacts;
    new->name = malloc(strlen(name) + 1);
    strcpy(new->name, name);
    new->number = number;
    new->next = NULL;

    if (contacts == NULL) {
        contacts = new;
        return;
    }
    while (last_node->next != NULL) last_node = last_node->next;
    last_node->next = new;
}

Contact* deleteContact(const char* name)
{
    Contact* current = contacts;
    Contact* previous = NULL;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (previous == NULL) {
                contacts = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            return current;
        }
        previous = current;
        current = current->next;
    }
    return NULL;
}

Contact * findContact(char* name)
{
    Contact* current = contacts;
    while (current != NULL) {
        if (strcmp(name, current->name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

Contact* changeNumber(const char* name, int number)
{
    Contact* current = contacts;
    while (current != NULL) {
        if (strcmp(name, current->name) == 0) {
            current->number = number;
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void printContact(char* buff)
{
    Contact* current = contacts;
    while (current != NULL) {
        char temp[MAX];
        sprintf(temp, "Nome: %s - numero: %d", current->name, current->number);
        strcat(buff, temp);
        strcat(buff, "\n");
        current = current->next;
    }
}
