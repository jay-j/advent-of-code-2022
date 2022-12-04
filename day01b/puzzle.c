#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "../base_types.h"

int main(int argc, char *argv[]){
    i32 calories_biggest[3] = {0, 0, 0};
    i32 calories_current = 0;
    
    // read in the file
    char filename[64];
    sscanf(argv[1], "%s", filename);
    FILE* fd;
    fd = fopen(filename, "r");
    assert(fd != NULL);
    
    // 
    char line_raw[30];
    char* fgets_status;
    fgets_status = fgets(line_raw, sizeof(line_raw), fd);
    
    while (fgets_status != NULL){
        if (line_raw[0] == '\n'){
            // manual sort
            if (calories_current > calories_biggest[0]){
                calories_biggest[2] = calories_biggest[1];
                calories_biggest[1] = calories_biggest[0];
                calories_biggest[0] = calories_current;
            }
            else if (calories_current > calories_biggest[1]){
                calories_biggest[2] = calories_biggest[1];
                calories_biggest[1] = calories_current;
            }
            else if (calories_current > calories_biggest[2]){
                calories_biggest[2] = calories_current;
            }
            
            // new elf
            calories_current = 0;
        }
        else{
            calories_current += atoi(line_raw);
        }
        
        fgets_status = fgets(line_raw, sizeof(line_raw), fd);
        
    }
    
    
    fclose(fd);
    
    i32 sum = calories_biggest[0] + calories_biggest[1] + calories_biggest[2];
    
    printf("%d\n",sum); 
    
    return 0;
}
