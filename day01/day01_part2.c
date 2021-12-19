#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv){
    FILE * fp;
    char * line = NULL;
    size_t read, len = 0;
    unsigned int counter = 0, result = 0;

    fp = fopen("input.txt", "r");
    if (fp == NULL) exit(EXIT_FAILURE);

    unsigned int measurement_window[3] = {0,0,0};
    unsigned int lastWindow = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        measurement_window[counter % 3] = atoi(line);
        counter++;
        // skip first window, since theres no previous measurement
        if(counter < 4) lastWindow = measurement_window[0]+measurement_window[1]+measurement_window[2];
        else{
            unsigned int tmp = measurement_window[0]+measurement_window[1]+measurement_window[2];
            if(tmp > lastWindow) result++;
            lastWindow = tmp;
        }
    }

    printf("%d 3-measurement windows are larger than the previous window\n", result);

    fclose(fp);
    if (line) free(line);

    return 0;
}