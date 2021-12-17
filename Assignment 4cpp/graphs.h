#ifndef GRAPHS_H_INCLUDED
#define GRAPHS_H_INCLUDED

#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <list>
#include <queue>
#include <math.h>
#include "minheaps.h"

//Function to find the index of a particular element in a vector of persons
int findPersons(std::vector<class node_persons> &array, int id);

int ProbabRec();

bool Create_EdgesProbability();

//Class of our Undirected 0 weights graph
class Graph
{
public:
    //Adjacency Matrix for our graph
    std::vector<std::vector<int>> AdjMatrix;

    //Vector to store the list of all People
    std::vector<class node_persons> ListPeople;

    //Vector to store the list of all Recovered People
    std::vector<class node_persons> RecoveredPeople;

    //Vector to store the list of all Susceptible People
    std::vector<class node_persons> SusceptiblePeople;

    //Vector to store the list of all Infected People
    std::vector<class node_persons> InfectedPeople;

    //2D vector to stoer the Statistics for each timestamp
    std::vector<std::vector<int>> Statistics;

    //Constructor function
    Graph();

    //Function to create edges between all the nodes based on the probability
    void CreateEdges();

    //Function BFS on the Graph using Matrix to get the distance between the StartIndex and EndIndex
    int BFS_Matrix(int StartIndex, int EndIndex);

    //Function to run the simulation of Pandemic
    void RunPandemicSimulator(std::vector<class time_stamp_event> &heap_arr);

    //Function to print the Adjacency matrix
    void PrintMatrix();
};

#endif