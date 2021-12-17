#include "minheaps.h"

node_persons::node_persons(int id_, enum condition_state condition_)
{
    id = id_;
    state = condition_;
}

time_stamp_event::time_stamp_event(int timestamp_, enum condition_state condition_, class node_persons *person_)
{
    timestamp = timestamp_;
    situation = condition_;
    person = person_;
}

//Function for Heapify our min heap array
void heapifyTimestamp(std::vector<class time_stamp_event> &heap_arr, int index)
{
    int size = heap_arr.size();
    int smallest_element;
    int l = 2 * index + 1;
    int r = 2 * index + 2;
    if (l <= size && heap_arr[l].timestamp < heap_arr[index].timestamp)
    {
        smallest_element = l;
    }
    else
    {
        smallest_element = index;
    }
    if (r <= size && heap_arr[r].timestamp < heap_arr[smallest_element].timestamp)
    {
        smallest_element = r;
    }

    if (smallest_element != index)
    {
        std::swap(heap_arr[index], heap_arr[smallest_element]);
        heapifyTimestamp(heap_arr, smallest_element);
    }
}

//Function to insert into a heap array
void insertHeap(std::vector<class time_stamp_event> &heap_arr, class time_stamp_event &tse)
{
    //Push the new element in the end and then swap elements to satisfy min heap condition
    heap_arr.push_back(tse);
    int i = heap_arr.size() - 1;
    //going from bottom to top
    while (i != 0 && heap_arr[ceil(i / 2.0) - 1].timestamp > heap_arr[i].timestamp)
    {
        //int j = ceil(i / 2.0) - 1;
        std::swap(heap_arr[ceil(i / 2.0) - 1], heap_arr[i]);
        i = ceil(i / 2.0) - 1;
    }
}

//Function to delete the top most most or the least value from our min heap array
void deleteMinHeap(std::vector<class time_stamp_event> &heap_arr)
{
    //Replace the top most element with the last element and delete the last element and perform heapify
    heap_arr[0] = heap_arr[heap_arr.size() - 1];
    heap_arr.pop_back();
    heapifyTimestamp(heap_arr, 0);
}

//Function to print the Min Heap array
void printHeap(std::vector<class time_stamp_event> &heap_arr)
{
    for (auto i : heap_arr)
    {
        std::cout << "Timestamp: " << i.timestamp << " "
                  << "Situation: " << i.situation << " "
                  << "Person id: " << i.person->id << "\n";
    }
}