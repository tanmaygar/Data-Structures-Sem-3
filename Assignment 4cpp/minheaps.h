#ifndef MINHEAPS_H_INCLUDED
#define MINHEAPS_H_INCLUDED

#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <list>
#include <queue>
#include <math.h>
#include <cstdlib>
#include <ctime>
#define NUM_PEOPLE 100

//Enum to define condition for either a person or a timestamp event
enum condition_state
{
    SUS, //Susceptible
    INF, //Infected
    REC  //Recovered
};

//Class for person which will behave like a node for graph
class node_persons
{
public:
    //ID of the person
    int id;
    //Enum to store the condition of the person as Susceptible, Infected, Recovered
    enum condition_state state;
    //Constructor for the person to initialize its ID, and original state
    node_persons(int id_, enum condition_state condition_);
};

//Class for time stamps
class time_stamp_event
{
public:
    //Time stamp number which denotes the time
    int timestamp;
    //Enum to store the event type as Infected or Recovered
    enum condition_state situation;
    //Storing the pointer to the person for which the timestamp is generated
    class node_persons *person;
    //Constructor for the timestamp to initialize its ID, event type and person
    time_stamp_event(int timestamp_, enum condition_state condition_, class node_persons *person_);
};

//Function for Heapify our min heap array
void heapifyTimestamp(std::vector<class time_stamp_event> &heap_arr, int index);

//Function to insert into a heap array
void insertHeap(std::vector<class time_stamp_event> &heap_arr, class time_stamp_event &tse);

//Function to delete the top most most or the least value from our min heap array
void deleteMinHeap(std::vector<class time_stamp_event> &heap_arr);

//Function to print the Min Heap array
void printHeap(std::vector<class time_stamp_event> &heap_arr);

#endif // MINHEAPS_H_INCLUDE