#ifndef BTREE_MEMORY_H_INCLUDED
#define BTREE_MEMORY_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>
#define NUM_KEYS 3
#define MINIMUM_DEGREE 2
#define COORD 2
#define FAST_BTREE 1
#define DISK 10
#define MEM_CELL_T_BTREE 10
#define MEM_ROW_N_BTREE 3

//Array for storing whether that particular memory address is filled or not
//bool memory_bool_btree[MEM_ROW_N_BTREE][2 * MEM_CELL_T_BTREE] = {{false}};
extern bool memory_bool_btree[MEM_ROW_N_BTREE][2 * MEM_CELL_T_BTREE];

//Function to find and empty spot in a particular row of memory_bool
int find_next_empty_row_BTREE(int *secondary_memory);

//Function to find the location of the empty cell for a given row of secondary memory
int find_next_empty_cell_BTREE(int row_number);

//Structure for our BTREE node with extra information which is the coordinates of the node in the memory
struct BTREE_node
{
    bool leaf;
    int num_keys;
    int key[NUM_KEYS];
    struct BTREE_node *children[NUM_KEYS + 1];
    int coord[COORD];
};

//Function to allocate memory for new node of BTREE and initialize the required values
struct BTREE_node *new_BTREE_node();

//Function to read disk for 2 node based on each SITUATION
void time_adder_DISK_READ_2_nodes(struct BTREE_node *x, struct BTREE_node *y, int *time);

//Function to write disk for 3 nodes based on each SITUATION
void time_adder_DISK_WRITE_3_nodes(struct BTREE_node *x, struct BTREE_node *y, struct BTREE_node *z, int *time);

//Function to read disk for 3 nodes based on each SITUATION
void time_adder_DISK_READ_3_nodes(struct BTREE_node *x, struct BTREE_node *y, struct BTREE_node *z, int *time);

//Function to initialize empty parts of array of key of a node as 0
void make_rest_keys_zero(struct BTREE_node *x);

//Function to initialize empty parts of array of children of a node as NULL
void make_rest_children_null(struct BTREE_node *x);

//Function to search for a key in the BTREE
struct BTREE_node *search_BTREE_node(struct BTREE_node *node, int input_key, int *time);

//Function to split the node into required parts
void BTREE_node_split_child(struct BTREE_node *node, int index, int *time, int *secondary_memory);

//Function to insert key in a node which is not full
void BTREE_insert_non_full(struct BTREE_node *node, int num, int *time, int *secondary_memory);

//Function to insert key into a BTREE
void insert_node_BTREE(struct BTREE_node **node, int num, int *time, int parent_coord[COORD], int *secondary_memory);

//Function to find the predecessor of a particular node
int find_predecessor(struct BTREE_node *node, int *time);

//Function to find the successor of a particular node
int find_successor(struct BTREE_node *node, int *time);

//Function to delete a key from the node
void delete_node_BTREE(struct BTREE_node **node, int num, int *time, int *secondary_memory);
#endif // BTREE_H_INCLUDED