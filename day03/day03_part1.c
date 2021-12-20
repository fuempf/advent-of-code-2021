#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv){

    FILE * fp;
    char * line = NULL;
    size_t read, len = 0;

    fp = fopen("input.txt", "r");
    if (fp == NULL) exit(EXIT_FAILURE);

    //get first line, determine the width of the diagnostic binary number
    read = getline(&line, &len, fp);
    if(read == -1) exit(EXIT_FAILURE); // no input -> error
    unsigned int width = strlen(line)-1;
    unsigned int * high_bits_counter = calloc(width, sizeof(unsigned int));

    //count the high bits
    unsigned int linecount = 0;
    do{
        for(int i = width-1; i >= 0; i--){
            if(line[i] == '1')  high_bits_counter[i]++;
        }
        linecount++;
    }while ((read = getline(&line, &len, fp)) != -1);

    // calculate gamma and epsilon values
    __uint32_t gamma = 0;
    __uint32_t epsilon = 0;
    for(int i = width-1; i >= 0; i--){
        __uint8_t most_common = high_bits_counter[i]*2 >= linecount ? 1 : 0;
        gamma += most_common << (width-i-1);
        epsilon += (most_common==0?1:0) << (width-i-1);
    }


    printf("position: gamma=%d, epsilon=%d. result: %d * %d = %d\n",gamma, epsilon, gamma, epsilon, gamma*epsilon);

    fclose(fp);
    if (line) free(line);

    return 0;
}