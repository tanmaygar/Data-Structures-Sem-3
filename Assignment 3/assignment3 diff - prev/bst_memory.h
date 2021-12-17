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
bool memory_bool_bst[MEM_ROW_N][2 * MEM_CELL_T] = {{false}};

//Structure for our BST node with extra information which is the coordinates of the node in the memory
struct BST_mem_node
{
    int value;
    struct BST_mem_node *left;
    struct BST_mem_node *right;
    int coord[COORD];
};

//Function to allocate memory to the node and initilaize it with the given key
struct BST_mem_node *new_BST_mem_node(int key)
{
    struct BST_mem_node *new_node = (struct BST_mem_node *)malloc(sizeof(struct BST_mem_node));
    new_node->value = key;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->coord[0] = 0;
    new_node->coord[1] = 0;
    return new_node;
}

//Function to find and empty spot in a particular row of memory_bool
int find_next_empty_row(int *secondary_memory)
{
    int i = 0;
    while (secondary_memory[i] >= 2 * MEM_CELL_T - 1)
    {
        i++;
    }
    return i;
}

//Function to find the location of the empty cell for a given row of secondary memory
int find_next_empty_cell(int row_number)
{
    for (int i = 0; i < 2 * MEM_CELL_T; i++)
    {
        if (memory_bool_bst[row_number][i] == false)
        {
            return i;
        }
    }
}

//Function to search for a key in a given BST
struct BST_mem_node *search_mem_node(struct BST_mem_node *node, int key, int *time)
{
    //If it is found then returns pointer to the node
    if (key == node->value)
    {
        return node;
    }
    //If key is less than value of node it recurses to left subtree
    else if (key < node->value)
    {
        //IF left subtree is not NULL then it performs time increament operation
        if (node->left != NULL)
        {
            //If first coordinate of left child and node is same then it is in same row and FAST read will take place
            if (node->coord[0] == node->left->coord[0])
            {
                (*time) = (*time) + FAST;
                return search_mem_node(node->left, key, time);
            }
            //Else it is in different row and DISK read will take place and then FAST read to access the cell
            else
            {
                (*time) = (*time) + DISK + FAST;
                return search_mem_node(node->left, key, time);
            }
        }
        //Else the key is not present
        else
        {
            return NULL;
        }
    }
    //If key is greater than value of node it recurses to right subtree
    else if (key > node->value)
    {   
        //IF right subtree is NULL then key not present
        if (node->right == NULL)
        {
            return NULL;
        }
        //IF right subtree is not NULL then it performs time increament operation
        //If first coordinate of right child and node is same then it is in same row and FAST read will take place
        if (node->coord[0] == node->right->coord[0])
        {
            (*time) = (*time) + FAST;
            return search_mem_node(node->right, key, time);
        }
        //Else it is in different row and DISK read will take place and then FAST read to access the cell
        else
        {
            (*time) = (*time) + DISK + FAST;
            return search_mem_node(node->right, key, time);
        }
    }
    //If key is not found then returns NULL
    else
    {
        return NULL;
    }
}

