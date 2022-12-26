#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "../base_types.h"

#define STATE_CMD_CD 0
#define STATE_CMD_LS 1
#define STATE_LS_RETURN 2

#define TYPE_UNKNOWN 0
#define TYPE_DIR 1
#define TYPE_FILE 2

#define NODES_MAX 2048
#define TEXT_MAX 8192

// make a linked list
typedef struct Node Node;
struct Node{
    string8 name;
    u8 type;
    u64 size; // if directory, size of recursive contents
    Node* parent;
};

// globals for all the nodes.. why not
Node* nodes;
u64 node_qty;
u8* text; // for long term string storage
u8* text_free;


string8 string8_from_char(char* cstring){
    char* end = memchr(cstring, 0, 256); // find the hend of the string
    assert(end != NULL);
    
    u64 len = end - cstring;
    memcpy(text_free, cstring, len);
    
    string8 result;
    result.str = text_free;
    result.size= len;
    
    text_free += len;
    return result;
}


// for each file node added, recuse up a parent tree and add sizes
// then just go through the entire list and add up directories that meet the conditions

Node* node_create(Node* parent, string8 child_name){
    Node* new = nodes + node_qty;
    node_qty++;
    
    new->parent = parent;
    new->type = TYPE_UNKNOWN;
    new->size = 0;
    new->name = child_name;
    
    return new;
}


Node* node_find_child(Node* current, string8 child_name){
    // Perform an incredibly inefficient linear search to match names to nodes.
    // return NULL if child is not found
    // This would be done better by a hash table lookup.
    
    Node* child = NULL;
    
    // look for a node with the correct name
    // and the correct parent
    for (u64 i=0; i<node_qty; ++i){
        
        Node* node = nodes+i;
        if (node->parent == current){
            if (string8_compare(child_name, node->name) == STRING8_SAME){
                child = node;
                break;
            }
        }
    }
    
    return child;
}


int main(int argc, char *argv[]){
    
    nodes = malloc(NODES_MAX * sizeof(Node));
    node_qty = 0;
    text = malloc(TEXT_MAX);
    text_free = text;
    
    // start on a root node
    nodes[0].type = TYPE_DIR;
    nodes[0].size = 0;
    nodes[0].parent = NULL;
    Node* pwd  = nodes; // init current location to root
    node_qty++;
    
    
    // read in the file
    char filename[64];
    sscanf(argv[1], "%s", filename);
    FILE* fd;
    fd = fopen(filename, "r");
    assert(fd != NULL);
    
    // 
    char line_raw[256];
    char* fgets_status;
    fgets_status = fgets(line_raw, sizeof(line_raw), fd); // skip 1st line
    assert(fgets_status != NULL);
    
    u8 keep_going = TRUE;
    
    // assume the first line is a command
    u64 state = STATE_CMD_CD;
    
    while (keep_going == TRUE){
        // get a line
        fgets_status = fgets(line_raw, sizeof(line_raw), fd);
        if (fgets_status == NULL){
            break;
        }
        
        // detect state changes based on the current line
        if (line_raw[0] == '$'){ // line is command
            if (line_raw[2] == 'c'){
                state = STATE_CMD_CD;
            }
            else if (line_raw[2] == 'l'){
                state = STATE_CMD_LS; // a do-nothing state
            }
            else{
                printf("Line '%s' has unrecognized command\n", line_raw);
                assert(FALSE);
            }
        }
        else{
            state = STATE_LS_RETURN;
        }
        
        // then parse
        if (state == STATE_CMD_CD){
            // change directory command. parse cd argument
            // go only one level up or down at a time (except the first one)
            if (line_raw[5] == '.'){
                // go up a level
                pwd = pwd->parent;
            }
            else{
                // go down a level
                // separate out the target string
                
                string8 child_name;
                child_name.str = (u8*) line_raw + 5;
                child_name.size = (u8*) memchr(child_name.str, 0, 256) - child_name.str;
                pwd = node_find_child(pwd, child_name);
            }
        }
        else if (state == STATE_LS_RETURN){
            // ls command
            // parse contents; read subsequent lines!
            // create directories if they don't exist..
            // create files if they don't exist..
            if (line_raw[0] == 'd'){ // create directory
                // dir 
                // start name at line_raw[4]
                // create nodes 
                
                printf("detected directory '%s' \n", line_raw+4);
                string8 new_name = string8_from_char(line_raw+4);  // TODO wasted if the node already exists..
                
                if (node_find_child(pwd, new_name) == NULL){
                    Node* new_node = node_create(pwd, new_name);
                    new_node->type = TYPE_DIR;
                }
                
            }
            else{ // create file, handle size
                // sscanf("%d %s\n");
                u64 new_size;
                char new_name_raw[256];
                
                sscanf(line_raw, "%lu %s\n", &new_size, new_name_raw);
                printf("detected file '%s' with size %lu\n", new_name_raw, new_size);
                
                // make a string8
                string8 new_name = string8_from_char(new_name_raw); // TODO wasted if the node already exists..
                
                // check the node doesn't already exist..
                if (node_find_child(pwd, new_name) == NULL){
                    
                    // make a node for that the filename. parent is pwd
                    Node* new_node = node_create(pwd, new_name);
                    
                    // assign the size to that node
                    new_node->size = new_size;
                    new_node->type = TYPE_FILE;
                    
                    // recurse up the directory structure to accumulate size
                    for (Node* parent = new_node->parent; parent != NULL; parent = parent->parent){
                        parent->size += new_node->size;
                    }
                }
            }
        }
        
    }
    
    
    fclose(fd);
    
    // search for directories that meet the score criteria and add them
    u64 current_used_space = nodes[0].size;
    u64 current_free_space = 70000000 - current_used_space;
    u64 reduction_required = 30000000 - current_free_space;
    
    u64 final_score =  70000000;
    
    for (u64 i=0; i<node_qty; ++i){
        Node* node = nodes + i;
        if (node->type == TYPE_DIR){
            if (node->size > reduction_required){
                if (node->size < final_score){
                    final_score = node->size;
                }
            }
        }
    }
    
    
    printf("%lu\n",final_score); 
    
    return 0;
}
