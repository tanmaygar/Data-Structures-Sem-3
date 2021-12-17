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
#define FAST 1
#define DISK 10
#define MEM_CELL_T 10
#define MEM_ROW_N 3

//Array for storing whether that particular memory address is filled or not
bool memory_bool_btree[MEM_ROW_N][2 * MEM_CELL_T] = {{false}};

//Function to find and empty spot in a particular row of memory_bool
int find_next_empty_row_BTREE(int *secondary_memory)
{
    int i = 0;
    while (secondary_memory[i] >= 2 * MEM_CELL_T - 1)
    {
        i++;
    }
    return i;
}

//Function to find the location of the empty cell for a given row of secondary memory
int find_next_empty_cell_BTREE(int row_number)
{
    for (int i = 0; i < 2 * MEM_CELL_T; i++)
    {
        if (memory_bool_btree[row_number][i] == false)
        {
            return i;
        }
    }
}

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
struct BTREE_node *new_BTREE_node()
{
    struct BTREE_node *new_node = (struct BTREE_node *)malloc(sizeof(struct BTREE_node));
    new_node->leaf = true;
    new_node->num_keys = 0;
    for (int i = 0; i < NUM_KEYS + 1; i++)
    {
        new_node->children[i] = NULL;
    }
    for (int i = 0; i < NUM_KEYS; i++)
    {
        new_node->key[i] = 0;
    }
    new_node->coord[0] = 0;
    new_node->coord[1] = 0;
    return new_node;
}

void time_adder_DISK_READ_2_nodes(struct BTREE_node *x, struct BTREE_node *y, int *time)
{
    if (x->coord[0] == y->coord[0])
    {
        (*time) = (*time) + FAST;
    }
    else
    {
        (*time) = (*time) + DISK + FAST;
    }
}

void time_adder_DISK_WRITE_3_nodes(struct BTREE_node *x, struct BTREE_node *y, struct BTREE_node *z, int *time)
{
    if ((x->coord[0] != y->coord[0]) && (x->coord[0] != z->coord[0]) && (z->coord[0] != y->coord[0]))
    {
        (*time) = (*time) + DISK + DISK + DISK;
    }
    else if ((x->coord[0] == y->coord[0]) && (x->coord[0] == z->coord[0]) && (z->coord[0] == y->coord[0]))
    {
        (*time) = (*time) + DISK;
    }
    else
    {
        (*time) = (*time) + DISK + DISK;
    }
}

void time_adder_DISK_READ_3_nodes(struct BTREE_node *x, struct BTREE_node *y, struct BTREE_node *z, int *time)
{
    if (x->coord[0] != y->coord[0] && x->coord[0] != z->coord[0] && y->coord[0] != z->coord[0])
    {
        (*time) = (*time) + DISK + DISK + FAST + FAST;
    }
    else if (x->coord[0] == y->coord[0] && x->coord[0] == z->coord[0] && y->coord[0] == z->coord[0])
    {
        (*time) = (*time) + FAST + FAST;
    }
    else
    {
        (*time) = (*time) + DISK + FAST + FAST;
    }
}

//Function to search for a key in the BTREE
struct BTREE_node *search_BTREE_node(struct BTREE_node *node, int input_key, int *time)
{
    int i = 0;
    //Searching for the key in the given node
    while (i < node->num_keys && input_key > node->key[i])
    {
        i++;
    }
    //If key is equal to the key of the node at that position then it returns the pointer to the node
    if (i < node->num_keys && input_key == node->key[i])
    {
        (*time) = (*time) + FAST;
        return node;
    }
    //else if the node is a leaf then the key is not present and it returns NULL
    else if (node->leaf)
    {
        return NULL;
    }
    //Else it proceeds to the appropriate child of the node
    else
    {
        time_adder_DISK_READ_2_nodes(node, node->children[i], time);
        //Recurses to the particular child to search for the key
        return search_BTREE_node(node->children[i], input_key, time);
    }
}

//Function to split the node into required parts
void BTREE_node_split_child(struct BTREE_node *node, int index, int *time, int *secondary_memory)
{
    //Assign new node z
    struct BTREE_node *z = new_BTREE_node();

    //y is the child needed to be split
    struct BTREE_node *y = node->children[index];
    z->leaf = y->leaf;
    z->num_keys = MINIMUM_DEGREE - 1;

    //Transfering keys from y to z after crossing MINIMUM_DEGREE
    for (int j = 0; j < MINIMUM_DEGREE - 1; j++)
    {
        z->key[j] = y->key[j + MINIMUM_DEGREE];
    }
    //Putting the rest of the keys keys as 0
    for (int j = MINIMUM_DEGREE - 1; j < 2 * MINIMUM_DEGREE - 1; j++)
    {
        z->key[j] = 0;
    }
    //If y is not a leaf then transfer the reuqired children pointers also
    if (!(y->leaf))
    {
        for (int j = 0; j < MINIMUM_DEGREE; j++)
        {
            z->children[j] = y->children[j + MINIMUM_DEGREE];
        }
    }
    //Change the number of keys of y
    y->num_keys = MINIMUM_DEGREE - 1;
    //Shift the children pointer ahead by 1
    for (int j = node->num_keys; j > index; j--)
    {
        node->children[j + 1] = node->children[j];
    }
    //Initialize z as the next child after y
    node->children[index + 1] = z;
    for (int j = node->num_keys; j >= index; j--) //made changes
    {
        node->key[j + 1] = node->key[j];
    }
    node->key[index] = y->key[1];
    node->num_keys = node->num_keys + 1;
    //Putting the rest of keys of y as 0
    for (int j = 1; j < 2 * MINIMUM_DEGREE - 1; j++)
    {
        y->key[j] = 0;
    }
    //Putting rest children of y as NULL
    for (int j = MINIMUM_DEGREE; j < 2 * MINIMUM_DEGREE; j++)
    {
        y->children[j] = NULL;
    }

    //Initialize the coords for z
    //If in the row where node exists as additional free cell then put in that cell
    if (secondary_memory[node->coord[0]] < 2 * MEM_CELL_T - 1)
    {
        z->coord[0] = node->coord[0];
        z->coord[1] = find_next_empty_cell_BTREE(z->coord[0]);
        ++secondary_memory[z->coord[0]];
        memory_bool_btree[z->coord[0]][z->coord[1]] = true;

        //FAST READ for access to the cell
        (*time) = (*time) + FAST;
    }
    //Else put in the next available row with empty cell
    else
    {
        z->coord[0] = find_next_empty_row_BTREE(secondary_memory);
        z->coord[1] = find_next_empty_cell_BTREE(z->coord[0]);
        ++secondary_memory[z->coord[0]];
        memory_bool_btree[z->coord[0]][z->coord[1]] = true;

        //DISK READ and FAST READ for access to the cell
        (*time) = (*time) + FAST + DISK;
    }

    time_adder_DISK_WRITE_3_nodes(node, y, z, time);
}

