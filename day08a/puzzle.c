#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "../base_types.h"

#define LINE_INPUT_MAX 256
#define MAP_SIZE_MAX 16384

int main(int argc, char *argv[]){
    
    // index: columns*row + column
    u8* map = malloc(MAP_SIZE_MAX);
    u8* vis_from_top = malloc(MAP_SIZE_MAX);
    u8* vis_from_bottom = malloc(MAP_SIZE_MAX);
    u8* vis_from_left = malloc(MAP_SIZE_MAX);
    u8* vis_from_right = malloc(MAP_SIZE_MAX);
    u8* map_head = map;
    
    // read in the file
    char filename[64];
    sscanf(argv[1], "%s", filename);
    FILE* fd;
    fd = fopen(filename, "r");
    assert(fd != NULL);
    
    // get the first line
    char line_raw[LINE_INPUT_MAX];
    char* fgets_status;
    fgets_status = fgets(line_raw, sizeof(line_raw), fd);
    assert(fgets_status != NULL);
    
    // figure out grid column qty
    char* end = memchr(line_raw, '\n', LINE_INPUT_MAX);
    u64 columns = end - line_raw; // TODO double check for off by one errors
    u64 rows = 0;
    
    while (fgets_status != NULL){
        // read in the map, don't store in the endlines
        memcpy(map_head, line_raw, columns);
        map_head += columns;
        
        // store the grid row qty
        rows++;
        
        fgets_status = fgets(line_raw, sizeof(line_raw), fd);
    }
    
    fclose(fd);
    
    // make four maps (visibility from top, from bottom, from left, from right)
    // TODO criteria is taller than previous maximum
    
    // visible from top. go down each column
    for (i64 col=0; col<columns; ++col){
        // mark exterior as visible
        vis_from_top[columns*0 + col] = TRUE;
        u8 tallest = map[columns*0 + col];
        
        // check interior
        for (i64 row=1; row<rows; ++row){
            vis_from_top[columns*row + col] = FALSE;
            
            if (map[columns*row + col] > tallest){
                vis_from_top[columns*row + col] = TRUE;
                tallest = map[columns*row + col];
            }
        }
    }
    
    // visible from bottom. go down each column
    for (i64 col=0; col<columns; ++col){
        // mark exterior as visible
        vis_from_bottom[columns*(rows-1) + col] = TRUE;
        u8 tallest = map[columns*(rows-1) + col];
        
        // check interior
        for (i64 row=rows-2; row>=0; --row){
            vis_from_bottom[columns*row + col] = FALSE;
            if (map[columns*row + col] > tallest){
                vis_from_bottom[columns*row + col]= TRUE;
                tallest = map[columns*row + col];
            }
        }
    }
    
    // visible from left. go across each row
    for (i64 row=0; row<rows; ++row){
        // mark exterior as visible
        vis_from_left[columns*row + 0] = TRUE;
        u8 tallest = map[columns*row + 0];
        
        // check interior
        for (i64 col=1; col<columns; ++col){
            vis_from_left[columns*row + col] = FALSE;
            
            if (map[columns*row + col] > tallest){
                vis_from_left[columns*row + col]= TRUE;
                tallest = map[columns*row + col];
            }
        }
    }
    
    // visible from right. go across each row
    for (i64 row=0; row<rows; ++row){
        // mark exterior as visible
        vis_from_right[columns*row +(columns-1)] = TRUE;
        u8 tallest = map[columns*row +(columns-1)];
        
        // check interior
        for (i64 col=columns-2; col>=0; --col){
            vis_from_right[columns*row + col] = FALSE;
            
            if (map[columns*row + col] > tallest){
                vis_from_right[columns*row + col]= TRUE;
                tallest = map[columns*row + col];
            }
        }
    }
    
    
    // then OR the maps to see what trees are totally unviewable
    i32 final_score = 0;
    
    for (i64 row=0; row<rows; ++row){
        for (i64 col=0; col<columns; ++col){
            i64 index = columns*row + col;
            if ((vis_from_top[index] == TRUE) ||
                (vis_from_bottom[index] == TRUE) || 
                (vis_from_left[index] == TRUE) ||
                (vis_from_right[index] == TRUE)){
                final_score++;
                printf("X");
            }
            else{
                printf("-");
            }
        }
        printf("\n");
    }
    
    printf("%d\n",final_score); 
    
    
    free(map);
    free(vis_from_top);
    free(vis_from_bottom);
    free(vis_from_left);
    free(vis_from_right);
    
    return 0;
}
