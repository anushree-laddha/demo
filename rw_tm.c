#include <pthread.h>  // Include the pthread library to use threads and synchronization mechanisms.
#include <stdio.h>    // Include standard input/output library for I/O operations.
#include <stdlib.h>   // Include the standard library for memory allocation and process control.

pthread_mutex_t wrt;       // Mutex for controlling access to the shared resource for writers.
pthread_mutex_t mutex;     // Mutex for controlling access to the shared resource for readers.
int cnt = 1;               // Shared variable that will be accessed and modified by both readers and writers.
int numreader = 0;         // Counter to track the number of active readers.


// Writer thread function: Each writer doubles the value of 'cnt'.
void *writer(void *wno) {
    int writer_id = *((int *)wno);   // Get the writer's ID.

    pthread_mutex_lock(&wrt);        // Lock the writer mutex to ensure exclusive access to 'cnt'.
    
    cnt = cnt * 2;                   // The writer modifies the shared variable 'cnt' by doubling it.
    printf("Writer %d modified cnt to %d\n", writer_id, cnt);  // Print the writer's modification.

    pthread_mutex_unlock(&wrt);      // Unlock the writer mutex to allow other writers to access 'cnt'.

    printf("Writer %d is leaving.\n", writer_id);  // Indicate that the writer has finished its operation.
    
    return NULL;  // Return from the writer thread function.
}


// Reader thread function: Each reader reads the value of 'cnt'.
void *reader(void *rno) {
    int reader_id = *((int *)rno);  // Get the reader's ID.
    
    pthread_mutex_lock(&mutex);     // Lock the mutex to safely modify the 'numreader' counter.
    
    numreader++;                    // Increment the number of active readers.
    
    if (numreader == 1) {           // If this is the first reader, lock the writer mutex.
        pthread_mutex_lock(&wrt);   // Ensure that no writers can modify 'cnt' while readers are active.
    }
    
    pthread_mutex_unlock(&mutex);   // Unlock the mutex after updating the reader count.

    // Print the reader's view of 'cnt'.
    printf("Reader %d: read cnt as %d\n", reader_id, cnt);
    
    pthread_mutex_lock(&mutex);     // Lock the mutex again to modify the 'numreader' counter.
    
    numreader--;                    // Decrement the number of active readers.
    
    if (numreader == 0) {           // If this is the last reader, unlock the writer mutex.
        pthread_mutex_unlock(&wrt); // Allow writers to access 'cnt' again.
    }
    
    pthread_mutex_unlock(&mutex);   // Unlock the mutex after updating the reader count.

    printf("Reader %d is leaving.\n", reader_id);  // Indicate that the reader has finished its operation.
    
    return NULL;  // Return from the reader thread function.
}


int main() {
    int num_readers, num_writers;
    
    // Ask the user for the number of readers and writers.
    printf("Enter the number of readers: ");
    scanf("%d", &num_readers);    // Input the number of readers.
    
    printf("Enter the number of writers: ");
    scanf("%d", &num_writers);    // Input the number of writers.
    
    pthread_t read[num_readers], write[num_writers];   // Declare arrays to hold reader and writer thread IDs.
    
    pthread_mutex_init(&mutex, NULL);  // Initialize the reader mutex.
    pthread_mutex_init(&wrt, NULL);    // Initialize the writer mutex.
    
    // Dynamically allocate memory for reader and writer IDs.
    int *reader_ids = (int *)malloc(num_readers * sizeof(int)); 
    int *writer_ids = (int *)malloc(num_writers * sizeof(int)); 
    
    // Create the reader threads and assign them unique IDs.
    for (int i = 0; i < num_readers; i++) {
        reader_ids[i] = i + 1;  // Assign unique ID to the reader.
        pthread_create(&read[i], NULL, reader, (void *)&reader_ids[i]);  // Create reader thread.
    }
    
    // Create the writer threads and assign them unique IDs.
    for (int i = 0; i < num_writers; i++) {
        writer_ids[i] = i + 1;  // Assign unique ID to the writer.
        pthread_create(&write[i], NULL, writer, (void *)&writer_ids[i]);  // Create writer thread.
    }
    
    // Wait for all reader threads to finish.
    for (int i = 0; i < num_readers; i++) {
        pthread_join(read[i], NULL);  // Join each reader thread.
    }
    
    // Wait for all writer threads to finish.
    for (int i = 0; i < num_writers; i++) {
        pthread_join(write[i], NULL);  // Join each writer thread.
    }
    
    // Free dynamically allocated memory for reader and writer IDs.
    free(reader_ids);
    free(writer_ids);
    
    // Destroy the mutexes after use.
    pthread_mutex_destroy(&mutex);  // Destroy the reader mutex.
    pthread_mutex_destroy(&wrt);    // Destroy the writer mutex.
    
    return 0;  // Exit the program successfully.
}
