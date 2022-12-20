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
    
    char line_raw[52];
    char* fgets_status = (char*) 1;
    
    while (fgets_status != NULL){
        // get one line from the file
        fgets_status = fgets(line_raw, sizeof(line_raw), fd);
        
        // figure out how long that line is
        char* line_end = memchr(line_raw, '\n', sizeof(line_raw));
        i32 line_length = line_end - line_raw;
        assert(line_length % 2 == 0);
        if (line_length == 0){
            break;
        }
        i32 pocket_size = line_length/2;
        
        char* pocket1 = line_raw;
        char* pocket2 = line_raw + pocket_size;
        
        // check if any items in the pocket are the same
        for (i32 i=0; i<pocket_size; ++i){
            if (memchr(pocket2, pocket1[i], pocket_size) != NULL){
                char match = pocket1[i];
                
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
