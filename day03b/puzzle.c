#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "../base_types.h"

int main(int argc, char *argv[]){
    
    // read in the file
    char filename[64];
    sscanf(argv[1], "%s", filename);
    FILE* fd;
    fd = fopen(filename, "r");
    assert(fd != NULL);
    
    i32 final_score = 0;
    
    char line1_raw[52];
    char line2_raw[52];
    char line3_raw[52];
    
    string8 lines[3];
    lines[0].str = (u8*) line1_raw;
    lines[1].str = (u8*) line2_raw;
    lines[2].str = (u8*) line3_raw;
    
    char* fgets_status = (char*) 1;
    
    while (fgets_status != NULL){
        // get three lines from the file
        for (i32 i=0; i<3; ++i){
            fgets_status = fgets((char*) lines[i].str, sizeof(line1_raw), fd);
            
            // figure out how long that line is
            u8* line_end = memchr(lines[i].str, '\n', sizeof(line1_raw));
            lines[i].size = line_end - lines[i].str;
        }
        
        // for every item in the first backpack..
        for (u64 i=0; i<lines[0].size; ++i){
            u8 match_found = TRUE;
            
            for (u64 j=1; j<3; ++j){ // for the other backpack
                if (memchr(lines[j].str, lines[0].str[i], lines[j].size) == NULL){
                    match_found = FALSE;
                }
            }
            
            if (match_found == TRUE){
                char match = lines[0].str[i];
                
                if (match > 0x60){ // lower case
                    final_score += match - 0x60;
                }
                else{  // upper case
                    final_score += match - 0x40 + 26;
                }
                break;
            }
        }
        
    }
    
    fclose(fd);
    
    printf("%d\n",final_score); 
    return 0;
}
