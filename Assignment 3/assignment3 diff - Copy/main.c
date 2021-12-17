//Tanmay Garg Assignment 3 Data Structures

/*  To run the Program enter the following commands in the terminal or powershell windows
*
*   $ gcc *.c *.h -o main
*
*   $ ./main
*   For windows use the .vscode folder and edit the path to the compiler and click on Run->Run without Debugging
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "btree_memory.h"
#include "bst_memory.h"

//Print inorder traversal for BST
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

//Print inorder traversal for BTREE
void print_inorder_BTREE(struct BTREE_node *node)
{
    if (node == NULL)
        return;
    print_inorder_BTREE(node->children[0]);
    for (int i = 0; i < node->num_keys; i++)
    {
        printf("%d: %d,%d\n", node->key[i], node->coord[0], node->coord[1]);
        print_inorder_BTREE(node->children[i + 1]);
    }
}

int main()
{
    //BST
// int coord_node[2] = {0,0};
// int time = 0;
// int secondary_memory[4] = {0, -1, -1, -1};
// struct BST_mem_node* list_of_nos = NULL;
// list_of_nos = insert_node_BST_mem(&list_of_nos, number_array_key[0], &time, coord_node, secondary_memory);
// list_of_nos->coord[1] = 0;
// secondary_memory[0] = 0;
// printf("Creating the BST Structure.......\n");
// int num = 0;
// for(int i = 1; i < 60; i++)
// {
//     num = number_array_key[i];
//     list_of_nos = insert_node_BST_mem(&list_of_nos, num, &time, list_of_nos->coord, secondary_memory);
// }
// printf("BST Structure has been created. Time taken = %d\n", time);
// printf("\n");
// // printf("coordinates of all the node of BST\n");
// // print_inorder(list_of_nos);

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
// // printf("coordinates of all the node of BST\n");
// // print_inorder(list_of_nos);

//BTREE
//Input arrays for insertion, search and deletion
    int number_array_key[60] = {17, 13, 2, 27, 48, 54, 39, 57, 60, 3, 23, 46, 16, 18, 49, 45, 33, 36, 55, 19, 47, 35, 7, 22, 4, 50, 9, 56, 37, 12, 11, 21, 31, 38, 29, 44, 8, 26, 25, 40, 6, 58, 51, 1, 15, 30, 52, 10, 28, 59, 53, 34, 43, 42, 24, 14, 32, 41, 5, 20};
    int search_array_key[9] = {49, 27, 22, 38, 11, 55, 7, 35, 59};
    int delete_array_key[9] = {13, 19, 24, 37, 43, 53, 18, 38, 58};
    int num = 0;

printf("Creating the structure of BTREE.................................\n");
struct BTREE_node *list_of_nos_btree = new_BTREE_node();
int time_btree = 0;
int coord_node_btree[2] = {0,0};
int secondary_memory_btree[4] = {0, -1, -1, -1};

insert_node_BTREE(&list_of_nos_btree, 1, &time_btree, coord_node_btree, secondary_memory_btree);
memory_bool_btree[list_of_nos_btree->coord[0]][list_of_nos_btree->coord[1]] = true;
list_of_nos_btree->coord[1] = 0;
secondary_memory_btree[0] = 0;

// for(int i=1; i < 60; i++)
// {
//     num = number_array_key[i];
//     insert_node_BTREE(&list_of_nos_btree, num, &time_btree, list_of_nos_btree->coord, secondary_memory_btree);
// }
insert_node_BTREE(&list_of_nos_btree, 2, &time_btree, list_of_nos_btree->coord, secondary_memory_btree);
insert_node_BTREE(&list_of_nos_btree, 3, &time_btree, list_of_nos_btree->coord, secondary_memory_btree);
insert_node_BTREE(&list_of_nos_btree, 4, &time_btree, list_of_nos_btree->coord, secondary_memory_btree);


delete_node_BTREE(&list_of_nos_btree, 3, &time_btree, secondary_memory_btree);
delete_node_BTREE(&list_of_nos_btree, 1, &time_btree, secondary_memory_btree);
printf("BTREE created. Time taken: %d\n", time_btree);

printf("coordinates of all the node of BTREE\n");
print_inorder_BTREE(list_of_nos_btree);
time_btree=0;
// search_BTREE_node(list_of_nos_btree, 49, &time_btree);

// for(int i = 0; i < 9; i++)
// {
//     time_btree=0;
//     search_BTREE_node(list_of_nos_btree, search_array_key[i], &time_btree);
//     printf("The time taken to search %d: %d\n", search_array_key[i], time_btree);
// }

// printf("\n");
// for(int i = 0; i < 9; i++)
// {
//     num = delete_array_key[i];
//     time_btree=0;
//     delete_node_BTREE(&list_of_nos_btree, num, &time_btree, secondary_memory_btree);
//     printf("The time taken to delete %d: %d\n", delete_array_key[i], time_btree);
// }
    

    // /****************************************************************
    //  * This section is for creating both the TREE structures and then printing out time taken for creation
    // *****************************************************************/
    // int num = 0;
    // printf("Creating BST and BTREE.......\n");

    // /**************************
    //  * BST
    // **************************/
    // int coord_node[2] = {0, 0};
    // int time = 0;
    // int secondary_memory[4] = {0, -1, -1, -1};
    // struct BST_mem_node *list_of_nos = NULL;
    // list_of_nos = insert_node_BST_mem(&list_of_nos, number_array_key[0], &time, coord_node, secondary_memory);
    // list_of_nos->coord[1] = 0;
    // secondary_memory[0] = 0;

    // /**************************
    //  * BTREE
    // **************************/
    // struct BTREE_node *list_of_nos_btree = new_BTREE_node();
    // int time_btree = 0;
    // int coord_node_btree[2] = {0, 0};
    // int secondary_memory_btree[10] = {0, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    // insert_node_BTREE(&list_of_nos_btree, number_array_key[0], &time_btree, coord_node_btree, secondary_memory_btree);
    // memory_bool_btree[list_of_nos_btree->coord[0]][list_of_nos_btree->coord[1]] = true;
    // list_of_nos_btree->coord[1] = 0;
    // secondary_memory_btree[0] = 0;

    // /**************************
    //  * For loop for inserting keys in both the trees
    // ***************************/
    // for (int i = 1; i < 60; i++)
    // {
    //     num = number_array_key[i];
    //     list_of_nos = insert_node_BST_mem(&list_of_nos, num, &time, list_of_nos->coord, secondary_memory);
    //     insert_node_BTREE(&list_of_nos_btree, num, &time_btree, list_of_nos_btree->coord, secondary_memory_btree);
    // }
    // printf("|--------------------|\n");
    // printf("|Tree Structure| Time|\n");
    // printf("|--------------------|\n");
    // printf("|BST           | %d|\n", time);
    // printf("|BTREE         | %d|\n", time_btree);
    // printf("|--------------------|\n");
    // printf("\n");

    // /**************************
    //  * END for insertion of keys and printing of time
    // ***************************/

    // /**************************
    //  * For loop for searching keys in both the trees
    // ***************************/
    // printf("|-------------------Search for keys-------------------|\n");
    // for (int i = 0; i < 9; i++)
    // {
    //     time = 0;
    //     search_mem_node(list_of_nos, search_array_key[i], &time);

    //     time_btree = 0;
    //     search_BTREE_node(list_of_nos_btree, search_array_key[i], &time_btree);
        

    //     printf("The time taken to search %d:    BST: %d :: BTREE: %d\n", search_array_key[i], time, time_btree);
    // }
    // printf("|-----------------------------------------------------|\n");
    // printf("\n");

    // /**************************
    //  * END for searching of keys and printing of time
    // ***************************/

    // /**************************
    //  * For loop for deleting keys in both the trees
    // ***************************/
    // printf("|-------------------Delete for keys-------------------|\n");
    // for (int i = 0; i < 9; i++)
    // {
    //     time = 0;
    //     num = delete_array_key[i];
    //     list_of_nos = delete_node_BST_mem(list_of_nos, num, &time, list_of_nos->coord, secondary_memory);

    //     time_btree = 0;
    //     delete_node_BTREE(&list_of_nos_btree, num, &time_btree, secondary_memory_btree);
    //     //print_inorder_BTREE(list_of_nos_btree);

    //     printf("The time taken to delete %d:    BST: %d :: BTREE: %d\n", delete_array_key[i], time, time_btree);
    // }
    // printf("|-----------------------------------------------------|\n");

    // /**************************
    //  * END for deletion of keys and printing of time
    // ***************************/
    // printf("end\n");
    // return EXIT_SUCCESS;
}

