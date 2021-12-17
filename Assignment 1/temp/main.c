//Tanmay Garg Assignment 1 Data Structures
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "linked_list.h"
#include "stack_node.h"
#include "queue_header.h"
#define MIN(a,b) (a<b)?a:b

//Function to find the song in the Linked List and then pushing to stack
void find_song_in_list(char *s, struct linked_list_node** lln, struct queue_header* Q)
{
    if(find_linked_list_node(lln, s) == true)
    {
        enq_queue_header(Q, s);
        printf("Song Found, Enqueued\n");
    }
    else
    {
        printf("Song not Found. Please input name again\n");
    }
}

//Function for playing next song
bool play_next(struct queue_header* Q, struct stack_node** sn)
{
    //Song is dequeued from queue and then pushed to stack
    char str_tmp[STRING_SIZE];
    deq_queue_header(Q, str_tmp);
    if(strcmp(str_tmp, "empty") != 0)
    {
        push_stack_node(sn, str_tmp);
    }
    //If playlist is empty then it returns false
    else
    {
        printf("Playlist is empty\n");
        return false;
    }
    //it prints the name of the song and returns true
    printf("Playing song: %s\n", str_tmp);
    return true;
}

//Function for playing previous song
bool play_previous(struct queue_header* Q, struct stack_node** sn, int num_repeat)
{
    //number of elements in recent songs stack
    int num_elements_recent_song = num_elements_stack_node(*sn);

    //finding minimum number between num of songs and num of elements in stack for for loop
    int min_num_song_elements = MIN(num_repeat, num_elements_recent_song);

    //popping and putting in queue
    char str_tmp[STRING_SIZE];
    for(int i = 0; i < min_num_song_elements; i++)
    {
        //Song is popped from stack
        pop_stack_node(sn, str_tmp);
        //enqueue song to queue and return true
        enq_queue_header(Q, str_tmp);
        printf("Playing song: %s\n", str_tmp);
    }

    if(num_repeat > num_elements_recent_song)
    {
        printf("There are no %d songs in recent songs list\n", num_repeat);
    }

    //putting songs back to recent songs stack
    char tmp_str[STRING_SIZE];
    for(int i = 0; i < min_num_song_elements; i++)
    {
        deq_queue_header(Q, tmp_str);
        push_stack_node(sn, tmp_str);
    }
    return true;
}

int main()
{
    //Linked List for Storing songs
    struct linked_list_node* store_song_lln = NULL;

    //Queue Pointer for Playlist
    struct queue_header* playlist_queue = queue_header_new();

    //Stack Pointer for Recent Songs
    struct stack_node* recent_songs_stack = NULL;

    //Temporary Pointer for queue for previous songs
    struct queue_header* tmp_queue = queue_header_new();

    //input terminator -1 for strings
    char input_terminate[STRING_SIZE] = "-1";

    char input_songs[STRING_SIZE] = "begin";

    printf("Enter all songs one by one and end with '-1': \n");

    //Using while loop for storing songs
    while(true)
    {
        //scanning input for name of songs
        scanf("%s", input_songs);
        if(strcmp(input_songs, input_terminate) == 0)
        {
            break;
        }
        //Inserting song in linked list
        insert_linked_list_node(&store_song_lln, input_songs);
    }
    printf("All songs have been entered\nWelcome to songs playlist\n");
    printf("Creating a playlist, enter names one by one and end with '-1': \n");

    char input_songs_playlist[STRING_SIZE] = "begin";

    //Using while loop for storing name of songs for playlist
    while(strcmp(input_songs_playlist, input_terminate) != 0)
    {
        //scanning name of songs and checking if they exist or not
        scanf("%s", input_songs_playlist);
        if(strcmp(input_songs_playlist, input_terminate) == 0)
        {
            break;
        }
        //finding song in linked list and then adding to playlist
        find_song_in_list(input_songs_playlist, &store_song_lln, playlist_queue);
    }
    printf("Playlist has been created\n");
    printf("Play next song, previous song, or end? n/p:k/e\n");

    char input_command[STRING_SIZE] = "a";

    //Using while loop for main program for playing the application
    while(true)
    {
        scanf("%s", input_command);

        //if input is n then play next
        if(strcmp(input_command, "n") == 0)
        {
            play_next(playlist_queue, &recent_songs_stack);
        }
        //if input is p then play previous
        else if(input_command[0] == 'p')
        {
            //if it is p: then it moves forward
            if(input_command[1] == ':')
            {
                //if after p: there is 0 or \0 then it prints invalid input
                if(input_command[2] == '\0' || input_command[2] == '0')
                {
                    printf("Invalid input\n");
                }
                //else it moves forward
                else
                {
                    //pointer to first number from p:
                    char* ptr = input_command + 2 * sizeof(char);

                    //getting integer from string
                    int num_song = atoi(ptr);

                    play_previous(tmp_queue, &recent_songs_stack, num_song);
                }
            }
            //if input does not have p: then it prints invalid input
            else
            {
                printf("Invalid Input\n");
            }
        }
        //if input is e in exit
        else if(strcmp(input_command, "e") == 0)
        {
            break;
        }
        //else it prints invalid input
        else
        {
            printf("Invalid Input\n");
        }
    }

    printf("Thank you for using Music app\n");
    return EXIT_SUCCESS;
}




//    struct linked_list_node* lln = NULL;
//
//    char s[STRING_SIZE] = "hello";
//    char b[STRING_SIZE] = "bye";
//    insert_linked_list_node(&lln, b);
//    insert_linked_list_node(&lln, s);
//
//    bool test = find_linked_list_node(&lln, b);
//    if(test == true)
//    {
//        printf("Great\n");
//    }
//    else
//    {
//        printf("Not there\n");
//    }
//
//    struct stack_node* sn = NULL;
//    push_stack_node(&sn, "he");
//    push_stack_node(&sn, "me");
//
//    char str[STRING_SIZE];
//
//    pop_stack_node(&sn, str);
//    printf("The string popped is: %s\n", str);
//
//    char str2[STRING_SIZE];
//    top_stack_node(sn, str2);
//    printf("The top is: %s\n", str2);
//    top_stack_node(sn, str2);
//    printf("The top is: %s\n", str2);
//
//    pop_stack_node(&sn, str);
//    printf("The string popped is: %s\n", str);
//
//    top_stack_node(sn, str2);
//    printf("The top is: %s\n", str2);
//
//    struct queue_header* Q = queue_header_new();
//    enq_queue_header(Q, "S1");
//    enq_queue_header(Q, "S2");
//
//    printf("First element is: %s\n", Q->front_queue->song_name);
//    printf("Last element is: %s\n", Q->back_queue->song_name);
//    enq_queue_header(Q, "S3");
//    printf("Last element is: %s\n", Q->back_queue->song_name);
//    deq_queue_header(Q, str2);
//    printf("First element is: %s\n", Q->front_queue->song_name);
//    printf("Last element is: %s\n", Q->back_queue->song_name);
//    printf("Dequeued element is: %s\n", str2);

