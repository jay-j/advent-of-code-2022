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
    
    // compare the reference cell to others
    i64 score_best = 0;
    for (i64 row_ref=1; row_ref<rows; ++row_ref){
        for (i64 col_ref=1; col_ref<columns; ++col_ref){
            u8 current = map[columns*row_ref + col_ref];
            i64 view_distance[4] = {0};
            
            // look down below the reference cell
            for (i64 row=row_ref+1; row<rows; ++row){
                if (map[columns*row + col_ref] < current){
                    view_distance[0] += 1;
                }
                else if (map[columns*row + col_ref] >= current){
                    view_distance[0] += 1;
                    break;
                }
            }
            
            // look up from the reference cell
            for (i64 row=row_ref-1; row>=0; --row){
                if (map[columns*row + col_ref] < current){
                    view_distance[1] += 1;
                }
                else if (map[columns*row + col_ref] >= current){
                    view_distance[1] += 1;
                    break;
                }
            }
            
            // look right from the reference cell
            for (i64 col=col_ref+1; col<columns; ++col){
                if (map[columns*row_ref + col] < current){
                    view_distance[2] += 1;
                }
                else if (map[columns*row_ref + col] >= current){
                    view_distance[2] += 1;
                    break;
                }
            }
            
            // look left from the reference cell
            for (i64 col=col_ref-1; col>=0; --col){
                if (map[columns*row_ref + col] < current){
                    view_distance[3] += 1;
                }
                else if (map[columns*row_ref + col] >= current){
                    view_distance[3] += 1;
                    break;
                }
            }
            
            i64 score = view_distance[0] * view_distance[1] * view_distance[2] * view_distance[3];
            
            if (score > score_best){
                score_best = score;
            } 
            
        }
    }
    
    printf("%ld\n",score_best); 
    
    free(map);
    
    return 0;
}
