#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv){

    FILE * fp;
    char * line = NULL;
    size_t read, len = 0;

    fp = fopen("input.txt", "r");
    if (fp == NULL) exit(EXIT_FAILURE);
    
    // get first measurement
    read = getline(&line, &len, fp);
    // no measurements -> error
    if(read == -1) exit(EXIT_FAILURE); 
    int last_measurement = atoi(line);

    unsigned int counter = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        int current = atoi(line);
        if(last_measurement < current) counter++;
        last_measurement = current;
    }

    printf("%d measurements are larger than the previous\n", counter);

    fclose(fp);
    if (line) free(line);

    return 0;
}