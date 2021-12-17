//Tanmay Garg Assignment 2 Data Structures

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
#include "bst.h"
#include "linked_list.h"
#include "queue_header.h"
#include "stack_node.h"
#include "avl.h"

//#define MAX(a, b) (((a) > (b)) ? (a) : (b))
//#define MIN(a, b) (a < b) ? a : b

//Function corrects the string after taking input from fgets function to remove \n
void string_corrector(char *s)
{
    int len = strlen(s);
    s[len - 1] = '\0';
}

//Function to play next song in the AVL tree playlist
//It is just like searching for nth inorder traversal node for an AVL tree
void play_next_AVL(struct AVL_TREE_node *node, int nth_inorder_index, int *counter, struct stack_node **sn)
{
    //If node is NULL it returns NULL
    if (node == NULL)
    {
        return;
    }
    //Counter keeps track if it has reached the nth node
    if ((*counter) <= nth_inorder_index)
    {
        //Inorder traversal
        play_next_AVL(node->left, nth_inorder_index, counter, sn);
        (*counter)++;
        if ((*counter) == nth_inorder_index)
        {
            //Prints the song and pushes it to the recent songs stack
            printf("Playing Song %s\n", node->song_name);
            push_stack_node(sn, node->song_name);
            return;
        }

        play_next_AVL(node->right, nth_inorder_index, counter, sn);
    }
}

//Function for playing previous song
bool play_previous(struct queue_header *Q, struct stack_node **sn, int num_repeat)
{
    //number of elements in recent songs stack
    int num_elements_recent_song = num_elements_stack_node(*sn);

    //finding minimum number between num of songs and num of elements in stack for for loop
    int min_num_song_elements = MIN(num_repeat, num_elements_recent_song);

    //popping and putting in queue
    char str_tmp[STRING_SIZE];
    for (int i = 0; i < min_num_song_elements; i++)
    {
        //Song is popped from stack
        pop_stack_node(sn, str_tmp);
        //enqueue song to queue and return true
        enq_queue_header(Q, str_tmp);
        printf("Playing song: %s\n", str_tmp);
    }

    if (num_repeat > num_elements_recent_song || num_repeat == 0)
    {
        printf("There are no %d songs in recent songs list\n", num_repeat);
    }

    //putting songs back to recent songs stack
    char tmp_str[STRING_SIZE];
    for (int i = 0; i < min_num_song_elements; i++)
    {
        deq_queue_header(Q, tmp_str);
        push_stack_node(sn, tmp_str);
    }
    return true;
}

//Function to create a 2D array of song names from the string taken from file in a particular format
void songs_array_creator_BST(char *input_file_string, char songs_bst_array[STRING_SIZE][STRING_SIZE], int *num_songs)
{
    //integer for looping through the entire input_file_string array
    //int i = 0;
    int input_file_string_counter = 0;

    //integer for looping through the input_file_string taking care if it encounters songs separator ':'
    //int j = 0;
    int input_file_string_separator_counter = 0;

    //integer for index to copy the song name
    int strncpy_num = 0;

    //loop through entire array till it encounters \0
    while (input_file_string[input_file_string_counter] != '\0')
    {
        //loop until it encounters ':'
        while (input_file_string[input_file_string_separator_counter] != ':')
        {
            //if jth index gives \0 then breaks
            if (input_file_string[input_file_string_separator_counter] == '\0')
            {
                break;
            }
            input_file_string_separator_counter++;
        }

        //copy the required string to the BST songs array
        strncpy(songs_bst_array[*num_songs], input_file_string + (strncpy_num) * sizeof(char), input_file_string_separator_counter - strncpy_num);
        strncpy_num = input_file_string_separator_counter + 1;
        (*num_songs)++;
        input_file_string_counter++;
        input_file_string_separator_counter++;
        if (input_file_string[input_file_string_separator_counter] == '\0')
        {
            break;
        }
    }
}

