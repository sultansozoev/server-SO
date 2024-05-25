#include "contact.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Contact *contacts = NULL;

void addContact(char* name, int number)
{
    Contact* newContact = (Contact*) malloc(sizeof(Contact));
    if (!newContact) {
        perror("Failed to allocate memory for newContact contact");
        exit(EXIT_FAILURE);
    }

    newContact->name = malloc(strlen(name) + 1);
    if (!newContact->name) {
        perror("Failed to allocate memory for contact name");
        free(newContact);
        exit(EXIT_FAILURE);
    }

    strcpy(newContact->name, name);
    newContact->number = number;
    newContact->next = NULL;

    if (contacts == NULL)
        contacts = newContact;
    else {
        Contact* last_node = contacts;
        while (last_node->next != NULL) last_node = last_node->next;
        last_node->next = newContact;
    }
}

Contact* deleteContact(const char* name)
{
    Contact* current = contacts;
    Contact* previous = NULL;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (previous == NULL)
                contacts = current->next;
            else
                previous->next = current->next;
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
        if (strcmp(name, current->name) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

Contact* changeContactNumber(const char* name, int number)
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
