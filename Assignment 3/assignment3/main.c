//Tanmay Garg Assignment 3 Data Structures

/*  To run the Program enter the following commands in the terminal or powershell windows
*
*   $ gcc *.c *.h -o main
*
*   $ ./main
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
//#include <unistd.h>
//#include "bst.h"
#include "btree.h"
#include "bst_memory.h"

void print_inorder(struct BST_mem_node *node)
{
    if (node == NULL)
    {
        return;
    }

    print_inorder(node->left);
    printf("%d: %d,%d\n", node->value, node->coord[0], node->coord[1]);
    print_inorder(node->right);
}

int main()
{
    int number_array_key[60] = {17,13,2,27,48,54,39,57,60,3,23,46,16,18,49,45,33,36,55,19,47,35,7,22,4,50,9,56,37,12,11,21,31,38,29,44,8,26,25,40,6,58,51,1,15,30,52,10,28,59,53,34,43,42,24,14,32,41,5,20};
    int search_array_key[9] = {49, 27, 22, 38, 11, 55, 7, 35, 59};
    int delete_array_key[9] = {13, 19, 24, 37, 43, 53, 18, 38, 58};
    
    // //BST
    // unsigned long long int time_taken_for_creating_BST = 0;
    // printf("Creation of binary tree begins\n");
    // struct BST_node* list_of_nos = NULL;
    // for(int i = 0; i < 60; i++)
    // {
    //     list_of_nos = insert_node_BST(&list_of_nos, number_array_key[i]);
    // }
    // printf("Binary tree Created\n");


    // //Time for search in BST tree
    // int numbers_for_searching[9] = {49, 27, 22, 38, 11, 55, 7, 35, 59};
    
    // for(int i = 0; i < 9; i++)
    // {
    //     list_of_nos = delete_node_BST(list_of_nos, numbers_for_searching[i]);
    // }

    int coord_node[2] = {0,0};
    int time = 0;
    int secondary_memory[4] = {0, -1, -1, -1};
    struct BST_mem_node* list_of_nos = NULL;
    list_of_nos = insert_node_BST_mem(&list_of_nos, number_array_key[0], &time, coord_node, secondary_memory);
    list_of_nos->coord[1] = 0;
    secondary_memory[0] = 0;
    printf("Creating the BST Structure.......\n");
    int num = 0;
    for(int i = 1; i < 60; i++)
    {
        num = number_array_key[i];
        list_of_nos = insert_node_BST_mem(&list_of_nos, num, &time, list_of_nos->coord, secondary_memory);
    }
    printf("BST Structure has been created. Time taken = %d\n", time);
    printf("\n");
    print_inorder(list_of_nos);

    // printf("Search for given keys has begun\n");
    // for(int i = 0; i < 9; i++)
    // {
    //     time = 0;
    //     search_mem_node(list_of_nos, search_array_key[i], &time);
    //     printf("The time taken to search %d: %d\n", search_array_key[i], time);
    // }
    // printf("\n");
    // printf("Deletion of given keys begin\n");
    // for(int i = 0; i < 9; i++)
    // {
    //     time = 0;
    //     num = delete_array_key[i];
    //     list_of_nos = delete_node_BST_mem(list_of_nos, num, &time, list_of_nos->coord, secondary_memory);
    //     printf("The time taken to delete %d: %d\n", delete_array_key[i], time);
        
    // }
    // print_inorder(list_of_nos);
    time = 0;
    search_mem_node(list_of_nos, 70, &time);
    //BTREE
    

    
    return EXIT_SUCCESS;
}
