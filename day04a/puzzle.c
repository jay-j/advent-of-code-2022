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
    
    char line_raw[64];
    char* fgets_status;
    fgets_status = fgets(line_raw, sizeof(line_raw), fd);
    assert(fgets_status != NULL);
    
    i32 high[2];
    i32 low[2];
    i32 final_score = 0;
    
    while (fgets_status != NULL){
        
        i32 check = sscanf(line_raw, "%d-%d,%d-%d", low, high, low+1, high+1);
        if (check != 4){
            break;
        }
        
        i32 delta_high = high[1] - high[0];
        i32 delta_low = low[1] - low[0];
        i32 delta = delta_high * delta_low;
        assert(high[0] >= low[0]);
        assert(high[1] >= low[1]);
        
        if (delta <= 0){
            printf("line %s has an overlap with %d delta\n", line_raw, delta);
            final_score++;
        }
        
        fgets_status = fgets(line_raw, sizeof(line_raw), fd);
    }
    
    fclose(fd);
    
    printf("%d\n",final_score); 
    
    // 432 is too high
    
    return 0;
}
