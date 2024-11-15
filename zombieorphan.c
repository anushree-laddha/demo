#include <stdio.h>      // Include the standard input/output library
#include <stdlib.h>     // Include the standard library for functions like exit()
#include <unistd.h>     // Include the POSIX library for system calls like fork() and sleep()
#include <sys/wait.h>   // Include the library for process control, specifically wait() function


// Function to create a zombie process
void createZombieProcess() {
    pid_t pid = fork();  // Fork a new process, creating a child process

    if (pid < 0) {  // If fork() fails
        perror("Fork failed");  // Print an error message
        exit(1);  // Exit the program with an error code
    }

    if (pid > 0) {  // Parent process
        printf("Parent process: Zombie process created. PID = %d\n", pid);  // Print a message with the child's PID
        sleep(10);  // Parent process sleeps, giving time for the child process to terminate and become a zombie
    } 
    else {  // Child process
        printf("Child process exiting to become zombie.\n");  // Child process prints a message
        exit(0);  // Child process exits, leaving a zombie process
    }
}


// Function to create an orphan process
void createOrphanProcess() {
    pid_t pid = fork();  // Fork a new process

    if (pid < 0) {  // If fork() fails
        perror("Fork failed");  // Print an error message
        exit(1);  // Exit the program with an error code
    }

    if (pid > 0) {  // Parent process
        printf("Parent process exiting to create orphan process.\n");  // Print a message before exiting
        exit(0);  // Parent process exits, leaving the child as an orphan process
    } 
    else {  // Child process
        sleep(5);  // Child process sleeps for a while, allowing parent to exit first
        printf("Child process (orphan) continuing after parent termination. PID = %d\n", getpid());  // Print a message showing the child process continues as an orphan
    }
}


// Function to calculate the sum of even and odd numbers in an array
void sumEvenOdd(int arr[], int size) {
    pid_t pid = fork();  // Fork a new process

    if (pid < 0) {  // If fork() fails
        perror("Fork failed");  // Print an error message
        exit(1);  // Exit the program with an error code
    }

    if (pid > 0) {  // Parent process
        int evenSum = 0;  // Variable to store sum of even numbers
        for (int i = 0; i < size; i++) {  // Loop through the array
            if (arr[i] % 2 == 0) {  // Check if the number is even
                evenSum += arr[i];  // Add even number to evenSum
            }
        }
        printf("Parent process: Sum of even numbers = %d\n", evenSum);  // Print sum of even numbers
        wait(NULL);  // Wait for the child process to finish
    } 
    else {  // Child process
        int oddSum = 0;  // Variable to store sum of odd numbers
        for (int i = 0; i < size; i++) {  // Loop through the array
            if (arr[i] % 2 != 0) {  // Check if the number is odd
                oddSum += arr[i];  // Add odd number to oddSum
            }
        }
        printf("Child process: Sum of odd numbers = %d\n", oddSum);  // Print sum of odd numbers
        exit(0);  // Exit the child process
    }
}


int main() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};  // Declare an array of integers
    int size = sizeof(arr) / sizeof(arr[0]);  // Calculate the size of the array (number of elements)
    printf("Calculating sum of even and odd numbers:\n");  // Print a message indicating the task
    sumEvenOdd(arr, size);  // Call the function to calculate the sum of even and odd numbers


    printf("\nCreating a zombie process:\n");  // Print a message indicating zombie process creation
    createZombieProcess();  // Call the function to create a zombie process
    sleep(5);   // Sleep for 5 seconds to let the zombie process remain in existence

    printf("\nCreating an orphan process:\n");  // Print a message indicating orphan process creation
    createOrphanProcess();  // Call the function to create an orphan process
    return 0;  // Return 0 to indicate successful execution of the program
}


