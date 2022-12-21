#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "../base_types.h"

#define LINE_MAX 4096

u64 check_norepeats(char* ptr){
    // search for repeats; IF repeat then FALSE - is not a marker
    // search backward from the starting ptr
    u64 result = TRUE;
    for (i32 i=0; i<4; ++i){
        for (i32 j=i+1; j<4; ++j){
            char val1 = *(ptr-i);
            char val2 = *(ptr-j);
            //printf("compare %c to %c\n", val1, val2);
            if (val1 == val2){
                result = FALSE;
                return result;
            }
        }
    }
    return result;
}


int main(int argc, char *argv[]){
    
    // read in the file
    char filename[64];
    sscanf(argv[1], "%s", filename);
    FILE* fd;
    fd = fopen(filename, "r");
    assert(fd != NULL);
    
    // 
    char* line_raw = malloc(LINE_MAX);
    char* fgets_status;
    fgets_status = fgets(line_raw, LINE_MAX, fd);
    assert(fgets_status != NULL);
    
    while (fgets_status != NULL){
        printf("look at the line %s\n", line_raw);
        char* ptr = line_raw + 3;
        
        while(check_norepeats(ptr) == FALSE){
            ptr++;
        }
        printf("found the marker at %lu\n", ptr - line_raw + 1);
        
        fgets_status = fgets(line_raw, LINE_MAX, fd);
    }
    
    fclose(fd);
    free(line_raw);
    
    return 0;
}
