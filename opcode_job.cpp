#include <iostream>
#include <cstring>
#include <cctype>  // For isdigit() function
using namespace std;

class Job {
private:
    char M[300][4];  // Memory (300 locations, each of 4 chars)
    char IR[4];      // Instruction Register (4 chars)
    int IC;          // Instruction Counter
    int PI;          // Program Interrupt (PI = 1 for opcode error, PI = 2 for operand error)
    int TTL;         // Time Limit (TTL) for the job

public:
    Job();  // Constructor
    void LOAD();  // Load the job into memory
    void EXECUTEUSERPROGRAM();  // Execute the user program
    void MOS();  // Handle interrupts (in case of opcode or operand errors)
    void TERMINATE();  // Terminate the job with an error message
};

// Constructor: Initializes all registers and memory
Job::Job() {
    IC = 0;
    PI = 0;
    TTL = 0;
    memset(M, 0, sizeof(M));  // Initialize memory with zero
    memset(IR, 0, sizeof(IR));  // Initialize IR with zero
}

// Load the job into memory (hardcoded job input in $AMJ format)
void Job::LOAD() {
    // Hardcoded job input in $AMJ format
    string input[] = {
        "$AMJ000100030001",  // Job ID = 0001, TTL = 0001, TLL = 0003
        "GD10",  // Valid opcode with numeric operand
        "PL10",  // Invalid operand (alphanumeric operand)
        "H000",  // Valid opcode
        "$END0001"  // End of job input
    };

    // Parse $AMJ line to initialize TTL
    TTL = stoi(input[0].substr(4, 4));  // Get TTL from $AMJ line

    // Store instructions in memory (skip the $AMJ and $END lines)
    for (int i = 1; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            M[i-1][j] = input[i][j];  // Copy the instruction into memory
        }
    }
}

// Execute the user program and check for opcode and operand errors
void Job::EXECUTEUSERPROGRAM() {
    while (true) {
        // Fetch the next instruction
        for (int i = 0; i < 4; i++) {
            IR[i] = M[IC][i];
        }

        // Display the instruction being executed
        cout << "Executing Instruction: " << IR[0] << IR[1] << IR[2] << IR[3] << endl;

        // Check for opcode error (invalid instruction)
        if (IR[0] != 'G' && IR[0] != 'P' && IR[0] != 'L' && IR[0] != 'S' && IR[0] != 'C' && IR[0] != 'B') {
            PI = 1;  // Opcode error
            MOS();  // Raise interrupt for opcode error
            break;
        }

        if (IR[1] != 'D' && IR[1] != 'R' && IR[1] != 'T') {
            PI = 1;  // Opcode error
            MOS();  // Raise interrupt for opcode error
            break;
        }

        // Check for operand error (if the last two characters are not numeric)
        if (!isdigit(IR[2]) || !isdigit(IR[3])) {
            PI = 2;  // Operand error
            MOS();  // Raise interrupt for operand error
            break;
        }

        // Halt if instruction is 'H'
        if (IR[0] == 'H') {
            cout << "Halting execution as per 'H' instruction." << endl;
            break;
        }

        IC++;  // Increment instruction counter
    }
}

// Handle interrupts (in this case, opcode or operand error)
void Job::MOS() {
    if (PI == 1) {
        TERMINATE();  // Terminate with opcode error
    } else if (PI == 2) {
        TERMINATE();  // Terminate with operand error
    }
}

// Terminate the job with an error message
void Job::TERMINATE() {
    if (PI == 1) {
        cout << "Error: Operation Code Error (PI = 1)" << endl;
    } else if (PI == 2) {
        cout << "Error: Operand Error (PI = 2)" << endl;
    }
    cout << "IC: " << IC << ", IR: " << IR[0] << IR[1] << IR[2] << IR[3] << endl;
    cout << "TTL: " << TTL << endl;
}

int main() {
    Job j;
    j.LOAD();  // Load the job into memory
    j.EXECUTEUSERPROGRAM();  // Start executing the user program
    return 0;
}
