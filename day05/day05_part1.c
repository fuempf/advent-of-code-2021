#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef __uint8_t u8;
typedef __uint32_t u32;

u32 mapsize = 1024; // starting with 1024*1024 map, if bigger map needed, than calc new size, calloc and copy..
u8 * map;
u32 counter = 0;

u8 getValue(u32 x, u32 y){
    return map[y*mapsize+x];
}

void setValue(u32 x, u32 y, u8 value){
    map[y*mapsize+x] = value;
}

void increase_mapsize(u32 lower_bound){
    u32 old_mapsize = mapsize;
    while(mapsize <= lower_bound) mapsize *= 2;
    u8 * newmap = calloc(mapsize*mapsize, sizeof(u8));
    for(int i = 0; i < old_mapsize; i=i+old_mapsize){
        memcpy(&(newmap[i/old_mapsize*mapsize]), &(map[i]), old_mapsize);
    }
    free(map);
    map = newmap;
}

void draw_line(u32 x0, u32 y0, u32 x1, u32 y1){
    if(x0 != x1 && y0 != y1) return; //skip non vertical and non horizontal lines..
    if(x0 == x1){
        if(y0>y1){y0+=y1;y1=y0-y1;y0-=y1;}
        for(int i = y0; i <= y1; i++){
            u32 value = getValue(x0, i)+1;
            if(value == 2) counter++;
            setValue(x0,i,value);
        }
    }else{
        if(x0>x1){x0+=x1; x1=x0-x1; x0-=x1;}
        for(int i = x0; i <= x1; i++){
            u32 value = getValue(i, y0)+1;
            if(value == 2) counter++;
            setValue(i,y0,value);
        }
    }
}

int main(int argc, char ** argv){

    FILE * fp;
    char * line = NULL;
    size_t read, len = 0;

    fp = fopen("input.txt", "r");
    if (fp == NULL) exit(EXIT_FAILURE);

    map = calloc(mapsize*mapsize, sizeof(u8));
    
    u32 x0, x1, y0, y1;
    while ((read = getline(&line, &len, fp)) != -1) {
        sscanf(line, "%d,%d -> %d,%d", &x0, &y0, &x1, &y1);
        line[strlen(line)-1] = '\0';
        if(x0 >= mapsize) increase_mapsize(x0);
        if(x1 >= mapsize) increase_mapsize(x1);
        if(y0 >= mapsize) increase_mapsize(y0);
        if(y1 >= mapsize) increase_mapsize(y1);
        draw_line(x0, y0, x1, y1);
    }

    printf("result: %d\n",counter);

    free(map);

    fclose(fp);
    if (line) free(line);

    return 0;
}