#include<iostream>              // Include standard input/output stream library for printing and taking input.
#include<vector>                // Include the vector library to work with dynamic arrays (vectors).

using namespace std;           // To avoid needing to prefix every statement with "std::".

// Function to detect deadlock using the Banker's algorithm approach.
bool detectDead(vector<vector<int>> &alloc, vector<vector<int>> request, vector<int> avail)
{
    int process = alloc.size();      // Get the number of processes (rows in allocation matrix).
    int resources = avail.size();    // Get the number of resource types (size of the available resources vector).
    
    vector<bool> finish(process, false);   // Vector to track whether each process is finished (initially all set to false).
    
    while(true)
    {
        bool anyFinish = false;  // Flag to check if any process has finished in the current iteration.
        
        // Iterate over each process.
        for(int i = 0; i < process; i++)
        {
            if(!finish[i])    // If the process i is not finished yet.
            {
                bool canFinish = true;   // Flag to check if process i can finish based on the resource request.

                // Check if the requested resources for process i are less than or equal to the available resources.
                for(int j = 0; j < resources; j++)
                {
                    if(request[i][j] > avail[j])    // If the request exceeds available resources, it can't finish.
                    {
                        canFinish = false;   // Set canFinish to false if the request is too high.
                        break;   // Exit the loop, as we already know it can't finish.
                    }
                }

                if(canFinish)    // If the process can finish (i.e., all resource requests can be satisfied).
                {
                    // For all resource types, add the allocated resources of process i to available resources.
                    for(int j = 0; j < resources; j++)
                    {
                        avail[j] += alloc[i][j];    // Add the resources allocated to process i to available resources.
                    }
                    anyFinish = true;    // Set anyFinish to true to indicate that at least one process has finished.
                    finish[i] = true;    // Mark the current process i as finished.
                }
            }
        }
        
        if(!anyFinish)   // If no process was able to finish in the current iteration, break the loop.
        { 
            break;
        }
    }
    
    // Check if all processes are finished.
    for(bool f : finish)
    {
        if(!f) return true;    // If any process is not finished, return true (indicating deadlock).
    }
    
    return false;   // If all processes are finished, return false (no deadlock).
}

int main()
{
    // Initialize the allocation matrix with values.
    vector<vector<int>> alloc = {
        {0, 1, 0},  // Process 0 has 0 instances of R0, 1 instance of R1, and 0 instances of R2.
        {2, 0, 0},  // Process 1 has 2 instances of R0, 0 instances of R1, and 0 instances of R2.
        {3, 0, 2},  // Process 2 has 3 instances of R0, 0 instances of R1, and 2 instances of R2.
        {2, 1, 1},  // Process 3 has 2 instances of R0, 1 instance of R1, and 1 instance of R2.
        {0, 0, 2}   // Process 4 has 0 instances of R0, 0 instances of R1, and 2 instances of R2.
    };
    
    // Initialize the request matrix with values (requests made by each process).
    vector<vector<int>> request = {
        {0, 0, 0},  // Process 0 requests 0 instances of R0, 0 instances of R1, and 0 instances of R2.
        {2, 0, 2},  // Process 1 requests 2 instances of R0, 0 instances of R1, and 2 instances of R2.
        {0, 0, 0},  // Process 2 requests 0 instances of R0, 0 instances of R1, and 0 instances of R2.
        {1, 0, 0},  // Process 3 requests 1 instance of R0, 0 instances of R1, and 0 instances of R2.
        {0, 0, 2}   // Process 4 requests 0 instances of R0, 0 instances of R1, and 2 instances of R2.
    };
    
    // Initialize the available resources vector (initially all set to 0).
    vector<int> avail = {0, 0, 0};  // There are initially 0 available resources of each type.

    // Call the detectDead function to check if there is a deadlock.
    if(detectDead(alloc, request, avail))
    {
        cout << "detected deadlock" << endl;   // If deadlock is detected, print "detected deadlock".
    }
    else
    {
        cout << "no detected deadlock" << endl;  // If no deadlock is detected, print "no detected deadlock".
    }

    return 0;   // End of the program.
}