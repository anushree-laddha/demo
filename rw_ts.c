#include <iostream>        // Includes the standard input-output library.
#include <vector>          // Includes the vector library for dynamic arrays.
#include <algorithm>       // Includes algorithms like sorting and searching.
#include <queue>           // Includes the queue library, used for Round Robin scheduling.
using namespace std;      // Use the standard namespace to avoid using `std::` repeatedly.


// Structure to represent a Process.
struct Process {
    int id;              // Unique identifier for the process.
    int arrivalTime;     // Arrival time of the process.
    int burstTime;       // Total burst time required by the process.
    int priority;        // Priority of the process (lower values indicate higher priority).
    int remainingTime;   // Remaining burst time (used in preemptive scheduling).
    int finishTime;      // Time when the process finishes execution.
    int turnaroundTime;  // Time from arrival to completion (turnaround time = finish time - arrival time).
    int waitingTime;     // Time a process has to wait before being executed (waiting time = turnaround time - burst time).
};

// Function to display the results for all processes.
void displayResults(vector<Process>& processes) {
    cout << "\nProcess ID | Finish Time | Turnaround Time | Waiting Time\n";
    for (auto& p : processes) {  // Loop over each process.
        // Print the process id, finish time, turnaround time, and waiting time.
        cout << "    " << p.id << "       |      " << p.finishTime << "       |       " << p.turnaroundTime << "       |     " << p.waitingTime << "\n";
    }
}

// FCFS (First Come First Serve) Scheduling function.
void FCFS(vector<Process>& processes) {
    int currentTime = 0;   // Initialize the current time to 0.
    for (auto& p : processes) {    // Loop through each process.
        if (currentTime < p.arrivalTime)  // If current time is less than the arrival time, move current time to the arrival time.
            currentTime = p.arrivalTime;
        
        p.finishTime = currentTime + p.burstTime;  // Calculate finish time of the process.
        p.turnaroundTime = p.finishTime - p.arrivalTime; // Calculate turnaround time.
        p.waitingTime = p.turnaroundTime - p.burstTime;  // Calculate waiting time.
        
        currentTime = p.finishTime;  // Update the current time to the finish time of the process.
    }
    cout << "\nFirst Come First Serve (FCFS):";  // Print the algorithm name.
    displayResults(processes);  // Display the results.
}

// Non-Preemptive Priority Scheduling.
void priorityNonPreemptive(vector<Process> processes) {
    vector<Process> result;   // Vector to store the processed results.
    int currentTime = 0;  // Initialize current time to 0.

    // Sort processes by arrival time in ascending order.
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrivalTime < b.arrivalTime;
    });

    while (!processes.empty()) {  // Loop while there are processes to execute.
        vector<Process> readyQueue;  // Vector to store processes that have arrived and can be executed.

        // Check which processes have arrived by the current time and add them to the ready queue.
        for (const auto& p : processes) {
            if (p.arrivalTime <= currentTime) {
                readyQueue.push_back(p);
            }
        }

        if (readyQueue.empty()) {  // If no process is ready, increment current time.
            currentTime++;
            continue;
        }

        // Select the process with the highest priority (lowest priority number) from the ready queue.
        auto it = min_element(readyQueue.begin(), readyQueue.end(), [](const Process& a, const Process& b) {
            return a.priority < b.priority;
        });
        
        Process currentProcess = *it;  // Select the process to execute.
        currentProcess.finishTime = currentTime + currentProcess.burstTime;  // Calculate finish time.
        currentProcess.turnaroundTime = currentProcess.finishTime - currentProcess.arrivalTime;  // Calculate turnaround time.
        currentProcess.waitingTime = currentProcess.turnaroundTime - currentProcess.burstTime;  // Calculate waiting time.
        
        currentTime = currentProcess.finishTime;  // Update the current time to the process's finish time.
        
        result.push_back(currentProcess);  // Add the completed process to the result.

        // Remove the completed process from the list of processes.
        processes.erase(remove_if(processes.begin(), processes.end(), [&](const Process& p) {
            return p.id == currentProcess.id;
        }), processes.end());
    }
    
    cout << "\nPriority (Non-Preemptive):";  // Print the algorithm name.
    displayResults(result);  // Display the results.
}

