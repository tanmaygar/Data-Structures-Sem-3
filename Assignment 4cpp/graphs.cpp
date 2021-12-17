#include "graphs.h"

//Function to find the index of a particular element in a vector of persons
int findPersons(std::vector<class node_persons> &array, int id)
{
    for (unsigned int i = 0; i < array.size(); i++)
    {
        if (array[i].id == id)
        {
            return i;
        }
    }
    return -1;
}

// int ProbabRec()
// {
//     int num = 2 + std::rand() % 6;
//     return num;
// }

// bool Create_EdgesProbability()
// {
//     if (std::rand() % 6 == 5)
//         return true;
//     return false;
// }

//Constructor function
Graph::Graph()
{
    //Resize AdjMatrix to fit for number of people
    AdjMatrix.resize(NUM_PEOPLE, std::vector<int>(NUM_PEOPLE));
    //Resize Statistics to fit for Susceptible, Infected, Recovered and number of loops
    Statistics.resize(2 * NUM_PEOPLE, std::vector<int>(3));

    // for (int i = 0; i < NUM_PEOPLE; i++)
    // {
    //     for (int j = 0; j < NUM_PEOPLE; j++)
    //     {
    //         AdjMatrix[i][j] = 0;
    //     }
    // }

    //Pushing people to List of All people and SusceptiblePeople
    for (int i = 0; i < NUM_PEOPLE; i++)
    {
        class node_persons node(i, SUS);
        ListPeople.push_back(node);
        SusceptiblePeople.push_back(node);
    }
}

//Function to create edges between all the nodes based on the probability
void Graph::CreateEdges()
{
    for (int i = 0; i < NUM_PEOPLE; i++)
    {
        for (int j = 0; j < NUM_PEOPLE; j++)
        {
            if (AdjMatrix[i][j] == 0 && i != j)
            {
                int CoinToss = std::rand() % 2;
                AdjMatrix[i][j] = CoinToss;
                AdjMatrix[j][i] = CoinToss;
                // if(Create_EdgesProbability() == true)
                // {
                //     AdjMatrix[i][j] = 1;
                //     AdjMatrix[j][i] = 1;
                // }
            }
        }
    }
}

//Function BFS on the Graph using Matrix to get the distance between the StartIndex and EndIndex
int Graph::BFS_Matrix(int StartIndex, int EndIndex)
{
    //Array to store the level of depth for each node from the root node
    std::array<int, NUM_PEOPLE> levels;
    //Array to store the parent of each node
    std::array<int, NUM_PEOPLE> parent;
    //Initialize the parents and levels for each node
    for (int j = 0; j < NUM_PEOPLE; j++)
    {
        levels[j] = -1;
        parent[j] = -1;
    }
    std::queue<int> Q;

    levels[StartIndex] = 0;
    Q.push(StartIndex);
    int j;
    //Looping until Q is empty
    while (Q.empty() != true)
    {
        j = Q.front();
        Q.pop();
        for (int k = 0; k < NUM_PEOPLE; k++)
        {
            if (AdjMatrix[j][k] == 1)
            {
                if (levels[k] == -1)
                {
                    levels[k] = 1 + levels[j];
                    Q.push(k);
                }
            }
        }
    }
    //The level of depth gives us the distance between root and the required node
    return levels[EndIndex];
}