/////////////////////////// Debugging Code

// //BST
// int coord_node[2] = {0,0};
// int time = 0;
// int secondary_memory[4] = {0, -1, -1, -1};
// struct BST_mem_node* list_of_nos = NULL;
// list_of_nos = insert_node_BST_mem(&list_of_nos, number_array_key[0], &time, coord_node, secondary_memory);
// list_of_nos->coord[1] = 0;
// secondary_memory[0] = 0;
// printf("Creating the BST Structure.......\n");
// int num = 0;
// for(int i = 1; i < 60; i++)
// {
//     num = number_array_key[i];
//     list_of_nos = insert_node_BST_mem(&list_of_nos, num, &time, list_of_nos->coord, secondary_memory);
// }
// printf("BST Structure has been created. Time taken = %d\n", time);
// printf("\n");
// // printf("coordinates of all the node of BST\n");
// // print_inorder(list_of_nos);

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
// // printf("coordinates of all the node of BST\n");
// // print_inorder(list_of_nos);

// //BTREE

// printf("Creating the structure of BTREE.................................\n");
// struct BTREE_node *list_of_nos_btree = new_BTREE_node();
// int time_btree = 0;
// int coord_node_btree[2] = {0,0};
// int secondary_memory_btree[4] = {0, -1, -1, -1};

// insert_node_BTREE(&list_of_nos_btree, number_array_key[0], &time_btree, coord_node_btree, secondary_memory_btree);
// memory_bool_btree[list_of_nos_btree->coord[0]][list_of_nos_btree->coord[1]] = true;
// list_of_nos_btree->coord[1] = 0;
// secondary_memory_btree[0] = 0;

// for(int i=1; i < 60; i++)
// {
//     num = number_array_key[i];
//     insert_node_BTREE(&list_of_nos_btree, num, &time_btree, list_of_nos_btree->coord, secondary_memory_btree);
// }
// printf("BTREE created. Time taken: %d\n", time_btree);

// printf("coordinates of all the node of BTREE\n");
// print_inorder_BTREE(list_of_nos_btree);
// time_btree=0;
// search_BTREE_node(list_of_nos_btree, 49, &time_btree);

// for(int i = 0; i < 9; i++)
// {
//     time_btree=0;
//     search_BTREE_node(list_of_nos_btree, search_array_key[i], &time_btree);
//     printf("The time taken to search %d: %d\n", search_array_key[i], time_btree);
// }

// printf("\n");
// for(int i = 0; i < 9; i++)
// {
//     num = delete_array_key[i];
//     time_btree=0;
//     delete_node_BTREE(&list_of_nos_btree, num, &time_btree, secondary_memory_btree);
//     printf("The time taken to delete %d: %d\n", delete_array_key[i], time_btree);
// }