//Function to insert a key into a BST
struct BST_mem_node *insert_node_BST_mem(struct BST_mem_node **node, int key, int *time, int parent_coord[COORD], int *secondary_memory)
{
    //Insertion into a node when it is NULL
    if ((*node) == NULL)
    {
        //Allocate memory to the new node
        (*node) = new_BST_mem_node(key);

        //If the first coordinate of parent node has an empty space then it adds it to that row and performs necessary operations
        if (secondary_memory[parent_coord[0]] < 2 * MEM_CELL_T - 1)
        {
            //Make first coordinate of parent node and new node same
            (*node)->coord[0] = parent_coord[0];
            //Make second coordinate as whatever emoty cell is available in that row
            (*node)->coord[1] = find_next_empty_cell(parent_coord[0]);

            //Increament the number of cells filled for that row
            ++secondary_memory[parent_coord[0]];

            //Make the cell filled
            memory_bool_bst[(*node)->coord[0]][(*node)->coord[1]] = true;

            //Do FAST for accessing that particular cell and then form DISK WRITE for saving
            (*time) = (*time) + FAST + DISK;
        }
        //If the first coordinate of parent node does not have an empty space then it adds it to the next empty row and performs necessary operations
        else
        {
            //Make first coordinate of new node as whatever row is has an empty space
            (*node)->coord[0] = find_next_empty_row(secondary_memory);
            //Make second coordinate as whatever emoty cell is available in that row
            (*node)->coord[1] = find_next_empty_cell((*node)->coord[0]);

            //Increament the number of cells filled for that row
            ++secondary_memory[(*node)->coord[0]];

            //Make the cell filled
            memory_bool_bst[(*node)->coord[0]][(*node)->coord[1]] = true;

            //Do FAST for accessing that particular cell and DISK READ for loading that row and then form 2 DISK WRITES for saving
            (*time) = (*time) + FAST + DISK + DISK + DISK;
        }
    }
    //If key value is less than node value then it recurses into the left subtree to find emty node
    else if (key < (*node)->value)
    {
        //If left child is NULL then it recurses to insert directly
        if ((*node)->left == NULL)
        {
            (*node)->left = insert_node_BST_mem(&(*node)->left, key, time, (*node)->coord, secondary_memory);
        }
        //If left child is not NULL then it recurses to insert into left subtree
        else if ((*node)->coord[0] == (*node)->left->coord[0])
        {
            //If first coordinate of left child and node is same then it is in same row and FAST read will take place
            (*time) = (*time) + FAST;
            (*node)->left = insert_node_BST_mem(&(*node)->left, key, time, (*node)->coord, secondary_memory);
        }
        else
        {
            //Else it is in different row and DISK read will take place and then FAST read to access the cell
            (*time) = (*time) + DISK + FAST;
            (*node)->left = insert_node_BST_mem(&(*node)->left, key, time, (*node)->coord, secondary_memory);
        }
    }
    //If key value is less than node value then it recurses into the left subtree to find emty node
    else
    {
        //If right child is NULL then it recurses to insert directly
        if ((*node)->right == NULL)
        {
            (*node)->right = insert_node_BST_mem(&(*node)->right, key, time, (*node)->coord, secondary_memory);
        }
        //If right child is not NULL then it recurses to insert into left subtree
        else if ((*node)->coord[0] == (*node)->right->coord[0])
        {
            //If first coordinate of right child and node is same then it is in same row and FAST read will take place
            (*time) = (*time) + FAST;
            (*node)->right = insert_node_BST_mem(&(*node)->right, key, time, (*node)->coord, secondary_memory);
        }
        else
        {
            //Else it is in different row and DISK read will take place and then FAST read to access the cell
            (*time) = (*time) + DISK + FAST;
            (*node)->right = insert_node_BST_mem(&(*node)->right, key, time, (*node)->coord, secondary_memory);
        }
    }

    //Return pointer to the node after insertion
    return (*node);
}

//Function to find the next element to fill the gaps when a partiuclar node needs to be deleted
struct BST_mem_node *find_next_deletion_BST_mem(struct BST_mem_node *node, int *time, int parent_coord[COORD])
{
    if (node == NULL)
    {
        return NULL;
    }
    struct BST_mem_node *tmp = node;
    int tmp_coord[COORD] = {parent_coord[0], parent_coord[1]};
    while (tmp->left != NULL)
    {
        //If current node and parent node has same coordinates then FAST to access the cell
        if (tmp->coord[0] == tmp_coord[0])
        {
            (*time) = (*time) + FAST;
        }
        //Else it is in separate row and perform DISK READ and then FAST to access element
        else
        {
            (*time) = (*time) + DISK + FAST;
        }
        tmp_coord[0] = tmp->coord[0];
        tmp_coord[1] = tmp_coord[1];
        tmp = tmp->left;
    }
    return tmp;
}

