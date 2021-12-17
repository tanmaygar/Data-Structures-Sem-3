#ifndef BST_MEMORY_H_INCLUDED
#define BST_MEMORY_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>
#define COORD 2
#define FAST 1
#define DISK 10
#define MEM_CELL_T 10
#define MEM_ROW_N 3

//Array for storing whether that particular memory address is filled or not
//bool memory_bool_bst[MEM_ROW_N][2 * MEM_CELL_T] = {{false}};
extern bool memory_bool_bst[MEM_ROW_N][2 * MEM_CELL_T];

//Structure for our BST node with extra information which is the coordinates of the node in the memory
struct BST_mem_node
{
    int value;
    struct BST_mem_node *left;
    struct BST_mem_node *right;
    int coord[COORD];
};

//Function to allocate memory to the node and initilaize it with the given key
struct BST_mem_node *new_BST_mem_node(int key);

//Function to find and empty spot in a particular row of memory_bool
int find_next_empty_row(int *secondary_memory);

//Function to find the location of the empty cell for a given row of secondary memory
int find_next_empty_cell(int row_number);

//Function to read disk for 2 node based on each SITUATION
void time_adder_DISK_READ_2_nodes_BST(struct BST_mem_node *x, struct BST_mem_node *y, int *time);

//Function to search for a key in a given BST
struct BST_mem_node *search_mem_node(struct BST_mem_node *node, int key, int *time);

//Function to insert a key into a BST
struct BST_mem_node *insert_node_BST_mem(struct BST_mem_node **node, int key, int *time, int parent_coord[COORD], int *secondary_memory);

//Function to find the next element to fill the gaps when a partiuclar node needs to be deleted
struct BST_mem_node *find_next_deletion_BST_mem(struct BST_mem_node *node, int *time, int parent_coord[COORD]);

//Function to delete a node from BST
struct BST_mem_node *delete_node_BST_mem(struct BST_mem_node *node, int key, int *time, int parent_coord[COORD], int *secondary_memory);

#endif