//Function to insert key in a node which is not full
void BTREE_insert_non_full(struct BTREE_node *node, int num, int *time, int *secondary_memory)
{
    int i = node->num_keys - 1;
    //Shift the keys to the right to make space for incoming key
    if (node->leaf)
    {
        while (i >= 0 && num < node->key[i])
        {
            node->key[i + 1] = node->key[i];
            i--;
        }
        node->key[i + 1] = num;
        node->num_keys = node->num_keys + 1;

        //DISK WRITE OPERTION for making changes
        (*time) = (*time) + FAST + DISK;
    }
    else
    {
        //If node is not a leaf then proceeed to add it in the child
        while (i >= 0 && num < node->key[i])
        {
            i--;
        }
        i++;

        time_adder_DISK_READ_2_nodes(node, node->children[i], time);
        //If child is full then split the child to make room for incoming key
        if (node->children[i]->num_keys == 2 * MINIMUM_DEGREE - 1)
        {
            BTREE_node_split_child(node, i, time, secondary_memory);
            if (num > node->key[i])
            {
                i++;
            }
        }
        //Recurses to add key to the child
        BTREE_insert_non_full(node->children[i], num, time, secondary_memory);
    }
}

//Function to insert key into a BTREE
void insert_node_BTREE(struct BTREE_node **node, int num, int *time, int parent_coord[COORD], int *secondary_memory)
{
    struct BTREE_node *current = (*node);
    //If current node is full then proceed to split the node and insert the key
    if ((*node)->num_keys == 2 * MINIMUM_DEGREE - 1)
    {
        struct BTREE_node *new_node = new_BTREE_node();
        (*node) = new_node;
        new_node->leaf = false;
        new_node->num_keys = 0;
        new_node->children[0] = current;

        //If an empty cell is there in the row node exists then make the coordinates of new_node that empty cell
        if (secondary_memory[parent_coord[0]] < 2 * MEM_CELL_T - 1)
        {
            new_node->coord[0] = parent_coord[0];
            new_node->coord[1] = find_next_empty_cell_BTREE(parent_coord[0]);
            ++secondary_memory[parent_coord[0]];
            memory_bool_btree[new_node->coord[0]][new_node->coord[1]] = true;

            //FAST and DISK WRITE
            (*time) = (*time) + FAST + DISK;
        }
        //Else find the next possible row with an empty cell
        else
        {
            new_node->coord[0] = find_next_empty_row_BTREE(secondary_memory);
            new_node->coord[1] = find_next_empty_cell_BTREE(new_node->coord[0]);
            ++secondary_memory[new_node->coord[0]];
            memory_bool_btree[new_node->coord[0]][new_node->coord[1]] = true;

            //DISK READ AND FAST to access the cell and then DISK WRITE
            (*time) = (*time) + FAST + DISK + DISK;
        }
        //Calls split node and insert in non full node to insert the key
        BTREE_node_split_child(new_node, 0, time, secondary_memory);
        BTREE_insert_non_full(new_node, num, time, secondary_memory);
    }
    //If current node is not full then it calls insert not full to insert the row
    else
    {
        BTREE_insert_non_full(current, num, time, secondary_memory);
    }
}

//Function to find the predecessor of a particular node
int find_predecessor(struct BTREE_node *node, int *time)
{
    //If right most child of the node is not NULL then return the right most key
    if (node->children[node->num_keys] == NULL)
    {
        return node->key[node->num_keys - 1];
    }
    time_adder_DISK_READ_2_nodes(node, node->children[node->num_keys], time);
    //recurses to the right most child to find the predecessor of a particular node
    return find_predecessor(node->children[node->num_keys], time);
}

//Function to find the successor of a particular node
int find_successor(struct BTREE_node *node, int *time)
{
    //If left most child of the node is not NULL then return the left most key
    if (node->children[0] == NULL)
    {
        return node->key[0];
    }
    time_adder_DISK_READ_2_nodes(node, node->children[0], time);
    //recurses to the left most child to find the predecessor of a particular node
    return find_successor(node->children[0], time);
}

