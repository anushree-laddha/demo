#include <stdio.h>    // Include standard input/output library for file handling and printing.
#include <stdlib.h>   // Include standard library for memory management and general functions.
#include <string.h>   // Include string library for string manipulation functions like 'strcspn'.

#define MAX_INSTRUCTIONS 100  // Define maximum number of instructions that can be loaded into memory.
#define MAX_LINE_LENGTH 50    // Define the maximum length of each instruction (line) in the program.

// Function to load the assembly language program (ALP) into memory from a file
void load_alp_to_memory(const char *file_path) {
    char main_memory[MAX_INSTRUCTIONS][MAX_LINE_LENGTH];  // Declare a 2D array to simulate main memory for instructions.
    FILE *file = fopen(file_path, "r");  // Open the file in read mode.
    
    if (file == NULL) {  // If the file can't be opened (maybe it doesn't exist).
        printf("Error: Input file not found.\n");  // Print an error message.
        return;  // Exit the function if the file cannot be opened.
    }

    int address = 0;  // Initialize the address (index) in memory to store instructions.
    
    // Read lines from the file and store them in memory until the end of the file or maximum instructions are reached.
    while (fgets(main_memory[address], MAX_LINE_LENGTH, file) != NULL && address < MAX_INSTRUCTIONS) {
        // Remove the newline character at the end of the string (if present).
        main_memory[address][strcspn(main_memory[address], "\n")] = '\0';  
        address++;  // Increment the address for the next instruction.
    }
    
    fclose(file);  // Close the file after reading all instructions.

    // Print the ALP program loaded into main memory.
    printf("ALP Program loaded into main memory:\n");
    
    // Loop through the loaded instructions and print them with their respective addresses.
    for (int i = 0; i < address; i++) {
        printf("Address %d: %s\n", i, main_memory[i]);  // Print the instruction stored at each address.
    }
}

int main() {
    load_alp_to_memory("alp_program.txt");  // Call the function to load the program from the file "alp_program.txt".
    return 0;  // Return 0 to indicate the program finished successfully.
}
