#ifndef BST_H_INCLUDED
#define BST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define STRING_SIZE 10
#define INPUT_SIZE 100

//Structure of Binary Search Tree node
struct BST_node
{
    char song_name[STRING_SIZE];
    struct BST_node *left;
    struct BST_node *right;
};

//Function to allocate memory for new node of BST
struct BST_node *new_BST_node(char *s);

//Function to create a BST from a given preorder traversal and also given that it is in lexicographic order
struct BST_node* make_bst_tree(char song_list_preorder[STRING_SIZE][STRING_SIZE], int *preorder_position_number, int begin, int end, int size);

//Function to print Inorder Traversal of BST
void print_order(struct BST_node *node);

//Function to search a particular key in a BST and returns the pointer to the node
struct BST_node *search_key_BST(struct BST_node *node, char *s);

//Function to find the next element to fill the gaps when a partiuclar node needs to be deleted
struct BST_node *find_next_for_deletion_BST(struct BST_node *node);

//Function to delete a node from BST
struct BST_node *delete_node_BST(struct BST_node *node, char *s);

//Calculates the number of songs in the BST tree
void num_songs_playlist_BST(struct BST_node *node, int* num_song_playlist);

#endif // BST_H_INCLUDED