//Function to delete a key from the node
void delete_node_BTREE(struct BTREE_node **node, int num, int *time, int *secondary_memory)
{
    //CASE 1 WHEN NODE IS A LEAF
    if ((*node)->leaf)
    {
        int j = 0;
        //Find the index where the element exists
        while (j < (*node)->num_keys && num > (*node)->key[j])
        {
            j++;
        }
        //Shift all the keys to the left
        for (int i = j; i < (*node)->num_keys - 1; i++)
        {
            (*node)->key[i] = (*node)->key[i + 1];
        }
        //Reduce the number of keys
        (*node)->num_keys--;
        //Make the rest of the keys as 0
        for (int i = (*node)->num_keys; i < 2 * MINIMUM_DEGREE - 1; i++)
        {
            (*node)->key[i] = 0;
        }
        //DISK WRITE OPERTION
        (*time) = (*time) + DISK;
    }
    //ELSE WHEN NODE IS NOT A LEAF
    else
    {
        int j = 0;
        //Find the position where the key might lie
        while (j < (*node)->num_keys && num > (*node)->key[j])
        {
            j++;
        }
        //CASE 2 WHEN JTH INDEX OF THE KEYS OF THE NODE IS EQUAL TO THE GIVEN KEY
        if ((*node)->key[j] == num)
        {
            //CASE 2A WHEN THE LEFT CHILD OF THE KEY HAS ATLEAST T KEYS
            if ((*node)->children[j]->num_keys >= MINIMUM_DEGREE)
            {
                //Find predecessor of the node
                int pred = find_predecessor((*node)->children[j], time);
                time_adder_DISK_READ_2_nodes((*node), (*node)->children[j], time);

                //Replace the key with predecessor
                (*node)->key[j] = pred;
                //DISK WRITE OPERTION for this node
                (*time) = (*time) + DISK;
                //Recurses to delete the predecessor in the child node
                delete_node_BTREE(&(*node)->children[j], pred, time, secondary_memory);
            }
            //CASE 2B WHEN THE RIGHT CHILD OF THE KEY HAS ATLEAST T KEYS
            else if ((*node)->children[j + 1]->num_keys >= MINIMUM_DEGREE)
            {
                //Find successor of the node
                int succ = find_successor((*node)->children[j + 1], time);
                time_adder_DISK_READ_2_nodes((*node), (*node)->children[j + 1], time);

                //Replace the key with successor
                (*node)->key[j] = succ;
                //DISK WRITE OPERTION for this node
                (*time) = (*time) + DISK;
                //Recurses to delete the successor in the child node
                delete_node_BTREE(&(*node)->children[j + 1], succ, time, secondary_memory);
            }
            //CASE 2C WHEN THE BOTH CHILDREN HAVE T-1 KEYS ONLY
            else
            {
                struct BTREE_node *y = (*node)->children[j];
                time_adder_DISK_READ_3_nodes((*node), y, (*node)->children[j + 1], time);
                time_adder_DISK_WRITE_3_nodes((*node), y, (*node)->children[j + 1], time);

                //Combining the J+1th child to Jth child along with the key
                y->key[y->num_keys] = (*node)->key[j];
                for (int i = 0; i < (*node)->children[j + 1]->num_keys; i++)
                {
                    y->key[i + y->num_keys + 1] = (*node)->children[j + 1]->key[i];
                }

                for (int i = 0; i < (*node)->children[j + 1]->num_keys + 1; i++)
                {
                    y->children[i + MINIMUM_DEGREE] = (*node)->children[j + 1]->children[i];
                }

                y->num_keys = y->num_keys + 1 + (*node)->children[j + 1]->num_keys;
                //REMOVING THE J+1th child location as filled and making it empty
                --secondary_memory[(*node)->children[j + 1]->coord[0]];
                memory_bool_btree[(*node)->children[j + 1]->coord[0]][(*node)->children[j + 1]->coord[1]] = false;

                //Shifting the rest of the keys to the left
                for (int i = j; i < 2 * MINIMUM_DEGREE - 2; i++)
                {
                    (*node)->key[i] = (*node)->key[i + 1];
                }
                for (int i = j + 1; j < (*node)->num_keys; j++)
                {
                    (*node)->children[i] = (*node)->children[i + 1];
                }
                //Reducing the number of keys
                (*node)->num_keys--;
                for (int i = (*node)->num_keys; i < 2 * MINIMUM_DEGREE - 1; i++)
                {
                    (*node)->key[i] = 0;
                }
                for (int i = (*node)->num_keys + 1; i < 2 * MINIMUM_DEGREE; i++)
                {
                    (*node)->children[i] = NULL;
                }
                //Recurses to delete the node in the new child Jth child
                delete_node_BTREE(&y, num, time, secondary_memory);
                // //Combining the J+1th child to Jth child along with the key
                // y->key[y->num_keys] = (*node)->key[j];
                // for (int i = y->num_keys + 1; i < 2 * MINIMUM_DEGREE - 1; i++)
                // {
                //     y->key[i] = (*node)->children[j + 1]->key[i - y->num_keys - 1];
                // }

                // y->num_keys = 2 * MINIMUM_DEGREE - 1;

                // for (int i = y->num_keys - 1; i < 2 * MINIMUM_DEGREE; i++)
                // {
                //     y->children[i] = (*node)->children[j + 1]->children[i - MINIMUM_DEGREE];
                // }

                // //REMOVING THE J+1th child location as filled and making it empty
                // --secondary_memory[(*node)->children[j + 1]->coord[0]];
                // memory_bool_btree[(*node)->children[j + 1]->coord[0]][(*node)->children[j + 1]->coord[1]] = false;
                // (*node)->children[j + 1] = NULL;

                // //Shifting the rest of the keys to the left
                // for (int i = j; i < 2 * MINIMUM_DEGREE - 2; i++)
                // {
                //     (*node)->key[i] = (*node)->key[i + 1];
                // }
                // //Reducing the number of keys
                // (*node)->num_keys--;
                // //Making rest of the keys as 0
                // for (int i = (*node)->num_keys; i < 2 * MINIMUM_DEGREE - 1; i++)
                // {
                //     (*node)->key[i] = 0;
                // }

                // (*node)->children[j+1] = NULL;
                // for(int i = j; i < 2* MINIMUM_DEGREE -2; i++)
                // {
                //     (*node)->key[i] = (*node)->key[i+1];
                // }
                // for(int i = j + 1; i < 2* MINIMUM_DEGREE - 1; i++)
                // {
                //     (*node)->children[i] = (*node)->children[i+1];
                // }
                // (*node)->children[2*MINIMUM_DEGREE-1] = NULL;

                //Recurses to delete the node in the new child Jth child
                //delete_node_BTREE(&(*node)->children[j], num, time, secondary_memory);
            }
        }
        //CASE 3 WHEN THE KEY DOES NOT EXIST IN THE PARTICULAR NODE
        else
        {
            //IF CHILD HAS T KEYS THEN DELETE NORMALLY
            if ((*node)->children[j]->num_keys >= MINIMUM_DEGREE)
            {
                //if child and node lie in the same row then FAST READ to access the cell
                // if ((*node)->coord[0] == (*node)->children[j]->coord[0])
                // {
                //     (*time) = (*time) + FAST;
                // }
                // //Else DISK READ and FAST to access the cell
                // else
                // {
                //     (*time) = (*time) + FAST + DISK;
                // }
                time_adder_DISK_READ_2_nodes((*node), (*node)->children[j], time);
                //Recurses to delete the key in the child
                delete_node_BTREE(&(*node)->children[j], num, time, secondary_memory);
            }
            //IF THE KEY LIES IN THE LEFTMOST CHILD OF THE NODE
            else if (j == 0)
            {
                //IF the next child has atleast T KEYS then proceed with case 3A
                if ((*node)->children[1]->num_keys >= MINIMUM_DEGREE)
                {
                    //DISK READ OPERATION FOR BOTH THE CHILD NODES FOR EACH SITUATION
                    // if ((*node)->coord[0] != (*node)->children[1]->coord[0] && (*node)->coord[0] != (*node)->children[0]->coord[0] && (*node)->children[0]->coord[0] != (*node)->children[1]->coord[0])
                    // {
                    //     (*time) = (*time) + DISK + DISK + FAST + FAST;
                    //     (*time) = (*time) + DISK + DISK + DISK; //DISK WRITE FOR ALL THE THREE ROWS
                    // }
                    // else if ((*node)->coord[0] == (*node)->children[1]->coord[0] && (*node)->coord[0] == (*node)->children[0]->coord[0] && (*node)->children[0]->coord[0] == (*node)->children[1]->coord[0])
                    // {
                    //     (*time) = (*time) + FAST + FAST;
                    //     (*time) = (*time) + DISK; //DISK WRITE FOR THAT ROW
                    // }
                    // else
                    // {
                    //     (*time) = (*time) + DISK + FAST + FAST;
                    //     (*time) = (*time) + DISK + DISK; //DISK WRITE FOR THE TWO ROWS
                    // }
                    time_adder_DISK_READ_3_nodes((*node), (*node)->children[1], (*node)->children[0], time);
                    time_adder_DISK_WRITE_3_nodes((*node), (*node)->children[1], (*node)->children[0], time);

                    struct BTREE_node *y = (*node)->children[0];
                    //Shifting a key from the node down to J=0 and moving a key from J=1 to node
                    y->key[y->num_keys] = (*node)->key[0];
                    (*node)->key[0] = (*node)->children[1]->key[0];

                    //Shifting the keys to the left
                    for (int i = 0; i < (*node)->children[1]->num_keys - 1; i++)
                    {
                        (*node)->children[1]->key[i] = (*node)->children[1]->key[i + 1];
                    }
                    //Shifting the children to the left
                    for (int i = 0; i < (*node)->children[1]->num_keys; i++)
                    {
                        (*node)->children[1]->children[i] = (*node)->children[1]->children[i + 1];
                    }

                    //Decreament number of keys for J=1
                    (*node)->children[1]->num_keys--;
                    for (int i = (*node)->children[1]->num_keys; i < 2 * MINIMUM_DEGREE - 1; i++)
                    {
                        (*node)->children[1]->key[i] = 0;
                    }
                    for (int i = (*node)->children[1]->num_keys + 1; i < 2 * MINIMUM_DEGREE - 1; i++)
                    {
                        (*node)->children[1]->children[i] = NULL;
                    }

                    // //Shifting a key from the node down to J=0 and moving a key from J=1 to node
                    // (*node)->children[0]->key[1] = (*node)->key[0];
                    // (*node)->children[0]->num_keys++;
                    // (*node)->children[0]->children[2] = (*node)->children[1]->children[0];

                    // (*node)->key[0] = (*node)->children[1]->key[0];
                    // for (int i = 0; i < (*node)->children[1]->num_keys - 1; i++)
                    // {
                    //     (*node)->children[1]->key[i] = (*node)->children[1]->key[i + 1];
                    // }

                    // for (int i = 0; i < (*node)->children[1]->num_keys; i++)
                    // {
                    //     (*node)->children[1]->children[i] = (*node)->children[1]->children[i + 1];
                    // }

                    // //Decreament number of keys for J=1
                    // (*node)->children[1]->num_keys--;
                    // for (int i = (*node)->children[1]->num_keys; i < 2 * MINIMUM_DEGREE - 1; i++)
                    // {
                    //     (*node)->children[1]->key[i] = 0;
                    // }
                    // for (int i = (*node)->children[1]->num_keys + 1; i < 2 * MINIMUM_DEGREE - 1; i++)
                    // {
                    //     (*node)->children[1]->children[i] = NULL;
                    // }

                    //Recurses to delete the key in J=0
                    delete_node_BTREE(&(*node)->children[0], num, time, secondary_memory);
                }
                else
                {
                    //IF J=0 and J=1 BOTH HAVE ONLY T-1 KEYS THEN MERGE AND DELETE FROM COMBINED NODES AND PROCEED WITH CASE 3B
                    //DISK READ OPERTION FOR EACH SITUATION
                    // if ((*node)->coord[0] != (*node)->children[1]->coord[0] && (*node)->coord[0] != (*node)->children[0]->coord[0] && (*node)->children[0]->coord[0] != (*node)->children[1]->coord[0])
                    // {
                    //     (*time) = (*time) + DISK + DISK + FAST + FAST;
                    //     (*time) = (*time) + DISK + DISK + DISK; //DISK WRITE FOR ALL THE THREE ROWS
                    // }
                    // else if ((*node)->coord[0] == (*node)->children[1]->coord[0] && (*node)->coord[0] == (*node)->children[0]->coord[0] && (*node)->children[0]->coord[0] == (*node)->children[1]->coord[0])
                    // {
                    //     (*time) = (*time) + FAST + FAST;
                    //     (*time) = (*time) + DISK; //DISK WRITE FOR THAT ROW
                    // }
                    // else
                    // {
                    //     (*time) = (*time) + DISK + FAST + FAST;
                    //     (*time) = (*time) + DISK + DISK; //DISK WRITE FOR THE TWO ROWS
                    // }
                    time_adder_DISK_READ_3_nodes((*node), (*node)->children[1], (*node)->children[0], time);
                    time_adder_DISK_WRITE_3_nodes((*node), (*node)->children[1], (*node)->children[0], time);


                    // (*node)->children[0]->key[1] = (*node)->key[0];
                    // (*node)->children[0]->key[2] = (*node)->children[1]->key[0];

                    struct BTREE_node *y = (*node)->children[0];
                    y->key[y->num_keys] = (*node)->key[0];
                    for (int i = 0; i < (*node)->children[1]->num_keys; i++)
                    {
                        y->key[i + y->num_keys + 1] = (*node)->children[1]->key[i];
                    }
                    for (int i = 0; i < (*node)->children[1]->num_keys + 1; i++)
                    {
                        y->children[i + MINIMUM_DEGREE] = (*node)->children[1]->children[i];
                    }
                    y->num_keys = y->num_keys + 1 + (*node)->children[1]->num_keys;

                    //Shifting the keys to the left
                    for (int i = j; i < 2 * MINIMUM_DEGREE - 2; i++)
                    {
                        (*node)->key[i] = (*node)->key[i + 1];
                    }

                    //Empty the memory location filled with J=1
                    secondary_memory[(*node)->children[1]->coord[0]]--;
                    memory_bool_btree[(*node)->children[1]->coord[0]][(*node)->children[1]->coord[1]] = false;

                    //Shifting the children to the left
                    for (int i = 1; i < (*node)->num_keys; i++)
                    {
                        (*node)->children[i] = (*node)->children[i + 1];
                    }

                    //Decreament number of keys of node
                    (*node)->num_keys--;
                    for (int i = (*node)->num_keys; i < 2 * MINIMUM_DEGREE - 1; i++)
                    {
                        (*node)->key[i] = 0;
                    }
                    for (int i = (*node)->num_keys + 1; i < 2 * MINIMUM_DEGREE; i++)
                    {
                        (*node)->children[i] = NULL;
                    }

                    // //Empty the memory location filled with J=1
                    // secondary_memory[(*node)->children[1]->coord[0]]--;
                    // memory_bool_btree[(*node)->children[1]->coord[0]][(*node)->children[1]->coord[1]] = false;

                    // for (int i = (*node)->num_keys + 1; i < 2 * MINIMUM_DEGREE - 1; i++)
                    // {
                    //     (*node)->children[0]->key[i] = (*node)->children[1]->key[i - ((*node)->num_keys + 1)];
                    // }

                    // for (int i = (*node)->num_keys + 1; i < 2 * MINIMUM_DEGREE; i++)
                    // {
                    //     (*node)->children[0]->children[i] = (*node)->children[1]->children[i - ((*node)->num_keys + 1)];
                    // }

                    // (*node)->children[0]->num_keys = 2 * MINIMUM_DEGREE - 1;

                    // for (int i = 0; i < (*node)->num_keys - 1; i++)
                    // {
                    //     (*node)->key[i] = (*node)->key[i + 1];
                    // }
                    // for (int i = (*node)->num_keys - 1; i < 2 * MINIMUM_DEGREE - 1; i++)
                    // {
                    //     (*node)->key[i] = 0;
                    // }
                    // for (int i = 1; i < (*node)->num_keys; i++)
                    // {
                    //     (*node)->children[i] = (*node)->children[i + 1];
                    // }
                    // (*node)->num_keys--;
                    // for (int i = (*node)->num_keys; i < 2 * MINIMUM_DEGREE; i++)
                    // {
                    //     (*node)->children[i] = NULL;
                    // }

                    // Recurses to delete the node in the combined node
                    delete_node_BTREE(&(*node)->children[0], num, time, secondary_memory);
                }
            }
            //IF THE KEY LIES IN THE RIGHTMOST CHILD OF THE NODE
            else if (j == (*node)->num_keys)
            {
                //IF the second last child has atleast T KEYS then proceed with case 3A
                if ((*node)->children[j - 1]->num_keys >= MINIMUM_DEGREE)
                {
                    //DISK READ OPERATION FOR BOTH THE CHILD NODES FOR EACH SITUATION
                    // if ((*node)->coord[0] != (*node)->children[j - 1]->coord[0] && (*node)->coord[0] != (*node)->children[j]->coord[0] && (*node)->children[j - 1]->coord[0] != (*node)->children[j]->coord[0])
                    // {
                    //     (*time) = (*time) + DISK + DISK + FAST + FAST;
                    //     (*time) = (*time) + DISK + DISK + DISK; //DISK WRITE FOR ALL THE THREE ROWS
                    // }
                    // else if ((*node)->coord[0] == (*node)->children[j - 1]->coord[0] && (*node)->coord[0] == (*node)->children[j]->coord[0] && (*node)->children[j - 1]->coord[0] == (*node)->children[j]->coord[0])
                    // {
                    //     (*time) = (*time) + FAST + FAST;
                    //     (*time) = (*time) + DISK; //DISK WRITE FOR THAT ROW
                    // }
                    // else
                    // {
                    //     (*time) = (*time) + DISK + FAST + FAST;
                    //     (*time) = (*time) + DISK + DISK; //DISK WRITE FOR THE TWO ROWS
                    // }
                    time_adder_DISK_READ_3_nodes((*node), (*node)->children[j-1], (*node)->children[j], time);
                    time_adder_DISK_WRITE_3_nodes((*node), (*node)->children[j-1], (*node)->children[j], time);

                    struct BTREE_node *y = (*node)->children[j];
                    //Shifting a key from the node down to last child and moving a key from second last child to node
                    for (int i = y->num_keys - 1; i >= 0; i--)
                    {
                        y->key[i + 1] = y->key[i];
                    }
                    for (int i = y->num_keys; i >= 0; i--)
                    {
                        y->children[i + 1] = y->children[i];
                    }
                    y->key[0] = (*node)->key[j - 1];
                    (*node)->key[j - 1] = (*node)->children[j - 1]->key[(*node)->children[j - 1]->num_keys - 1];
                    y->children[0] = (*node)->children[j - 1]->children[(*node)->children[j - 1]->num_keys];
                    y->num_keys++;
                    (*node)->children[j - 1]->num_keys--;
                    for (int i = (*node)->children[j - 1]->num_keys; i < 2 * MINIMUM_DEGREE - 1; i++)
                    {
                        (*node)->children[j - 1]->key[i] = 0;
                    }
                    for (int i = (*node)->children[j - 1]->num_keys + 1; i < 2 * MINIMUM_DEGREE; i++)
                    {
                        (*node)->children[j - 1]->children[i] = NULL;
                    }
                    // //Shifting a key from the node down to last child and moving a key from second last child to node
                    // (*node)->children[j]->key[1] = (*node)->children[j]->key[0];
                    // (*node)->children[j]->key[0] = (*node)->key[j - 1];
                    // (*node)->key[j - 1] = (*node)->children[j - 1]->key[(*node)->children[j - 1]->num_keys - 1];

                    // (*node)->children[j]->children[2] = (*node)->children[j]->children[1];
                    // (*node)->children[j]->children[1] = (*node)->children[j]->children[0];
                    // (*node)->children[j]->children[0] = (*node)->children[j - 1]->children[(*node)->children[j - 1]->num_keys];
                    // (*node)->children[j]->num_keys++;

                    // //empty the location of that memory cell
                    // (*node)->children[j - 1]->num_keys--;
                    // for (int i = (*node)->children[j - 1]->num_keys; i < 2 * MINIMUM_DEGREE - 1; i++)
                    // {
                    //     (*node)->children[j - 1]->key[i] = 0;
                    // }
                    // for (int i = (*node)->children[j - 1]->num_keys + 1; i < 2 * MINIMUM_DEGREE; i++)
                    // {
                    //     (*node)->children[j - 1]->children[i] = NULL;
                    // }
                    //Recurses to delete the key in the last child
                    delete_node_BTREE(&(*node)->children[j], num, time, secondary_memory);
                }
                else
                {
                    //IF J=0 and J=1 BOTH HAVE ONLY T-1 KEYS THEN MERGE AND DELETE FROM COMBINED NODES AND PROCEED WITH CASE 3B
                    //DISK READ OPERTION FOR EACH SITUATION
                    // if ((*node)->coord[0] != (*node)->children[j - 1]->coord[0] && (*node)->coord[0] != (*node)->children[j]->coord[0] && (*node)->children[j - 1]->coord[0] != (*node)->children[j]->coord[0])
                    // {
                    //     (*time) = (*time) + DISK + DISK + FAST + FAST;
                    //     (*time) = (*time) + DISK + DISK + DISK; //DISK WRITE FOR ALL THE THREE ROWS
                    // }
                    // else if ((*node)->coord[0] == (*node)->children[j - 1]->coord[0] && (*node)->coord[0] == (*node)->children[j]->coord[0] && (*node)->children[j - 1]->coord[0] == (*node)->children[j]->coord[0])
                    // {
                    //     (*time) = (*time) + FAST + FAST;
                    //     (*time) = (*time) + DISK; //DISK WRITE FOR THAT ROW
                    // }
                    // else
                    // {
                    //     (*time) = (*time) + DISK + FAST + FAST;
                    //     (*time) = (*time) + DISK + DISK; //DISK WRITE FOR THE TWO ROWS
                    // }
                    time_adder_DISK_READ_3_nodes((*node), (*node)->children[j-1], (*node)->children[j], time);
                    time_adder_DISK_WRITE_3_nodes((*node), (*node)->children[j-1], (*node)->children[j], time);
                    struct BTREE_node *y = (*node)->children[j - 1];
                    y->key[y->num_keys] = (*node)->key[j - 1];
                    for (int i = 0; i < (*node)->children[j]->num_keys; i++)
                    {
                        y->key[i + y->num_keys + 1] = (*node)->children[j]->key[i];
                    }
                    for (int i = 0; i < (*node)->children[j]->num_keys + 1; i++)
                    {
                        y->children[i + MINIMUM_DEGREE] = (*node)->children[j]->children[i];
                    }

                    //Empty the memory location of jth index child
                    --secondary_memory[(*node)->children[j]->coord[0]];
                    memory_bool_btree[(*node)->children[j]->coord[0]][(*node)->children[j]->coord[1]] = false;

                    (*node)->num_keys--;
                    y->num_keys = y->num_keys + 1 + (*node)->children[j]->num_keys;
                    for (int i = (*node)->num_keys; i < 2 * MINIMUM_DEGREE - 1; i++)
                    {
                        (*node)->key[i] = 0;
                    }
                    for (int i = (*node)->num_keys + 1; i < 2 * MINIMUM_DEGREE; i++)
                    {
                        (*node)->children[i] = NULL;
                    }

                    // (*node)->children[(*node)->num_keys - 1]->key[1] = (*node)->key[(*node)->num_keys - 1];
                    // for (int i = MINIMUM_DEGREE; i < 2 * MINIMUM_DEGREE - 1; i++)
                    // {
                    //     (*node)->children[(*node)->num_keys - 1]->key[i] = (*node)->children[(*node)->num_keys]->key[i - MINIMUM_DEGREE];
                    // }
                    // for (int i = MINIMUM_DEGREE; i < 2 * MINIMUM_DEGREE; i++)
                    // {
                    //     (*node)->children[(*node)->num_keys - 1]->children[i] = (*node)->children[(*node)->num_keys]->children[i - MINIMUM_DEGREE];
                    // }
                    // //Empty the memory location of jth index child
                    // --secondary_memory[(*node)->children[j]->coord[0]];
                    // memory_bool_btree[(*node)->children[j]->coord[0]][(*node)->children[j]->coord[1]] = false;

                    // (*node)->num_keys--;
                    // for (int i = (*node)->num_keys; i < 2 * MINIMUM_DEGREE - 1; i++)
                    // {
                    //     (*node)->key[i] = 0;
                    // }
                    // for (int i = (*node)->num_keys + 1; i < 2 * MINIMUM_DEGREE; i++)
                    // {
                    //     (*node)->children[i] = 0;
                    // }
                    // Recurses to delete the node in the combined node
                    delete_node_BTREE(&(*node)->children[(*node)->num_keys], num, time, secondary_memory);
                }
            }
            else if ((*node)->children[j - 1]->num_keys == MINIMUM_DEGREE - 1 && (*node)->children[j + 1]->num_keys == MINIMUM_DEGREE - 1)
            {
                //CASE 3B WHEN BOTH THE SIBLINGS AND THE CHILD HAVE T-1 KEYS
                // if ((*node)->coord[0] != (*node)->children[j - 1]->coord[0] && (*node)->coord[0] != (*node)->children[j + 1]->coord[0] && (*node)->children[j - 1]->coord[0] != (*node)->children[j + 1]->coord[0])
                // {
                //     (*time) = (*time) + DISK + DISK + FAST + FAST;
                //     (*time) = (*time) + DISK + DISK + DISK; //DISK WRITE FOR ALL THE THREE ROWS
                // }
                // else if ((*node)->coord[0] == (*node)->children[j - 1]->coord[0] && (*node)->coord[0] == (*node)->children[j + 1]->coord[0] && (*node)->children[j - 1]->coord[0] == (*node)->children[j + 1]->coord[0])
                // {
                //     (*time) = (*time) + FAST + FAST;
                //     (*time) = (*time) + DISK; //DISK WRITE FOR THAT ROW
                // }
                // else
                // {
                //     (*time) = (*time) + DISK + FAST + FAST;
                //     (*time) = (*time) + DISK + DISK; //DISK WRITE FOR THE TWO ROWS
                // }
                time_adder_DISK_READ_3_nodes((*node), (*node)->children[j-1], (*node)->children[j+1], time);
                time_adder_DISK_WRITE_3_nodes((*node), (*node)->children[j-1], (*node)->children[j+1], time);

                struct BTREE_node *y = (*node)->children[j - 1];
                y->key[y->num_keys] = (*node)->key[j - 1];
                for (int i = 0; i < (*node)->children[j]->num_keys; i++)
                {
                    y->key[i + y->num_keys + 1] = (*node)->children[j]->key[i];
                }
                for (int i = 0; i < (*node)->children[j]->num_keys + 1; i++)
                {
                    y->children[i + MINIMUM_DEGREE] = (*node)->children[j]->children[i];
                }

                //EMPTY THE LOCATION of J+1th CHILD
                --secondary_memory[(*node)->children[j + 1]->coord[0]];
                memory_bool_btree[(*node)->children[j + 1]->coord[0]][(*node)->children[j + 1]->coord[1]] = false;
                for (int i = j - 1; i < 2 * MINIMUM_DEGREE - 2; i++)
                {
                    (*node)->key[i] = (*node)->key[i + 1];
                }
                (*node)->num_keys--;
                for (int i = (*node)->num_keys; i < 2 * MINIMUM_DEGREE - 1; i++)
                {
                    (*node)->key[i] = 0;
                }
                for (int i = j + 1; i < 2 * MINIMUM_DEGREE - 1; i++)
                {
                    (*node)->children[i] = (*node)->children[i + 1];
                }
                for (int i = (*node)->num_keys + 1; i < 2 * MINIMUM_DEGREE; i++)
                {
                    (*node)->children[i] = NULL;
                }
                y->num_keys = y->num_keys + 1 + (*node)->children[j]->num_keys;
                // //EMPTY THE LOCATION of J+1th CHILD
                // --secondary_memory[(*node)->children[j + 1]->coord[0]];
                // memory_bool_btree[(*node)->children[j + 1]->coord[0]][(*node)->children[j + 1]->coord[1]] = false;

                // (*node)->children[j - 1]->key[1] = (*node)->key[j - 1];
                // for (int i = MINIMUM_DEGREE; i < 2 * MINIMUM_DEGREE - 1; i++)
                // {
                //     (*node)->children[j - 1]->key[i] = (*node)->children[j]->key[i - MINIMUM_DEGREE];
                // }
                // for (int i = MINIMUM_DEGREE; i < 2 * MINIMUM_DEGREE; i++)
                // {
                //     (*node)->children[j - 1]->children[i] = (*node)->children[j]->children[i - MINIMUM_DEGREE];
                // }
                // for (int i = j - 1; i < 2 * MINIMUM_DEGREE - 2; i++)
                // {
                //     (*node)->key[i] = (*node)->key[i + 1];
                // }
                // (*node)->num_keys--;
                // for (int i = (*node)->num_keys; i < 2 * MINIMUM_DEGREE - 1; i++)
                // {
                //     (*node)->key[i] = 0;
                // }
                // for (int i = j + 1; i < 2 * MINIMUM_DEGREE - 1; i++)
                // {
                //     (*node)->children[i] = (*node)->children[i + 1];
                // }
                // for (int i = (*node)->num_keys + 1; i < 2 * MINIMUM_DEGREE; i++)
                // {
                //     (*node)->children[i] = NULL;
                // }

                //Recurses to delete in the combined node
                delete_node_BTREE(&(*node)->children[j - 1], num, time, secondary_memory);
            }
            else if ((*node)->children[j - 1]->num_keys >= MINIMUM_DEGREE)
            {
                //CASE 3A WHEN SIBLING HAS T KEYS AND CHILD HAS T-1 KEYS (LEFT SIDE)
                //DISK READ OPERTION FOR EACH SITUATION
                // if ((*node)->coord[0] != (*node)->children[j - 1]->coord[0] && (*node)->coord[0] != (*node)->children[j]->coord[0] && (*node)->children[j - 1]->coord[0] != (*node)->children[j]->coord[0])
                // {
                //     (*time) = (*time) + DISK + DISK + FAST + FAST;
                //     (*time) = (*time) + DISK + DISK + DISK; //DISK WRITE FOR ALL THE THREE ROWS
                // }
                // else if ((*node)->coord[0] == (*node)->children[j - 1]->coord[0] && (*node)->coord[0] == (*node)->children[j]->coord[0] && (*node)->children[j - 1]->coord[0] == (*node)->children[j]->coord[0])
                // {
                //     (*time) = (*time) + FAST + FAST;
                //     (*time) = (*time) + DISK; //DISK WRITE FOR THAT ROW
                // }
                // else
                // {
                //     (*time) = (*time) + DISK + FAST + FAST; //DISK WRITE FOR THE TWO ROWS
                //     (*time) = (*time) + DISK + DISK;        //DISK WRITE FOR THE TWO ROWS
                // }
                time_adder_DISK_READ_3_nodes((*node), (*node)->children[j-1], (*node)->children[j], time);
                time_adder_DISK_WRITE_3_nodes((*node), (*node)->children[j-1], (*node)->children[j], time);

                //Shifting a key from node to the child and from the sibling to the node
                struct BTREE_node *y = (*node)->children[j];
                for (int i = y->num_keys - 1; i >= 0; i--)
                {
                    y->key[i + 1] = y->key[i];
                }
                for (int i = y->num_keys; i >= 0; i--)
                {
                    y->children[i + 1] = y->children[i];
                }
                y->key[0] = (*node)->key[j - 1];
                (*node)->key[j - 1] = (*node)->children[j - 1]->key[(*node)->children[j - 1]->num_keys - 1];
                y->children[0] = (*node)->children[j - 1]->children[(*node)->children[j - 1]->num_keys];
                y->num_keys++;
                (*node)->children[j - 1]->num_keys--;
                for (int i = (*node)->children[j - 1]->num_keys; i < 2 * MINIMUM_DEGREE - 1; i++)
                {
                    (*node)->children[j - 1]->key[i] = 0;
                }
                for (int i = (*node)->children[j - 1]->num_keys + 1; i < 2 * MINIMUM_DEGREE; i++)
                {
                    (*node)->children[j - 1]->children[i] = NULL;
                }
                // //Shifting a key from node to the child and from the sibling to the node
                // (*node)->children[j]->key[1] = (*node)->children[j]->key[0];
                // (*node)->children[j]->children[2] = (*node)->children[j]->children[1];
                // (*node)->children[j]->children[1] = (*node)->children[j]->children[0];
                // (*node)->children[j]->children[0] = (*node)->children[j - 1]->children[(*node)->children[j - 1]->num_keys];

                // (*node)->children[j]->key[0] = (*node)->key[j - 1];
                // (*node)->key[j - 1] = (*node)->children[j - 1]->key[(*node)->children[j - 1]->num_keys - 1];

                // (*node)->children[j - 1]->num_keys--;
                // (*node)->children[j]->num_keys++;

                // for (int i = (*node)->children[j - 1]->num_keys; i < 2 * MINIMUM_DEGREE - 1; i++)
                // {
                //     (*node)->children[j - 1]->key[i] = 0;
                // }
                // for (int i = (*node)->children[j - 1]->num_keys + 1; i < 2 * MINIMUM_DEGREE; i++)
                // {
                //     (*node)->children[j - 1]->children[i] = NULL;
                // }

                //Recurses to delete the key in the child
                delete_node_BTREE(&(*node)->children[j], num, time, secondary_memory);
            }
            else
            {
                //CASE 3A WHEN SIBLING HAS T KEYS AND CHILD HAS T-1 KEYS (RIGHT SIDE)
                //DISK READ OPERTION FOR EACH SITUATION
                // if ((*node)->coord[0] != (*node)->children[j + 1]->coord[0] && (*node)->coord[0] != (*node)->children[j]->coord[0] && (*node)->children[j + 1]->coord[0] != (*node)->children[j]->coord[0])
                // {
                //     (*time) = (*time) + DISK + DISK + FAST + FAST;
                //     (*time) = (*time) + DISK + DISK + DISK; //DISK WRITE FOR ALL THE THREE ROWS
                // }
                // else if ((*node)->coord[0] == (*node)->children[j + 1]->coord[0] && (*node)->coord[0] == (*node)->children[j]->coord[0] && (*node)->children[j + 1]->coord[0] == (*node)->children[j]->coord[0])
                // {
                //     (*time) = (*time) + FAST + FAST;
                //     (*time) = (*time) + DISK; //DISK WRITE FOR THAT ROW
                // }
                // else
                // {
                //     (*time) = (*time) + DISK + FAST + FAST;
                //     (*time) = (*time) + DISK + DISK; //DISK WRITE FOR THE TWO ROWS
                // }
                time_adder_DISK_READ_3_nodes((*node), (*node)->children[j], (*node)->children[j+1], time);
                time_adder_DISK_WRITE_3_nodes((*node), (*node)->children[j], (*node)->children[j+1], time);

                //Shifting a key from node to the child and from the sibling to the node
                struct BTREE_node *y = (*node)->children[j];
                y->key[y->num_keys] = (*node)->key[j];
                (*node)->key[j] = (*node)->children[j + 1]->key[0];
                y->children[y->num_keys + 1] = (*node)->children[j + 1]->children[0];
                y->num_keys++;
                for (int i = 0; i < (*node)->children[j + 1]->num_keys - 1; i++)
                {
                    (*node)->children[j + 1]->key[i] = (*node)->children[j + 1]->key[i + 1];
                }
                for (int i = 0; i < (*node)->children[j + 1]->num_keys; i++)
                {
                    (*node)->children[j + 1]->children[i] = (*node)->children[j + 1]->children[i + 1];
                }
                (*node)->children[j + 1]->num_keys--;
                for (int i = (*node)->children[j + 1]->num_keys; i < 2 * MINIMUM_DEGREE - 1; i++)
                {
                    (*node)->children[j + 1]->key[i] = 0;
                }
                for (int i = (*node)->children[j + 1]->num_keys + 1; i < 2 * MINIMUM_DEGREE; i++)
                {
                    (*node)->children[j + 1]->children[i] = NULL;
                }
                // //Shifting a key from node to the child and from the sibling to the node
                // (*node)->children[j]->key[1] = (*node)->key[j];
                // (*node)->key[j] = (*node)->children[j + 1]->key[0];
                // (*node)->children[j]->children[2] = (*node)->children[j + 1]->children[0];
                // (*node)->children[j]->num_keys++;

                // for (int i = 0; i < (*node)->children[j + 1]->num_keys - 1; i++)
                // {
                //     (*node)->children[j + 1]->key[i] = (*node)->children[j + 1]->key[i + 1];
                // }
                // for (int i = 0; i < (*node)->children[j + 1]->num_keys; i++)
                // {
                //     (*node)->children[j + 1]->children[i] = (*node)->children[j + 1]->children[i + 1];
                // }

                // (*node)->children[j + 1]->num_keys--;
                // for (int i = (*node)->children[j + 1]->num_keys; i < 2 * MINIMUM_DEGREE - 1; i++)
                // {
                //     (*node)->children[j + 1]->key[i] = 0;
                // }
                // for (int i = (*node)->children[j + 1]->num_keys + 1; i < 2 * MINIMUM_DEGREE; i++)
                // {
                //     (*node)->children[j + 1]->children[i] = NULL;
                // }
                //Recurses to delete the key in the child
                delete_node_BTREE(&(*node)->children[j], num, time, secondary_memory);
            }
        }
    }
}
#endif // BTREE_H_INCLUDED