#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "../base_types.h"

#define LINE_MAX 128
#define CARGO_MAX 256
#define STACK_QTY 16

typedef struct Stack{
    char cargo[CARGO_MAX];
    char* free; // above the last item; a free slot
} Stack;


void print_stacks(Stack* stacks, u64 columns){
    
    // find the max stack size
    u64 max_height = 0;
    for (u64 c=0; c<columns; ++c){
        if (stacks[c].free - stacks[c].cargo > max_height){
            max_height = stacks[c].free - stacks[c].cargo;
        }
    }
    max_height--;
    
    // print
    for (i32 row=max_height; row>=0; --row){
        
        for (u64 c=0; c<columns; ++c){
            if (stacks[c].free - stacks[c].cargo > row){
                printf("%c", *(stacks[c].cargo + row));
            }
            else{
                printf(" ");
            }
        } 
        printf("\n");
    }
    printf("123456789\n");
}


int main(int argc, char *argv[]){
    
    // initialize empty stacks
    Stack stacks[STACK_QTY] = {0};
    for (u64 i=0; i<STACK_QTY; ++i){
        stacks[i].free = stacks[i].cargo;
    }
    u64 stacks_in_use = 0;
    
    // read in the file
    char filename[64];
    sscanf(argv[1], "%s", filename);
    FILE* fd;
    fd = fopen(filename, "r");
    assert(fd != NULL);
    
    // 
    char* header = malloc(LINE_MAX*128); // just a big workspace 
    char* line_raw = header;
    char* fgets_status;
    fgets_status = fgets(line_raw, LINE_MAX, fd);
    assert(fgets_status != NULL);
    
    // read lines forward, cache them until find the stack bottoms
    while (line_raw[1] != '1'){
        line_raw += LINE_MAX;
        fgets_status = fgets(line_raw, LINE_MAX, fd);
        assert(fgets_status != NULL);
    }
    
    printf("last line got: %s\n", line_raw);
    
    // then go through the lines backward and push on contents
    while (line_raw > header){
        line_raw -= LINE_MAX;
        printf("looking in line %s\n", line_raw);
        
        // look for an open bracket '['
        // position of the open bracket says what stack to deal with
        // keep looking until all open brackets are found for the line
        char* token = memchr(line_raw, '[', LINE_MAX);
        while (token != NULL){
            token++;
            u64 column = (token - line_raw)/4;
            printf("need to place '%c' at %lu\n", *token, column);
            *stacks[column].free = *token;
            stacks[column].free++;
            
            if (column > stacks_in_use){
                stacks_in_use = column;
            }
            
            token = memchr(token, '[', LINE_MAX - (token - line_raw));
        }
    }
    stacks_in_use++;
    
    // skip past the blank line
    line_raw = header;
    fgets_status = fgets(line_raw, LINE_MAX, fd);
    
    // now read the instructions and follow them
    fgets_status = fgets(line_raw, LINE_MAX, fd);
    while (fgets_status != NULL){
        u64 qty;
        u64 src;
        u64 dst;
        i32 result = sscanf(line_raw, "move %lu from %lu to %lu", &qty, &src, &dst);
        if (result != 3){ // reading the last line wrong..
            break;
        }
        
        src--; // columns in input are 1-indexed
        dst--; // columns in input are 1-indexed
        
        // do the moves
        stacks[src].free -= qty;
        memcpy(stacks[dst].free, stacks[src].free, qty);
        stacks[dst].free += qty;
        
        //printf("%s", line_raw);
        //print_stacks(stacks, stacks_in_use);
        
        fgets_status = fgets(line_raw, LINE_MAX, fd);
    }
    
    // print the top thing in each stack
    // HNSNMTLHQ is correct
    for (u64 col=0; col<stacks_in_use; ++col){
        printf("%c", *(stacks[col].free - 1));
    }
    printf("\n");
    
    fclose(fd);
    
    free(header);
    
    return 0;
}
