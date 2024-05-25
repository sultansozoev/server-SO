#ifndef CONTACT_H
#define CONTACT_H

#define MAX 1024

typedef struct Contact {
    int number;
    char* name;
    struct Contact* next;
} Contact;
Contact *contacts;

void addContact(char*, int);
Contact* deleteContact(const char*);
Contact* findContact(char*);
Contact* changeContactNumber(const char*, int);
void printContact(char *);

#endif
