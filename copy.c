#include <stdio.h>   // Include standard input/output library for file operations.
#include <stdlib.h>  // Include standard library for memory management and program exit functions.


// Function to copy the contents of a file character by character
void copyFileCharacterByCharacter(const char *sourceFile, const char *destFile) {
    // Open the source file in read mode ('r') and the destination file in write mode ('w')
    FILE *src = fopen(sourceFile, "r");
    FILE *dest = fopen(destFile, "w");

    // Check if either of the files could not be opened
    if (src == NULL || dest == NULL) {
        perror("Error opening file");  // Print error message if file opening fails
        exit(1);                       // Exit the program with an error code
    }

    char ch;  // Variable to store each character read from the source file
    // Loop through the source file and read each character until reaching the end of the file (EOF)
    while ((ch = fgetc(src)) != EOF) {
        fputc(ch, dest);  // Write the character to the destination file
    }

    // Print success message indicating the file has been copied character by character
    printf("File copied character by character successfully.\n");

    // Close both source and destination files
    fclose(src);
    fclose(dest);
}


// Function to copy the contents of a file line by line
void copyFileLineByLine(const char *sourceFile, const char *destFile) {
    // Open the source file in read mode ('r') and the destination file in write mode ('w')
    FILE *src = fopen(sourceFile, "r");
    FILE *dest = fopen(destFile, "w");

    // Check if either of the files could not be opened
    if (src == NULL || dest == NULL) {
        perror("Error opening file");  // Print error message if file opening fails
        exit(1);                       // Exit the program with an error code
    }

    char line[1024];  // Array to store each line read from the source file
    // Loop through the source file and read each line until reaching the end of the file (EOF)
    while (fgets(line, sizeof(line), src) != NULL) {
        fputs(line, dest);  // Write the line to the destination file
    }

    // Print success message indicating the file has been copied line by line
    printf("File copied line by line successfully.\n");

    // Close both source and destination files
    fclose(src);
    fclose(dest);
}


int main() {
    // Declare arrays to store file names for source and destination files
    char sourceFile[100];        // Array to hold the source file name
    char destFileChar[100];      // Array to hold the destination file name for character-by-character copy
    char destFileLine[100];      // Array to hold the destination file name for line-by-line copy

    // Prompt the user to enter the source file name
    printf("Enter the name of the source file: ");
    scanf("%s", sourceFile);     // Read the source file name from user input

    // Prompt the user to enter the destination file name for character-by-character copy
    printf("Enter the name of the destination file for character-by-character copy: ");
    scanf("%s", destFileChar);   // Read the destination file name from user input

    // Prompt the user to enter the destination file name for line-by-line copy
    printf("Enter the name of the destination file for line-by-line copy: ");
    scanf("%s", destFileLine);   // Read the destination file name from user input

    // Print a message indicating the start of character-by-character copy
    printf("\nCopying file character by character...\n");
    copyFileCharacterByCharacter(sourceFile, destFileChar);  // Call function to copy file character by character

    // Print a message indicating the start of line-by-line copy
    printf("\nCopying file line by line...\n");
    copyFileLineByLine(sourceFile, destFileLine);  // Call function to copy file line by line

    return 0;  // Exit the program successfully
}