//Function to delete a node from BST
struct BST_mem_node *delete_node_BST_mem(struct BST_mem_node *node, int key, int *time, int parent_coord[COORD], int *secondary_memory)
{
    //If node is NULL then key is not present
    if (node == NULL)
    {
        return NULL;
    }
    //If key value is less than node value it recurses to left subtree
    if (key < node->value)
    {
        //If first coordinate of parent node and curent node is same then then it performs FAST to access the cell
        if (parent_coord[0] == node->coord[0])
        {
            (*time) = (*time) + FAST;
            node->left = delete_node_BST_mem(node->left, key, time, node->coord, secondary_memory);
        }
        //Else it performs DISK READ and then FAST to access the cell
        else
        {
            (*time) = (*time) + DISK + FAST;
            node->left = delete_node_BST_mem(node->left, key, time, node->coord, secondary_memory);
        }
    }
    //If key value is more than node value it recurses to right subtree
    else if (key > node->value)
    {
        //If first coordinate of parent node and curent node is same then then it performs FAST to access the cell
        if (parent_coord[0] == node->coord[0])
        {
            (*time) = (*time) + FAST;
            node->right = delete_node_BST_mem(node->right, key, time, node->coord, secondary_memory);
        }
        else
        {
            //Else it performs DISK READ and then FAST to access the cell
            (*time) = (*time) + DISK + FAST;
            node->right = delete_node_BST_mem(node->right, key, time, node->coord, secondary_memory);
        }
    }
    //Else node value is equal to the key
    else
    {   
        //Creating a tmp pointer
        struct BST_mem_node *tmp;

        //If node does not have children then performs necessary time operation and deletion
        if (node->left == NULL && node->right == NULL)
        {
            tmp = NULL;

            //Decrement the number of element of cells filled for the row which is filled with this node
            --secondary_memory[node->coord[0]];

            //Empty the cell
            memory_bool_bst[(node)->coord[0]][(node)->coord[1]] = false;

            //If parent node and current node lie in same row then perform single DISK WRITE
            if(parent_coord[0] == node->coord[0])
            {
                (*time) = (*time) + DISK;
            }
            //Else perform 2 DISK WRITES
            else
            {
                (*time) = (*time) + DISK + DISK;
            }
            node = NULL;

            //return the tmp pointer for updation
            return tmp;
        }
        //If left child is NULL then performs necessary time operation and deletion
        else if (node->left == NULL)
        {
            //If right child and current node lie in same row then FAST to access the cell
            if (parent_coord[0] == node->right->coord[0])
            {
                (*time) = (*time) + FAST;
                //(*time) = (*time) + DISK;
            }
            //ELSE it performs DISK READ and FAST to access the cell
            else
            {
                (*time) = (*time) + DISK + FAST;
                (*time) = (*time) + DISK;   //This is the DISK WRITE for the parent row
            }
            //Make tmp as right child and then return it for updation
            tmp = node->right;

            //Decrement the number of element of cells filled for the row which is filled with this node
            --secondary_memory[node->coord[0]];

            //Empty the cell
            memory_bool_bst[(node)->coord[0]][(node)->coord[1]] = false;
            node = NULL;

            //DISK WRITE for the current row in which the node belongs to
            (*time) = (*time) + DISK;

            return tmp;
        }
        //If right child is NULL then performs necessary time operation and deletion
        else if (node->right == NULL)
        {
            //If left child and current node lie in same row then FAST to access the cell
            if (parent_coord[0] == node->left->coord[0])
            {
                (*time) = (*time) + FAST;
            }
            //ELSE it performs DISK READ and FAST to access the cell
            else
            {
                (*time) = (*time) + DISK + FAST;
                (*time) = (*time) + DISK;   //This is the DISK WRITE for the parent row
            }
            //Make tmp as left child and then return it for updation
            tmp = node->left;

            //Decrement the number of element of cells filled for the row which is filled with this node
            --secondary_memory[node->coord[0]];

            //Empty the cell
            memory_bool_bst[(node)->coord[0]][(node)->coord[1]] = false;
            node = NULL;

            //DISK WRITE for the current row in which the node belongs to
            (*time) = (*time) + DISK;
            return tmp;
        }
        //If both child are not NULL
        else
        {
            //Find successor in the right child
            struct BST_mem_node *new_tmp = find_next_deletion_BST_mem(node->right, time, node->coord);

            //Make the value of current node same as the successor
            node->value = new_tmp->value;

            //DISK WRITE for the current row 
            (*time) = (*time) + DISK;

            //Recurses to the right child to perform deletion operation on successor node
            node->right = delete_node_BST_mem(node->right, new_tmp->value, time, node->coord, secondary_memory);
        }
    }
    return node;
}
#endif
