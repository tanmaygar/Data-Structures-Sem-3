#ifndef AVL_H_INCLUDED
#define AVL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bst.h"
#include "linked_list.h"
#include "queue_header.h"
#include "stack_node.h"

#define MAX_IMBALANCE_AVL_TREE 1
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a,b) (a<b)?a:b

//Keeping each node at initial height 1 for simplicty of the functions
struct AVL_TREE_node
{
    char song_name[STRING_SIZE];
    struct AVL_TREE_node *left;
    struct AVL_TREE_node *right;
    int height;
};

//Function which returns the height of AVL node
int find_height_AVL(struct AVL_TREE_node *node);


//Function to allocate memory for new AVL tree node
struct AVL_TREE_node *new_AVL_TREE_node(char *s);

//Function to provide with the right rotation of AVL tree node for balancing purpose
void right_rotation_AVL(struct AVL_TREE_node **node);

//Function to provide with the left rotation of AVL tree node for balancing purpose
void left_rotation_AVL(struct AVL_TREE_node **node);

//Function to provide with the double right rotation of AVL tree node for balancing purpose
void double_right_rotation_AVL(struct AVL_TREE_node **node);

//Function to provide with the double left rotation of AVL tree node for balancing purpose
void double_left_rotation_AVL(struct AVL_TREE_node **node);

//Function to Balance AVL tree node
void balance_AVL_TREE(struct AVL_TREE_node **node);

//Function to insert a node into an AVL tree
struct AVL_TREE_node *insert_node_AVL_TREE_pointer(struct AVL_TREE_node **node, char *s);

//Function to search for a key inside the AVL tree
struct AVL_TREE_node *search_key_AVL(struct AVL_TREE_node *node, char *s);

//Function to find the next element to fill the gaps when a partiuclar node needs to be deleted
struct AVL_TREE_node *find_next_for_deletion_AVL(struct AVL_TREE_node *node);

//Function to delete a node from an AVL tree
struct AVL_TREE_node *delete_node_AVL(struct AVL_TREE_node **node, char *s);

//Calculates the number of songs in the AVL tree
void num_songs_playlist_AVL(struct AVL_TREE_node* node, int* num_song_playlist);


// void insert_node_AVL_TREE(struct AVL_TREE_node *node, char *s)
// {
//     if (node == NULL)
//     {
//         //node = new_AVL_TREE_node(s);
//         node = (struct AVL_TREE_node *)malloc(sizeof(struct AVL_TREE_node));
//         node->left = NULL;
//         node->right = NULL;
//         node->height = 0;
//         strcpy(node->song_name, s);
//     }
//     else if (strcmp(node->song_name, s) < 0)
//     {
//         insert_node_AVL_TREE(node->left, s);
//     }
//     else
//     {
//         insert_node_AVL_TREE(node->right, s);
//     }

//     //balance_AVL_TREE(node);
// }


#endif // AVL_H_INCLUDED
