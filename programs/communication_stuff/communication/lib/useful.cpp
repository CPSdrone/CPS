#include "useful.h"

int die(const char *msg)
{
    perror(msg);
    exit(1);
}

