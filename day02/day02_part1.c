#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv){

    FILE * fp;
    char * line = NULL;
    size_t read, len = 0;

    fp = fopen("input.txt", "r");
    if (fp == NULL) exit(EXIT_FAILURE);
    
    signed int xpos = 0;
    signed int ypos = 0;

    signed int value;
    char command[10];
    while ((read = getline(&line, &len, fp)) != -1) {
        sscanf(line, "%s %d", command, &value);
        if(strcmp(command, "forward") == 0) xpos += value;
        else if(strcmp(command, "down")) ypos -= value;
        else ypos += value;
    }

    printf("position: horizontal=%d, depth=%d. result: %d * %d = %d\n",xpos, ypos, xpos, ypos, xpos*ypos);

    fclose(fp);
    if (line) free(line);

    return 0;
}