//Function to run the simulation of Pandemic
void Graph::RunPandemicSimulator(std::vector<class time_stamp_event> &heap_arr)
{
    //PrintMatrix();
    //int numloop = 0;

    //Choosing our Patient0
    int Patient0 = std::rand() % NUM_PEOPLE;

    //Infecting Patient0
    ListPeople[Patient0].state = INF;
    //Generating Timestamps for its infection and recovery and inserting into the heap
    class time_stamp_event Patient0_tse_INF(0, INF, &ListPeople[Patient0]);
    class time_stamp_event Patient0_tse_REC(2 + (std::rand() % 6), REC, &ListPeople[Patient0]);
    insertHeap(heap_arr, Patient0_tse_INF);
    insertHeap(heap_arr, Patient0_tse_REC);

    //Pointer to time_stamp_event for each event deleted from minheap
    class time_stamp_event *e;

    //Pandemic Loop
    while (heap_arr.size() != 0)
    {
        e = new time_stamp_event(heap_arr[0].timestamp, heap_arr[0].situation, heap_arr[0].person);

        deleteMinHeap(heap_arr);
        //Event when Recovered
        if (e->situation == REC)
        {
            //Change the Status of Person to REC in List of People
            ListPeople[e->person->id].state = REC;
            //Pushing the person into RecoveredPeople vector
            RecoveredPeople.push_back(ListPeople[e->person->id]);
            //Removing the person from InfectedPeople Vector
            InfectedPeople.erase(InfectedPeople.begin() + findPersons(InfectedPeople, e->person->id));
        }
        //Event when Infected
        if (e->situation == INF)
        {
            //Push person into InfectedPeople Vector
            InfectedPeople.push_back(ListPeople[e->person->id]);
            //Removing the person from SusceptiblePeople Vector
            SusceptiblePeople.erase(SusceptiblePeople.begin() + findPersons(SusceptiblePeople, e->person->id));
            //Change the Status of Person to INF in List of People
            ListPeople[e->person->id].state = INF;

            //Looping through all neighbours of this person to infect them
            for (int i = 0; i < NUM_PEOPLE; i++)
            {
                if (AdjMatrix[e->person->id][i] == 1 && ListPeople[i].state == SUS)
                {
                    bool isInfected = false;
                    int j = 2;
                    for (j = 1; j < 6; j++)
                    {
                        if (std::rand() % 2 == 1)
                        {
                            isInfected = true;
                            break;
                        }
                    }
                    //isInfected = Create_EdgesProbability();
                    if (isInfected == true)
                    {
                        //Infect the Person, Generating Timestamps for its infection and recovery and inserting into the heap
                        ListPeople[i].state = INF;
                        class time_stamp_event Patient_tse_INF(j + e->timestamp, INF, &ListPeople[i]);
                        class time_stamp_event Patient_tse_REC(e->timestamp + j + 1 + (std::rand() % 5), REC, &ListPeople[i]);
                        //class time_stamp_event Patient_tse_REC(e->timestamp + j + ProbabRec(), REC, &ListPeople[i]);
                        insertHeap(heap_arr, Patient_tse_INF);
                        insertHeap(heap_arr, Patient_tse_REC);
                        std::cout << "The Distance between Patient0 : " << Patient0 << " Patient : " << i << " is: " << BFS_Matrix(Patient0, i) << std::endl;
                    }
                }
            }
        }
        //numloop++;
        Statistics[e->timestamp][SUS] = SusceptiblePeople.size();
        Statistics[e->timestamp][INF] = InfectedPeople.size();
        Statistics[e->timestamp][REC] = RecoveredPeople.size();
        //std::cout << "The Number of People Susceptible: " << countNumElements(SusceptiblePeople) << " Infected: " << InfectedPeople.size() << " Recovered: " << RecoveredPeople.size() << " At time: " << e->timestamp << " Loop: " << numloop << std::endl;
    }

    std::cout << "\n------The Statistics for entire simulation------\n";
    for (int i = 0; i <= e->timestamp; i++)
    {
        if (Statistics[i][SUS] == 0 && Statistics[i][INF] == 0 && Statistics[i][REC] == 0)
        {
            Statistics[i][SUS] = Statistics[i - 1][SUS];
            Statistics[i][INF] = Statistics[i - 1][INF];
            Statistics[i][REC] = Statistics[i - 1][REC];
        }
        std::cout << "The Number of People Susceptible: " << Statistics[i][0] << " Infected: " << Statistics[i][1] << " Recovered: " << Statistics[i][2] << " At time: " << i << std::endl;
    }
}

//Function to print the Adjacency matrix
void Graph::PrintMatrix()
{
    for (int i = 0; i < NUM_PEOPLE; i++)
    {
        std::cout << i << ": ";
        for (int j = 0; j < NUM_PEOPLE; j++)
        {
            std::cout << AdjMatrix[i][j] << " ";
        }
        std::cout << "\n";
    }
}