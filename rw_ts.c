#include <pthread.h>      // Include the POSIX threads library for thread management
#include <semaphore.h>     // Include the POSIX semaphore library for synchronization
#include <stdio.h>         // Include the standard input/output library
#include <stdlib.h>        // Include the standard library for memory allocation, etc.

// Declare semaphores for controlling access to the shared resource (cnt)
sem_t wrt;                  // Semaphore for controlling write access (mutex for writers)
sem_t reader_count_access;  // Semaphore for controlling access to reader count (used by readers)

// Global variables for managing the shared resource (cnt) and reader count
int cnt = 1;                 // Shared counter initialized to 1
int numreader = 0;           // Number of readers currently reading

// Writer thread function
void *writer(void *wno) {
    int writer_id = *((int *)wno);  // Extract the writer ID passed as argument
    
    sem_wait(&wrt);  // Acquire the write semaphore, ensuring mutual exclusion for writers
    
    cnt = cnt * 2;  // Modify the shared resource (cnt) by multiplying it by 2
    printf("Writer %d modified cnt to %d\n", writer_id, cnt);  // Print the change made by the writer
    
    sem_post(&wrt);  // Release the write semaphore, allowing other writers to access the resource
    
    printf("Writer %d is leaving.\n", writer_id);  // Print when the writer leaves
    
    return NULL;  // Return from the function
}

// Reader thread function
void *reader(void *rno) {
    int reader_id = *((int *)rno);  // Extract the reader ID passed as argument
    
    sem_wait(&reader_count_access);  // Lock the reader count to modify the number of readers
    
    numreader++;  // Increment the number of readers
    if (numreader == 1) {  // If this is the first reader, acquire the write semaphore (to prevent writing)
        sem_wait(&wrt);  // Block writers
    }
    
    sem_post(&reader_count_access);  // Release the reader count semaphore
    
    printf("Reader %d: read cnt as %d\n", reader_id, cnt);  // Read and print the value of cnt
    
    sem_wait(&reader_count_access);  // Lock the reader count again to safely modify it
    
    numreader--;  // Decrement the number of readers
    if (numreader == 0) {  // If this is the last reader, release the write semaphore (allowing writers to write)
        sem_post(&wrt);  // Unblock writers
    }
    
    sem_post(&reader_count_access);  // Release the reader count semaphore
    
    printf("Reader %d is leaving.\n", reader_id);  // Print when the reader leaves
    
    return NULL;  // Return from the function
}

int main() {
    int num_readers, num_writers;  // Variables to store the number of readers and writers
    
    printf("Enter the number of readers: ");  // Prompt for the number of readers
    scanf("%d", &num_readers);  // Read the number of readers from the user
    
    printf("Enter the number of writers: ");  // Prompt for the number of writers
    scanf("%d", &num_writers);  // Read the number of writers from the user
    
    pthread_t read[num_readers], write[num_writers];   // Declare arrays to store reader and writer threads
    
    // Initialize the semaphores
    sem_init(&wrt, 0, 1);                  // Initialize the write semaphore (wrt) with value 1 (mutex)
    sem_init(&reader_count_access, 0, 1);  // Initialize the semaphore for controlling reader count access
    
    // Allocate memory for reader and writer IDs
    int *reader_ids = (int *)malloc(num_readers * sizeof(int));  // Allocate memory for reader IDs
    int *writer_ids = (int *)malloc(num_writers * sizeof(int));  // Allocate memory for writer IDs
    
    // Create reader threads
    for (int i = 0; i < num_readers; i++) {
        reader_ids[i] = i + 1;  // Assign reader IDs starting from 1
        pthread_create(&read[i], NULL, reader, (void *)&reader_ids[i]);  // Create reader thread
    }
    
    // Create writer threads
    for (int i = 0; i < num_writers; i++) {
        writer_ids[i] = i + 1;  // Assign writer IDs starting from 1
        pthread_create(&write[i], NULL, writer, (void *)&writer_ids[i]);  // Create writer thread
    }
    
    // Wait for all reader threads to finish
    for (int i = 0; i < num_readers; i++) {
        pthread_join(read[i], NULL);  // Join (wait for) each reader thread to finish
    }
    
    // Wait for all writer threads to finish
    for (int i = 0; i < num_writers; i++) {
        pthread_join(write[i], NULL);  // Join (wait for) each writer thread to finish
    }
    
    // Free dynamically allocated memory
    free(reader_ids);  // Free the memory for reader IDs
    free(writer_ids);  // Free the memory for writer IDs
    
    // Destroy the semaphores to clean up resources
    sem_destroy(&wrt);  // Destroy the write semaphore
    sem_destroy(&reader_count_access);  // Destroy the reader count semaphore
    
    return 0;  // Return 0 to indicate successful execution
}


