#include "../include/server.h"

#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

int main(void)
{
    setupServer();
    return 0;
}
