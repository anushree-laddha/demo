#include <iostream>           // Includes the standard input/output stream library for printing and taking user input.
#include <vector>             // Includes the vector library to work with dynamic arrays (vectors).
#include <array>              // Includes the array library (though it's not directly used here, it could be for fixed size arrays).
using namespace std;          // To avoid the need to prefix everything with "std::".

// Function to print a 2D matrix (allocated, max, need matrices).
void printMatrix(vector<vector<int>> matrix)
{
    // Iterate through each row in the matrix.
    for (auto row : matrix) {
        cout << "[ ";  // Print the opening bracket for the row.
        
        // Iterate through each value in the row and print it.
        for (auto val : row)
            cout << val << " ";
        cout << "]" << endl;  // Print the closing bracket for the row and move to the next line.
    }
}

int main()
{
    int processes, numberOfResources;

    // Take input for the number of processes and number of resources.
    cout << "\nEnter total Number of processes: ";
    cin >> processes;

    cout << "\nEnter total Number of resources: ";
    cin >> numberOfResources;

    // Initialize matrices for allocated, max, and need resources.
    vector<vector<int>> allocated(processes, vector<int>(numberOfResources + 1, 0));  // +1 for process ID column.
    vector<vector<int>> max(processes, vector<int>(numberOfResources + 1, 0));  // +1 for process ID column.
    vector<vector<int>> need(processes, vector<int>(numberOfResources + 1, 0));  // +1 for process ID column.

    // Input the allocated resources for each process.
    for (int i = 0; i < processes; i++) {
        int processId;
        cout << "\nEnter process Id: ";
        cin >> processId;
        allocated[i][0] = processId;  // Store the process ID in the first column of the allocated matrix.
        max[i][0] = processId;        // Store the process ID in the first column of the max matrix.
        need[i][0] = processId;       // Store the process ID in the first column of the need matrix.

        // Input the allocated resources for each resource type (R0, R1, etc.).
        for (int j = 1; j <= numberOfResources; j++) {
            cout << "\nEnter allocated resource for R" << (j - 1) << ": ";
            cin >> allocated[i][j];  // Input and store the allocated resources for each process.
        }
    }

    // Input the maximum resources required for each process.
    for (int i = 0; i < processes; i++){
        // Input the maximum resources for each resource type.
        for (int j = 1; j <= numberOfResources; j++){
            cout << "\nFor process with processId " << max[i][0] << " max instances of resource R" << (j - 1) << ": ";
            cin >> max[i][j];  // Input and store the maximum required resources for each process.
        }
    }

    // Input the total available instances of each resource.
    vector<int> totalResources(numberOfResources);  // Vector to hold the total instances of each resource.
    for (int i = 0; i < numberOfResources; i++){
        cout << "Enter total instances of R" << i << ": ";
        cin >> totalResources[i];  // Input and store the total number of each resource.
    }

    // Calculate the need matrix: Need[i][j] = Max[i][j] - Allocated[i][j]
    for (int i = 0; i < processes; i++){
        for (int j = 1; j <= numberOfResources; j++)
            need[i][j] = max[i][j] - allocated[i][j];  // Calculate the remaining needed resources for each process.
    }

    // Print the matrices: allocated, max, and need.
    cout << "\nAllocated Matrix:\n";
    printMatrix(allocated);  // Call the printMatrix function to print the allocated matrix.

    cout << "\nMax Matrix:\n";
    printMatrix(max);  // Call the printMatrix function to print the max matrix.

    cout << "\nNeed Matrix:\n";
    printMatrix(need);  // Call the printMatrix function to print the need matrix.

    // Calculate the total allocated resources per resource type.
    vector<int> allocatedResources(numberOfResources, 0);  // Initialize a vector to hold the total allocated resources per resource type.
    
    // Loop over each resource type to calculate the total allocated resources.
    for (int i = 1; i <= numberOfResources; i++){
        for (int j = 0; j < processes; j++)
            allocatedResources[i - 1] += allocated[j][i];  // Sum up the allocated resources for each resource type.
    }

    // Calculate the available resources by subtracting allocated resources from the total resources.
    vector<int> available(numberOfResources, 0);  // Initialize a vector to hold the available resources.
    
    // Loop through each resource type to calculate available resources.
    for (int i = 0; i < numberOfResources; i++)
        available[i] = totalResources[i] - allocatedResources[i];  // Available resources = total - allocated.

    vector<bool> statusOfProcess(processes, false);  // Vector to track whether each process has been completed.

    // Print the total allocated resources and the available resources.
    cout << "\nAllocated: ";
    for (int i : allocatedResources)
        cout << i << " ";  // Print the allocated resources for each resource type.
    
    cout << "\nAvailable: ";
    for (int i : available)
        cout << i << " ";  // Print the available resources for each resource type.
    cout << endl;

    // Banker's algorithm for finding the safe sequence.
    vector<int> safeSequence;  // Vector to store the safe sequence of processes.
    bool flag1 = true;  // Flag to indicate whether any process has been executed in the current iteration.

    // While flag1 is true, keep checking for processes that can execute.
    while (flag1){
        flag1 = false;  // Reset the flag for the next iteration.
        bool flag2 = false;  // Flag to check if a process can be executed.

        // Loop through each process to check if it can be executed.
        for (int i = 0; i < processes; i++){
            if (!statusOfProcess[i]){  // Check if the process has not yet been completed.
                flag2 = false;  // Reset the flag for the current process.

                // Check if the process's resource needs are less than or equal to the available resources.
                for (int j = 1; j <= numberOfResources; j++){
                    if (need[i][j] > available[j - 1]){  // If any need is greater than the available resources, it cannot execute.
                        flag2 = true;
                        break;
                    }
                }

                // If the process can execute, mark it as completed and update the available resources.
                if (!flag2){
                    flag1 = true;  // Set the flag to true to indicate that at least one process has been executed.
                    statusOfProcess[i] = true;  // Mark the process as completed.
                    safeSequence.push_back(allocated[i][0]);  // Add the process ID to the safe sequence.

                    // Update the available resources after executing the process.
                    for (int j = 1; j <= numberOfResources; j++)
                        available[j - 1] += allocated[i][j];  // Add the allocated resources back to the available resources.

                    // Print the available resources after executing the current process.
                    cout << "Available resource after execution of process Id " << allocated[i][0] << ": ";
                    for (int j : available)
                        cout << j << " ";
                    cout << endl;
                }
            }
        }
    }

    // Check if any process has not been completed (unsafe state).
    for (bool element : statusOfProcess){
        if (!element){  // If any process is not completed, print unsafe state.
            cout << "The system is in unsafe state" << endl;
            return 0;  // Exit the program as the system is unsafe.
        }
    }

    // If all processes have been completed, print the safe sequence.
    cout << "Safe sequence: ";
    for (int seq : safeSequence)
        cout << seq << " ";  // Print the safe sequence.
    cout << endl;

    return 0;  // End of the program.
}

