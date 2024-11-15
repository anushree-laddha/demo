#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class Job {
private:
    string Memory[100][4];  // Memory (100 locations, each of 4 chars)
    string IR[4];           // Instruction Register (4 chars)
    int IC[2];              // Instruction Counter
    string buff;            // Buffer to hold each line of the input job

public:
    Job();  // Constructor to initialize memory and IC
    void LOAD();  // Load the job into memory
    void INITIALIZE();  // Initialize memory
    void DISPLAY_MEMORY();  // Display the memory contents for debugging
};

// Constructor: Initializes all registers and memory
Job::Job() {
    IC[0] = 0;
    IC[1] = 0;
    INITIALIZE();
}

// Initialize memory with empty spaces
void Job::INITIALIZE() {
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 4; j++) {
            Memory[i][j] = " ";  // Fill memory with blank spaces initially
        }
    }
}

// Load the job into memory (hardcoded input)
void Job::LOAD() {
    // Hardcoded job input in $AMJ format
    string input[] = {
        "$AMJ000100030001",  // Job ID = 0001, TTL = 0001, TLL = 0003
        "GD20",  // Valid instruction (GD20)
        "PD20",  // Valid instruction (PD20)
        "H000",   // Halt instruction (H000)
        "$DTA",   // Data section starts here
        "Hello World",  // Data to be stored at memory location 20
        "$END0001"  // End of job input
    };

    int m = 0;  // Memory index for loading instructions

    // Parse the hardcoded input job
    for (int i = 0; i < sizeof(input) / sizeof(input[0]); i++) {
        string s = input[i].substr(0, 4);
        if (s == "$AMJ") {
            // Initialize job from $AMJ
            IC[0] = 0;
            IC[1] = 0;
        }
        else if (s == "$DTA") {
            // Data section starts here
            break;  // Stop loading instructions, start data section
        }
        else if (s == "$END") {
            // End of the job input, stop
            continue;
        }
        else {
            // Load instructions into memory
            int j = 0;
            for (int k = 0; k < input[i].size(); k++) {
                if (j == 4) {
                    j = 0;
                    m++;
                }
                if (input[i][k] == 'H') {
                    int k = 1;
                    Memory[m][j] = input[i][k];
                    for (k = 1; k <= 3; k++) {
                        Memory[m][k] = ' ';
                    }
                    j = k - 1;
                } else {
                    Memory[m][j] = input[i][k];
                }
                j++;
            }
            m = m + 1;
        }
    }

    // Now, store data after $DTA into memory starting at address provided by GD instruction
    string data = input[5];  // Data to be stored in memory starting at location 20
    int address = 20;        // This address is taken from GD20 (hardcoded)

    int dataIndex = 0;
    for (int i = address; i < address + data.size(); i++) {
        Memory[i][0] = string(1, data[dataIndex++]);
        Memory[i][1] = " ";
        Memory[i][2] = " ";
        Memory[i][3] = " ";
    }

    // Display the memory contents for debugging purposes
    DISPLAY_MEMORY();
}

// Display memory contents (for debugging purposes)
void Job::DISPLAY_MEMORY() {
    cout << "Memory Contents after Loading:" << endl;
    for (int i = 0; i < 100; i++) {
        cout << "Memory[" << i << "] ";
        for (int j = 0; j < 4; j++) {
            cout << Memory[i][j];
        }
        cout << endl;
    }
}

int main() {
    Job job;  // Create job object
    job.LOAD();  // Load job into memory
    return 0;
}