int main()
{
    //Stack Pointer for Recent Songs
    struct stack_node *recent_songs_stack = NULL;

    //Temporary Pointer for queue for previous songs
    struct queue_header *tmp_queue = queue_header_new();

    //char array for taking input of entire string in the file
    char input_file_string[INPUT_SIZE] = "";

    //2D char array to store the songs separately
    char songs_bst_array[STRING_SIZE][STRING_SIZE];

    //Pointer to the file which has preorder of the BST
    FILE *file_ptr = fopen("songs.txt", "r");
    if (file_ptr == NULL)
    {
        return EXIT_FAILURE;
    }

    //Reading and correcting the string if necessary
    fgets(input_file_string, INPUT_SIZE, file_ptr);
    if (input_file_string[strlen(input_file_string) - 1] == '\n')
    {
        input_file_string[strlen(input_file_string) - 1] = '\0';
    }

    //Initializing the entire 2D char array with \0
    for (int i = 0; i < STRING_SIZE; i++)
    {
        for (int j = 0; j < STRING_SIZE; j++)
        {
            songs_bst_array[i][j] = '\0';
        }
    }

    //integer for counting the number of songs in the entire array
    int num_songs = 0;

    //Copying the songs to a 2D array of song names from string given in input file
    songs_array_creator_BST(input_file_string, songs_bst_array, &num_songs);

    //integer for position acts like preorder position number which is the index of the song in the array
    int position = 0;
    int *position_ptr = &position;

    struct BST_node *songs_list = make_bst_tree(songs_bst_array, position_ptr, 0, num_songs - 1, num_songs);

    printf("Songs have been LOADED to your LIST of all songs\n");

    /***********************************************
    *   The loop for deleting songs from BST begins
    ************************************************/

    printf("Enter the songs you need to DELETE from your LIST and end with -1 \n");

    //input_char is for getting input from user of the name of the song
    char input_char[STRING_SIZE];
    //input_termin is input_terminator which is -1
    char input_termin[STRING_SIZE] = "-1";

    while (true)
    {
        fgets(input_char, STRING_SIZE, stdin);
        string_corrector(input_char);
        if (strcmp(input_char, input_termin) == 0)
        {
            break;
        }
        if (search_key_BST(songs_list, input_char) != NULL)
        {
            songs_list = delete_node_BST(songs_list, input_char);
            printf("Deleted the song %s\n", input_char);
        }
        else
        {
            printf("Song to be deleted not found \n");
        }
    }

    //integer to get the number of songs in BST
    int num_songs_BST_song_list = 0;
    num_songs_playlist_BST(songs_list, &num_songs_BST_song_list);

    //asking the user to exit if number of songs is 0
    if (num_songs_BST_song_list == 0)
    {
        printf("All songs have been deleted from your LIST of Songs\n");
        printf("Press e to exit the App or press any other key to continue: \n");
        char command[STRING_SIZE];
        fgets(command, STRING_SIZE, stdin);
        string_corrector(command);
        if (strcmp(command, "e") == 0)
        {
            printf("Thank you for using Music App\n");

            fclose(file_ptr);
            return EXIT_SUCCESS;
        }
    }

    /***********************************************
    *   The loop for deleting songs from BST ends
    ************************************************/

    /***********************************************
    *   The loop for adding songs to AVL playlist begins
    ************************************************/
    struct AVL_TREE_node *playlist_avl = NULL;

    //Here input_char is being reused for getting input from user regarding the name of the song
    //Here input_termin is being reused for input terminator
    printf("Enter the songs you need to ADD to PLAYLIST and end with -1 \n");
    while (true)
    {
        fgets(input_char, STRING_SIZE, stdin);
        string_corrector(input_char);
        if (strcmp(input_char, input_termin) == 0)
        {
            break;
        }
        if (search_key_BST(songs_list, input_char) != NULL)
        {
            printf("Song found and added\n");
            playlist_avl = insert_node_AVL_TREE_pointer(&playlist_avl, input_char);
        }
        else
        {
            printf("Song not found\n");
        }
    }
    printf("The PLAYLIST has been created\n");

    /***********************************************
    *   The loop for adding songs to AVL playlist ends
    ************************************************/

    /***********************************************
    *   The loop for deleting songs from AVL playlist begins
    ************************************************/

    //Here input_char is being reused for getting input from user regarding the name of the song
    //Here input_termin is being reused for input terminator
    printf("Enter the songs you would like to DELETE from PLAYLIST and end with -1\n");
    while (true)
    {
        fgets(input_char, STRING_SIZE, stdin);
        string_corrector(input_char);
        if (strcmp(input_char, input_termin) == 0)
        {
            break;
        }
        if (search_key_AVL(playlist_avl, input_char) != NULL)
        {
            printf("Deleted the song %s\n", input_char);
            playlist_avl = delete_node_AVL(&playlist_avl, input_char);
        }
        else
        {
            printf("Song not found in playlist\n");
        }
    }

    //Integer for the number of songs in the playlist AVL
    int num_songs_playlist = 0;
    num_songs_playlist_AVL(playlist_avl, &num_songs_playlist);
    if (num_songs_playlist == 0)
    {
        printf("All songs have been deleted from your PLAYLIST of Songs\n");
        printf("Press e to exit the App or press any other key to continue: \n");
        char command[STRING_SIZE];
        fgets(command, STRING_SIZE, stdin);
        string_corrector(command);
        if (strcmp(command, "e") == 0)
        {
            printf("Thank you for using Music App\n");

            fclose(file_ptr);
            return EXIT_SUCCESS;
        }
    }

    printf("Playlist has been finalised\n");

    /***********************************************
    *   The loop for deleting songs from AVL playlist ends
    ************************************************/

    //input_command takes in input from user for the command to be carried out
    char input_command[STRING_SIZE] = "a";

    //nth inorder index of AVL playlist to play the next song
    int nth_inorder_index = 1;

    //Counter passed to the play next function so that its value changes at the address and is used to play the correct song
    int counter = 0;

    /***********************************************
    *   Main loop for playing songs begins
    ************************************************/

    printf("Play next song, previous song, or end? n/p:k/e\n");
    //Using while loop for main program for playing the application
    while (true)
    {
        scanf("%s", input_command);

        //if input is n then play next
        if (strcmp(input_command, "n") == 0)
        {
            //If nth_inorder_index is greater than number of songs in playlist then it shows playlist is empty
            if (nth_inorder_index > num_songs_playlist)
            {
                printf("Playlist is empty\n");
            }
            else
            {
                //intializing the counter to be 0 to be passed into play next function
                counter = 0;

                //Playing the next song
                play_next_AVL(playlist_avl, nth_inorder_index, &counter, &recent_songs_stack);

                //incrementing the nth_inorder_index to play the next song in the inoder traversal of the playlist
                nth_inorder_index++;

                //intializing the counter to be 0 to be passed into play next function
                counter = 0;
            }
        }

        //if input is p then play previous
        else if (input_command[0] == 'p')
        {
            //if it is p: then it moves forward
            if (input_command[1] == ':')
            {
                //if after p: there is 0 or \0 then it prints invalid input
                if (input_command[2] == '\0')
                {
                    printf("Invalid input\n");
                }
                //else it moves forward
                else
                {
                    //pointer to first number from p:
                    char *ptr = input_command + 2 * sizeof(char);

                    //getting integer from string which is the number of songs to be played in previous
                    int num_song_repeat = atoi(ptr);

                    if (num_song_repeat > 0)
                    {
                        play_previous(tmp_queue, &recent_songs_stack, num_song_repeat);
                    }
                    else
                    {
                        printf("Invalid Input\n");
                    }
                }
            }
            //if input does not have p: then it prints invalid input
            else
            {
                printf("Invalid Input\n");
            }
        }
        //if input is e in exit
        else if (strcmp(input_command, "e") == 0)
        {
            break;
        }
        //else it prints invalid input
        else
        {
            printf("Invalid Input\n");
        }
    }

    printf("Thank you for using Music App\n");

    fclose(file_ptr);
    return EXIT_SUCCESS;
}
