#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef __uint32_t u32;
typedef __uint16_t u16;
typedef __uint8_t u8;


u8 count_matching_prefix_bits(const u32 a, const u32 b, const u8 width){
    u8 result = 0;
    for(int i = width-1; i >= 0; i--){
        if(((a >> i) & 0x01) == ((b >> i) & 0x01)) result++;
        else break;
    }
    return result;
}


int main(int argc, char ** argv){

    FILE * fp;
    char * line = NULL;
    size_t read, len = 0;

    fp = fopen("input.txt", "r");
    if (fp == NULL) exit(EXIT_FAILURE);

    //get first line, determine the width of the diagnostic binary number
    read = getline(&line, &len, fp);
    if(read == -1) exit(EXIT_FAILURE); // no input -> error

    u16 width = strlen(line)-1;
    u32 high_bits_counter = 0;
    u32 low_bits_counter = 0;
    u32 numbers_amount = 0;
    u32 numbers_length = 2048;
    u32 * numbers = calloc(numbers_length, sizeof(__uint32_t));

    // count the high bits on position 0, count the numbers and create the numbers array
    do{
        // realloc the array if more space needed.
        if(numbers_amount == numbers_length){
            numbers_length *= 2;
            numbers = realloc(numbers, numbers_length);
        }

        if(line[0] == '1')  high_bits_counter++;
        else low_bits_counter++;

        for(int i = width-1; i >= 0; i--) numbers[numbers_amount] += (line[i] == '1' ? 1 : 0) << (width-i-1);
        numbers_amount++;
    }while ((read = getline(&line, &len, fp)) != -1);


    // find oxy
    u32 oxy = 0;
    u32 oxy_mask = 0 ;
    u32 count = numbers_amount;
    for(int i = width; i > 0; i--){
        if(high_bits_counter*2 >= count) oxy_mask += 1 << (i-1);

        count = 0;
        high_bits_counter = 0;
        for(int j = 0; j < numbers_amount; j++){
            u8 match = count_matching_prefix_bits(oxy_mask, numbers[j], width);
            if(count_matching_prefix_bits(oxy_mask, numbers[j], width) >= width-i+1){
                oxy = numbers[j];
                count++;
                if((numbers[j] >> (i-2)) & 0x01 == 1) high_bits_counter++; 
            }
        }
        if(count == 1) break;
    }


    // find co2
    u32 co2 = 0;
    u32 co2_mask = 0 ;
    count = numbers_amount;
    for(int i = width; i > 0; i--){
        if(low_bits_counter*2 > count) co2_mask += 1 << (i-1);
        count = 0;
        low_bits_counter = 0;
        for(int j = 0; j < numbers_amount; j++){
            u8 match = count_matching_prefix_bits(co2_mask, numbers[j], width);
            if(count_matching_prefix_bits(co2_mask, numbers[j], width) >= width-i+1){
                co2 = numbers[j];
                count++;
                if(((numbers[j] >> (i-2)) & 0x01) == 0){
                    low_bits_counter++;
                } 
            }

        }

        if(count == 1) break;
    }

    printf("oxy: %d\n", oxy);
    printf("co2: %d\n", co2);

    printf("result: %d * %d = %d\n", oxy, co2, oxy*co2);

    fclose(fp);
    if (line) free(line);

    free(numbers);

    return 0;
}