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
    
    // 
    char line_raw[10];
    char* fgets_status;
    fgets_status = fgets(line_raw, sizeof(line_raw), fd);
    assert(fgets_status != NULL);
    players[0].position = atoi(line_raw) - 1;
    
    fgets_status = fgets(line_raw, sizeof(line_raw), fd);
    assert(fgets_status != NULL);
    players[1].position = atoi(line_raw) - 1;
    
    
    
    
    fclose(fd);
    
    printf("%d\n",final_score); 
    
    return 0;
}
