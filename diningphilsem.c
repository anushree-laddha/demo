#include <pthread.h>      // Include the pthread library for thread management.
#include <semaphore.h>     // Include the semaphore library for synchronization mechanisms.
#include <stdio.h>         // Include the standard I/O library for printing messages.
#include <unistd.h>        // Include the unistd library for sleep functionality.

#define N 5                // Define the number of philosophers (and the number of forks) to be 5.

sem_t forks[N];           // Declare an array of semaphores, one for each fork. Initially, all forks are available.


// Function to simulate the behavior of a philosopher
void *philosopher(void *arg) {
    int id = *((int *)arg);  // Extract philosopher's id from the argument passed to the thread.

    // Loop to simulate the philosopher's activity three times
    for (int i = 0; i < 3; i++) {
        printf("Philosopher %d is thinking.\n", id);  // Print that the philosopher is thinking.
        usleep(100000);  // Simulate thinking for 100 milliseconds.

        // Wait to pick up the left fork (fork[id])
        sem_wait(&forks[id]);
        // Wait to pick up the right fork (fork[(id + 1) % N]) - the modulo ensures circular fork usage.
        sem_wait(&forks[(id + 1) % N]);

        printf("Philosopher %d is eating.\n", id);  // Print that the philosopher is eating.
        usleep(100000);  // Simulate eating for 100 milliseconds.

        // Release the left fork (fork[id]) after eating
        sem_post(&forks[id]);
        // Release the right fork (fork[(id + 1) % N]) after eating
        sem_post(&forks[(id + 1) % N]);

        printf("Philosopher %d finished eating.\n", id);  // Print that the philosopher has finished eating.
        usleep(100000);  // Simulate the philosopher's post-eating rest for 100 milliseconds.
    }
    return NULL;  // Exit the philosopher thread.
}

int main() {
    pthread_t philosophers[N];  // Declare an array of pthreads for the philosophers.
    int ids[N];                 // Declare an array to store each philosopher's id.

    // Initialize the semaphores for each fork (set the initial value to 1, meaning each fork is available).
    for (int i = 0; i < N; i++)
        sem_init(&forks[i], 0, 1);

    // Create a thread for each philosopher.
    for (int i = 0; i < N; i++) {
        ids[i] = i;  // Assign an ID to the philosopher.
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);  // Create a new thread for philosopher i.
    }

    // Wait for all philosopher threads to finish.
    for (int i = 0; i < N; i++)
        pthread_join(philosophers[i], NULL);

    // Destroy the semaphores for each fork after use.
    for (int i = 0; i < N; i++)
        sem_destroy(&forks[i]);

    return 0;  // Exit the program.
}
