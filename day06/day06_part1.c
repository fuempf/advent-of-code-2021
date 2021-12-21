#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef __uint8_t u8;
typedef __uint32_t u32;


int main(int argc, char ** argv){

    FILE * fp;
    char * line = NULL;
    size_t len = 0;

    fp = fopen("input.txt", "r");
    if (fp == NULL) exit(EXIT_FAILURE);

    u32 fish_groups[7] = {0,0,0,0,0,0,0};
    u32 new_fishes[9] = {0,0,0,0,0,0,0,0,0};

    if(getline(&line, &len, fp) == -1) exit(EXIT_FAILURE); // no input -> error..
    for(int pos = 0; pos < len; pos++){
        if(line[pos] == ',') continue;
        if(line[pos] < 0x30 || line[pos] > 0x39) break;
        u8 fg = line[pos]-0x30;
        fish_groups[fg]++;
    }
    
    for(int i = 0; i < 80; i++){
        fish_groups[i%7] += new_fishes[0];
        for(int i = 1; i<9; i++){
            new_fishes[i-1] = new_fishes[i];
        }
        new_fishes[8] = fish_groups[i%7];
    }

    u32 fishes = 0;
    for(int i = 0; i < 7; i++){
        fishes += fish_groups[i];
    }
    for(int i = 0; i < 9; i++){
        fishes += new_fishes[i];
    }
    printf("result: %d\n",fishes);

    fclose(fp);
    if (line) free(line);

    return 0;
}