// Preemptive Priority Scheduling.
void priorityPreemptive(vector<Process> processes) {
    vector<Process> result;  // Vector to store the processed results.
    int currentTime = 0, completed = 0;  // Initialize current time to 0 and completed processes to 0.
    int n = processes.size();  // Get the total number of processes.
    vector<int> remainingBurstTime(n);  // Vector to track the remaining burst time for each process.

    for (int i = 0; i < n; i++) remainingBurstTime[i] = processes[i].burstTime;  // Initialize remaining burst time for each process.

    while (completed != n) {  // Loop until all processes are completed.
        int idx = -1, highestPriority = 1e9;  // Variable to track the highest priority process.

        // Loop through all processes to find the one with the highest priority and remaining burst time.
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && remainingBurstTime[i] > 0 && processes[i].priority < highestPriority) {
                highestPriority = processes[i].priority;
                idx = i;
            }
        }

        if (idx != -1) {  // If a process is found, execute it.
            remainingBurstTime[idx]--;  // Decrease the remaining burst time.
            currentTime++;  // Increment the current time.
            
            // If the process has completed, update its finish time, turnaround time, and waiting time.
            if (remainingBurstTime[idx] == 0) {
                completed++;
                processes[idx].finishTime = currentTime;
                processes[idx].turnaroundTime = currentTime - processes[idx].arrivalTime;
                processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
                result.push_back(processes[idx]);
            }
        } else {
            currentTime++;  // If no process is ready, increment the current time.
        }
    }

    cout << "\nPriority Scheduling (Preemptive):";  // Print the algorithm name.
    displayResults(result);  // Display the results.
}

// Non-Preemptive Shortest Job First (SJF) Scheduling.
void SJFNonPreemptive(vector<Process> processes) {
    vector<Process> result;   // Vector to store the processed results.
    int currentTime = 0;      // Initialize current time to 0.

    // Sort processes by arrival time in ascending order.
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrivalTime < b.arrivalTime;
    });

    while (!processes.empty()) {  // Loop while there are processes to execute.
        vector<Process> readyQueue;  // Vector to store processes that have arrived and can be executed.

        // Check which processes have arrived by the current time and add them to the ready queue.
        for (const auto& p : processes) {
            if (p.arrivalTime <= currentTime) {
                readyQueue.push_back(p);
            }
        }

        if (readyQueue.empty()) {  // If no process is ready, increment current time.
            currentTime++;
            continue;
        }

        // Select the process with the shortest burst time from the ready queue.
        auto it = min_element(readyQueue.begin(), readyQueue.end(), [](const Process& a, const Process& b) {
            return a.burstTime < b.burstTime;
        });

        Process currentProcess = *it;  // Select the process to execute.
        currentProcess.finishTime = currentTime + currentProcess.burstTime;  // Calculate finish time.
        currentProcess.turnaroundTime = currentProcess.finishTime - currentProcess.arrivalTime;  // Calculate turnaround time.
        currentProcess.waitingTime = currentProcess.turnaroundTime - currentProcess.burstTime;  // Calculate waiting time.
        
        currentTime = currentProcess.finishTime;  // Update the current time to the process's finish time.
        
        result.push_back(currentProcess);  // Add the completed process to the result.

        // Remove the completed process from the list of processes.
        processes.erase(remove_if(processes.begin(), processes.end(), [&](const Process& p) {
            return p.id == currentProcess.id;
        }), processes.end());
    }
    
    cout << "\nShortest Job First (Non-Preemptive):";  // Print the algorithm name.
    displayResults(result);  // Display the results.
}

// Preemptive Shortest Job First (SJF) Scheduling.
void SJFPreemptive(vector<Process> processes) {
    vector<Process> result;  // Vector to store the processed results.
    int currentTime = 0, completed = 0;  // Initialize current time to 0 and completed processes to 0.
    int n = processes.size();  // Get the total number of processes.
    vector<int> remainingBurstTime(n);  // Vector to track the remaining burst time for each process.

    for (int i = 0; i < n; i++) remainingBurstTime[i] = processes[i].burstTime;  // Initialize remaining burst time for each process.

    while (completed != n) {  // Loop until all processes are completed.
        int idx = -1, minRemainingTime = 1e9;  // Variables to track the process with the shortest remaining burst time.

        // Loop through all processes to find the one with the shortest remaining burst time.
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && remainingBurstTime[i] > 0 && remainingBurstTime[i] < minRemainingTime) {
                minRemainingTime = remainingBurstTime[i];
                idx = i;
            }
        }

        if (idx != -1) {  // If a process is found, execute it.
            remainingBurstTime[idx]--;  // Decrease the remaining burst time.
            currentTime++;  // Increment the current time.
            
            // If the process has completed, update its finish time, turnaround time, and waiting time.
            if (remainingBurstTime[idx] == 0) {
                completed++;
                processes[idx].finishTime = currentTime;
                processes[idx].turnaroundTime = currentTime - processes[idx].arrivalTime;
                processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
                result.push_back(processes[idx]);
            }
        } else {
            currentTime++;  // If no process is ready, increment the current time.
        }
    }

    cout << "\nShortest Job First (Preemptive):";  // Print the algorithm name.
    displayResults(result);  // Display the results.
}

