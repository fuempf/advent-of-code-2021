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

    // input is only one line..
    if(getline(&line, &len, fp) == -1) exit(EXIT_FAILURE); // no input -> error..

    //count the numbers and sort them using kind of redix sort to stay in O(n)
    u32 pos = 0;
    u32 lastpos = 0;
    u32 bl = 2048;
    u32 * buckets = calloc(bl, sizeof(u32));
    u32 count = 0;
    while(pos < len){
        if(line[pos] == ',' || line[pos] == '\0'){
            u32 tmp = atoi(&(line[lastpos]));
            if(tmp >= bl){
                bl*=2;
                buckets = realloc(buckets, bl);
                memset(buckets+(sizeof(u32)*bl/2), 0, bl/2); // zero the new part of the buffer
            }
            buckets[tmp]++;
            count++;
            lastpos = pos+1;
            if(line[pos] == '\0') break;
        }
        pos++;
    }

    u32 * sorted = malloc(count * sizeof(32)); 
    u32 index = 0;
    u32 max = 0;
    for(int i = 0; i < bl; i++){
        while(buckets[i] > 0){
            sorted[index] = i;
            buckets[i]--;
            index++;
            max = i;
        }
    }

    // in this example, the median is not the best position.. but its a good starting point.. we start there and search the best one
    u32 median = sorted[count/2];

    u32 result = 0;
    for(int i = 0; i < count; i++){
        u32 n = median>sorted[i]?median-sorted[i]:sorted[i]-median;
        result += n*(n+1)/2;
    }

    
    u32 tmp_result;

    u32 big_median = median+1;
    do{
        tmp_result = 0;
        for(int i = 0; i < count; i++){
            u32 n = big_median>sorted[i]?big_median-sorted[i]:sorted[i]-big_median;
            tmp_result += n*(n+1)/2;
        }
        big_median++;
        if(tmp_result < result) result = tmp_result;
    }while(tmp_result <= result);

    u32 small_median = median-1;
    do{
        tmp_result = 0;
        for(int i = 0; i < count; i++){
            u32 n = small_median>sorted[i]?small_median-sorted[i]:sorted[i]-small_median;
            tmp_result += n*(n+1)/2;
        }        
        small_median--;
        if(tmp_result < result) result = tmp_result;
    }while(tmp_result <= result);

   if(tmp_result < result) result = tmp_result;

   printf("result: %d\n",result);

    fclose(fp);
    if (line) free(line);

    return 0;
}