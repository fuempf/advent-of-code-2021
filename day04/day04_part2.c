#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef __uint32_t u32;
typedef __uint8_t u8;
typedef u32 bingo_board[5][5]; 
typedef u32 flatten_bingo_board[25];


u32 * load_draws(FILE * fp, u32 * drawcount){
    char * line = NULL;
    size_t read, len = 0;
    // first line contains the draw order.
    read = getline(&line, &len, fp);
    if(read == -1) exit(EXIT_FAILURE);
    // count the draws
    int i = 0;
    *drawcount = 1;
    while(line[i] != '\0'){
        if(line[i] == ',') (*drawcount)++;
        i++;
    }
    u32 * draws = malloc(sizeof(u32) * (*drawcount));
    // assuming numbers are <100
    char tmp[3];
    u32 lastpos = 0;
    u32 counter = 0;
    i = -1;
    do{
        i++;
        if(line[i] == ',' || line[i] == '\0'){
            memcpy(tmp, &(line[lastpos]), i-lastpos);
            tmp[i-lastpos] = '\0';
            draws[counter] = atoi(tmp);
            counter++;
            lastpos=i+1;
        }
    }while(line[i] != '\0');
    if (line) free(line);
    return draws;
}


bingo_board * load_boards(FILE * fp, u32 * boardcount){
    char * line = NULL;
    size_t len = 0;
    u8 row = 0;
    *boardcount = 0;
    // count boards
    while(getline(&line, &len, fp) != -1){
        if(line[0] == '\n') (*boardcount)++;
    }
    bingo_board * boards = calloc(*boardcount, sizeof(bingo_board));

    //reset pointer..
    fseek(fp, 0, SEEK_SET);
    getline(&line, &len, fp); // get rid of first two lines
    getline(&line, &len, fp); // get rid of first two lines
    u32 boardindex = 0;
    while(getline(&line, &len, fp) != -1){
        if(line[0] == '\n'){ boardindex++; row = 0; continue;}
        if(row > 4){boardindex++; row = 0;}
        sscanf(line, "%d %d %d %d %d", 
                    &(boards[boardindex][row][0]), 
                    &(boards[boardindex][row][1]), 
                    &(boards[boardindex][row][2]), 
                    &(boards[boardindex][row][3]), 
                    &(boards[boardindex][row][4]));
        
        row++;
    }
    if (line) free(line);
    return boards;
}

// we use the most significant bit as a marker (since the numbers are <100)
u8 has_board_won(bingo_board * board){
    u8 rowcount;
    u8 colcount;
    for(int i = 0; i < 5; i++){
        rowcount = 0;
        colcount = 0;
        for(int j = 0; j < 5; j++){
            if((*board)[i][j] & 0x80) rowcount++;
            if((*board)[j][i] & 0x80) colcount++;
        }
        if(rowcount == 5 || colcount == 5) return 1;
    }
    return 0;
}

u8 mark_board(bingo_board * board, u8 draw){
    flatten_bingo_board * flatten_board = (flatten_bingo_board *) board;
    for(int i = 0; i < 25; i++){
        if((*flatten_board)[i] == draw) (*flatten_board)[i] = (*flatten_board)[i] + 0x80;
    }
}

u32 get_sum_of_unmarked(bingo_board * board){
    u32 sum = 0;
    flatten_bingo_board * flatten_board = (flatten_bingo_board *) board;
    for(int i = 0; i < 25; i++){
        if(!((*flatten_board)[i] & 0x80)) sum += (*flatten_board)[i];
    }
    return sum;
}

int main(int argc, char ** argv){

    FILE * fp;
   

    fp = fopen("input.txt", "r");
    if (fp == NULL) exit(EXIT_FAILURE);

    u32 drawcount;
    u32 * draws = load_draws(fp, &drawcount);

    u32 boardcount;
    bingo_board * boards = load_boards(fp, &boardcount);

    fclose(fp);


    bingo_board * last_winning_board = NULL;
    u32 last_winning_draw = 0;
    u32 wonmaplen = boardcount / 8 + 1;
    u8 * wonmap = calloc(wonmaplen, sizeof(u8));
    u32 boardsleft = boardcount;
    for(int i = 0; i < drawcount; i++){
        for(int j = 0; j < boardcount; j++){
            mark_board(&(boards[j]), draws[i]);
            if(has_board_won(&(boards[j]))){
                if(wonmap[j/8] & (0x01 << (j%8))) continue;
                wonmap[j/8] += 1 << (j%8);
                boardsleft--;
                last_winning_draw = draws[i];
                last_winning_board = &(boards[j]);
                if(boardsleft == 0) goto lastwin;
            }
        }
    }
lastwin:
    if(last_winning_board == NULL){
        printf("no board won :(\n");
        exit(EXIT_FAILURE);
    }

    u32 sum = get_sum_of_unmarked(last_winning_board);
    printf("sum of unmarked: %d, winning draw: %d, result: %d * %d = %d\n", sum, last_winning_draw, sum, last_winning_draw, sum * last_winning_draw);

    free(wonmap);
    free(draws);
    free(boards);

    return 0;
}