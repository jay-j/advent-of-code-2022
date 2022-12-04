#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "../base_types.h"

int main(int argc, char *argv[]){
    i32 score = 0;
    
    // read in the file
    char filename[64];
    sscanf(argv[1], "%s", filename);
    FILE* fd;
    fd = fopen(filename, "r");
    assert(fd != NULL);
    
    // 
    char line_raw[8];
    char* fgets_status;
    fgets_status = fgets(line_raw, sizeof(line_raw), fd);
    
    for(; fgets_status != NULL;
        fgets_status = fgets(line_raw, sizeof(line_raw), fd)){
        
        if (line_raw[0] == '\n'){
            break;
        }
        
        // Just get the raw characters from the line string
        char raw_op = line_raw[0];
        char raw_my = line_raw[2];
        printf("opponent: %c     mine: %c\n", raw_op, raw_my);
        
        // how far would move_op need to shift to always put the 1 in the 3rd (leftmost) bit?
        u64 shift = 2 - (raw_op - 0x41);
        
        // make binary numbers: scissors, paper, rock
        u64 move_my = 1 << (raw_my - 0x58); // X-->0
        move_my += move_my << 3; // duplicate to support rotations
        
        // now shift
        move_my = move_my << (shift);
        
        // and read off the table
        if ((move_my & 0x20) > 0){
            printf("draw\n");
            score += 3;
        }
        else if (( move_my & 0x10) > 0){
            printf("loss\n");
        }
        else if ((move_my & 0x8) > 0){
            printf("win\n");
            score += 6;
        }
        
        // add the portion of the score corresponding to the selected shape
        score += (raw_my - 0x58) + 1;
    }
    
    fclose(fd);
    
    printf("%d\n",score); 
    
    return 0;
}

// 11908 was too high