/* Tanmay Garg CS20BTECH11063 Data Structures Assignment 4
*
*   To compile and run the program put the following commands in terminal or powershell:
*   
*   $ g++ *.cpp *.h -o main
*   $ ./main
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <list>
#include "minheaps.h"
#include "graphs.h"

int main()
{
    //Pandemic Graph Simulation
    class Graph PandemicGraph;

    //Min Heap Array for timestamps
    std::vector<class time_stamp_event> TSE_HeapArray;

    std::srand(std::time(NULL));

    //Create Edges in the graph
    PandemicGraph.CreateEdges();
    //Run the Simulation
    PandemicGraph.RunPandemicSimulator(TSE_HeapArray);

    std::cout<<"End of simulation"<<std::endl;
    
    return EXIT_SUCCESS;
}

//Debug Code
// std::vector<class time_stamp_event> HeapArray;
    // class node_persons np(1, SUS);
    // class time_stamp_event t1(2, INF, &np);
    // class time_stamp_event t2(3, INF, &np);
    // class time_stamp_event t3(3, REC, &np);
    // class time_stamp_event t4(1, INF, &np);

    // insertHeap(HeapArray, t1);
    // insertHeap(HeapArray, t2);
    // insertHeap(HeapArray, t3);
    // insertHeap(HeapArray, t4);

    // deleteMinHeap(HeapArray);

    // printHeap(HeapArray);