#ifndef CONTACT_H
#define CONTACT_H

#define MAX 1024

typedef struct Contact {
    char* number;
    char* name;
    struct Contact* next;
} Contact;
Contact *contacts;

void addContact(char*, char*);
Contact* deleteContact(const char*);
Contact* findContact(char*);
Contact* changeContactNumber(const char*, char*);
void printContact(char *);

#endif
