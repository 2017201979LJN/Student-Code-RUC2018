#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bof(char *str)
{
    char buffer[12];
    
    /* The following statement has a buffer overflow problem */ 
    strcpy(buffer, str);
    
    return 1;
}

int main(int argc, char **argv)
{
    char str[517];
    FILE *badfile;

    badfile = fopen("badfile", "r");
    fread(str, sizeof(char), 517, badfile);
    bof(str);

    printf("Returned Properly\n");
    return 1;
}