// Round Robin Scheduling function.
void roundRobin(vector<Process>& processes, int quantum) {
    int currentTime = 0;  // Initialize current time to 0.
    int completed = 0;    // Initialize completed processes to 0.
    int n = processes.size();  // Get the total number of processes.
    queue<int> q;  // Queue to simulate Round Robin scheduling.
    vector<bool> inQueue(n, false);  // Track which processes are already in the queue.
    vector<int> remainingBurstTime(n);  // Track the remaining burst time for each process.

    for (int i = 0; i < n; i++) 
        remainingBurstTime[i] = processes[i].burstTime;  // Initialize remaining burst time for each process.

    // Start with processes that have arrived at time 0.
    for (int i = 0; i < n; i++) {
        if (processes[i].arrivalTime == 0) {
            q.push(i);  // Add the process to the queue.
            inQueue[i] = true;
        }
    }

    while (completed < n) {  // Loop until all processes are completed.
        if (q.empty()) {  // If no processes are in the queue, increment the current time and check for newly arrived processes.
            currentTime++;
            for (int i = 0; i < n; i++) {
                if (!inQueue[i] && processes[i].arrivalTime <= currentTime) {
                    q.push(i);  // Add the newly arrived process to the queue.
                    inQueue[i] = true;
                }
            }
            continue;
        }

        int i = q.front();  // Get the next process from the queue.
        q.pop();  // Remove it from the queue.

        // Execute the current process.
        if (remainingBurstTime[i] > quantum) {  // If the process has more remaining time than the quantum, execute it for the quantum duration.
            currentTime += quantum;
            remainingBurstTime[i] -= quantum;
        } else {  // If the process has less remaining time than the quantum, execute it until completion.
            currentTime += remainingBurstTime[i];
            processes[i].finishTime = currentTime;
            processes[i].turnaroundTime = currentTime - processes[i].arrivalTime;
            processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
            remainingBurstTime[i] = 0;
            completed++;  // Increment the completed processes count.
        }

        // Check for newly arrived processes and add them to the queue.
        for (int j = 0; j < n; j++) {
            if (!inQueue[j] && processes[j].arrivalTime <= currentTime && remainingBurstTime[j] > 0) {
                q.push(j);  // Add the newly arrived process to the queue.
                inQueue[j] = true;
            }
        }

        // Re-add current process if it still has remaining burst time.
        if (remainingBurstTime[i] > 0) {
            q.push(i);  // Re-insert the process back into the queue if it’s not yet finished.
        }
    }

    cout << "\nRound Robin Scheduling:";  // Print the algorithm name.
    displayResults(processes);  // Display the results.
}


// Main function to execute the scheduling algorithms.
int main() {
    int n, quantum;
    cout << "Enter the number of processes: ";
    cin >> n;  // Get the number of processes from the user.
    vector<Process> processes(n);  // Create a vector of Process structures.

    for (int i = 0; i < n; i++) {
        cout << "\nEnter arrival time, burst time, and priority for process " << i + 1 << ": ";
        processes[i].id = i;  // Assign process ID.
        cin >> processes[i].arrivalTime >> processes[i].burstTime >> processes[i].priority;  // Get arrival time, burst time, and priority for each process.
        processes[i].remainingTime = processes[i].burstTime;  // Initialize the remaining burst time.
    }

    cout << "Enter the time quantum for Round Robin: ";
    cin >> quantum;  // Get the time quantum for Round Robin scheduling.

    FCFS(processes);  // Call FCFS scheduling function.
    SJFNonPreemptive(processes);  // Call SJF Non-Preemptive scheduling function.
    SJFPreemptive(processes);  // Call SJF Preemptive scheduling function.
    priorityPreemptive(processes);  // Call Priority Preemptive scheduling function.
    priorityNonPreemptive(processes);  // Call Priority Non-Preemptive scheduling function.
    roundRobin(processes, quantum);  // Call Round Robin scheduling function.

    return 0;  // End of the program.
}

