//process related
// Include necessary headers for process management, system calls, and input/output
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Function to demonstrate process-related system calls
void process_related_calls() {
    printf("Process Related System Calls:\n");
    printf("1. Fork a process\n");
    printf("2. Exit a process\n");
    printf("3. Wait for a process\n");
    printf("Enter your choice: ");
    int process_choice;
    scanf("%d", &process_choice);  // Input the user's choice for process operations

    switch (process_choice) {
        case 1: {
            pid_t pid = fork();  // Fork a new process
            if (pid == 0) {  // Child process
                printf("This is the child process\n");
                exit(0);  // Exit the child process
            } else if (pid > 0) {  // Parent process
                printf("This is the parent process\n");
            } else {
                perror("Fork failed");  // If fork fails
            }
            break;
        }
        case 2:
            printf("Exiting the process...\n");
            exit(0);  // Exit the program
        case 3: {
            pid_t pid = fork();  // Fork a new process
            if (pid == 0) {  // Child process
                sleep(2);  // Simulate some work
                printf("Child process finished\n");
                exit(0);  // Exit the child process
            } else if (pid > 0) {  // Parent process
                printf("Waiting for the child process to complete...\n");
                wait(NULL);  // Wait for the child process to complete
                printf("Parent process resumed\n");
            } else {
                perror("Fork failed");  // If fork fails
            }
            break;
        }
        default:
            printf("Invalid choice\n");  // Handle invalid input
    }
}

int main() {
    process_related_calls();  // Call the function to demonstrate process-related system calls
    return 0;
}

//file related
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

// Function to demonstrate file-related system calls
void file_related_calls() {
    printf("File Related System Calls:\n");
    printf("1. Open and Read a file\n");
    printf("2. Write and Close a file\n");
    printf("3. Link and Unlink a file\n");   
    printf("4. Display file status\n");
    printf("Enter your choice: ");
    int file_choice;
    scanf("%d", &file_choice);  // Input user's choice for file operations
    
    switch (file_choice) {
        case 1: {
            char filename[100];
            printf("Enter file name to open and read: ");
            scanf("%s", filename);  // Input file name to read
            int fd = open(filename, O_RDONLY);  // Open the file for reading
            if (fd == -1) {
                perror("Error opening file");  // Handle error if file can't be opened
                break;
            }
            char buffer[1024];
            int bytes_read = read(fd, buffer, sizeof(buffer) - 1);  // Read the file content
            if (bytes_read == -1) {
                perror("Error reading file");  // Handle error if reading fails
                close(fd);
                break;
            }
            buffer[bytes_read] = '\0';  // Null-terminate the buffer
            printf("File content:\n%s\n", buffer);  // Print file content
            close(fd);  // Close the file after reading
            break;
        }
        case 2: {
            char filename[100];
            printf("Enter file name to write to: ");
            scanf("%s", filename);  // Input file name to write to
            int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);  // Open file for writing
            if (fd == -1) {
                perror("Error opening file");  // Handle error if file can't be opened
                break;
            }
            char text[1024];
            printf("Enter the text to write: ");
            getchar();  // Consume the newline left by previous scanf
            fgets(text, sizeof(text), stdin);  // Input the text to write to the file
            int bytes_written = write(fd, text, strlen(text));  // Write text to the file
            if (bytes_written == -1) {
                perror("Error writing to file");  // Handle error if writing fails
                close(fd);
                break;
            }
            close(fd);  // Close the file after writing
            printf("Text written to file\n");
            break;
        } 
        case 3: {
            char srcfile[100], destlink[100];
            printf("Enter the source file name to link: ");
            scanf("%s", srcfile);  // Input the source file name
            printf("Enter the destination link name: ");
            scanf("%s", destlink);  // Input the destination link name
            if (link(srcfile, destlink) == -1) {
                perror("Error linking file");  // Handle error if linking fails
                break;
            }
            printf("Link created\n");
            if (unlink(destlink) == -1) {
                perror("Error unlinking file");  // Handle error if unlinking fails
                break;
            }
            printf("Link removed\n");
            break;
        }
        case 4: {
            char filename[100];
            struct stat file_stat;
            printf("Enter the file name to get status: ");
            scanf("%s", filename);  // Input the file name to get its status
            if (stat(filename, &file_stat) == -1) {
                perror("Error getting file status");  // Handle error if stat fails
                break;
            }
            // Print the file status
            printf("File Size: %lld bytes\n", (long long)file_stat.st_size);
            printf("Number of Links: %hu\n", (unsigned short)file_stat.st_nlink);
            printf("File inode: %llu\n", (unsigned long long)file_stat.st_ino);
            printf("File Permissions: ");
            // Display file permissions
            printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
            printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
            printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
            printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
            printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
            printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
            printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
            printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
            printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
            printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");
            printf("\n");
            break;
        }
        default:
            printf("Invalid choice\n");  // Handle invalid choice input
    }
}

int main() {
    file_related_calls();  // Call the function to demonstrate file-related system calls
    return 0;
}

//communication sys call
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

// Function to demonstrate communication-related system calls
void communication_related_calls() {
    printf("Communication Related System Calls:\n");
    printf("1. Pipe\n");
    printf("2. FIFO\n");
    printf("Enter your choice: ");
    int comm_choice;
    scanf("%d", &comm_choice);  // Input user's choice for communication operations

    switch (comm_choice) {
        case 1: {
            int fd[2];
            pipe(fd);  // Create a pipe
            if (fork() == 0) {  // Child process
                close(fd[0]);  // Close the read end of the pipe in the child
                write(fd[1], "Hello from child", 16);  // Write message to pipe
                close(fd[1]);  // Close the write end
                exit(0);  // Exit child process
            } else {  // Parent process
                close(fd[1]);  // Close the write end of the pipe in the parent
                char buffer[1024];
                read(fd[0], buffer, 1024);  // Read message from the pipe
                printf("Parent received: %s\n", buffer);  // Print received message
                close(fd[0]);  // Close the read end
                wait(NULL);  // Wait for the child process to finish
            }
            break;
        }
        case 2: {
            char* fifo = "/tmp/my_fifo";  // FIFO file path
            mkfifo(fifo, 0666);  // Create a FIFO special file
            if (fork() == 0) {  // Child process
                int fd = open(fifo, O_WRONLY);  // Open FIFO for writing
                write(fd, "Hello FIFO", 10);  // Write message to FIFO
                close(fd);  // Close the FIFO
                exit(0);  // Exit child process
            } else {  // Parent process
                int fd = open(fifo, O_RDONLY);  // Open FIFO for reading
                char buffer[1024];
                read(fd, buffer, 1024);  // Read message from FIFO
                printf("Parent received: %s\n", buffer);  // Print received message
                close(fd);  // Close the FIFO
                wait(NULL);  // Wait for the child process to finish
                unlink(fifo);  // Delete the FIFO
            }
            break;
        }
        default:
            printf("Invalid choice\n");  // Handle invalid input
    }
}

int main() {
    communication_related_calls();  // Call the function to demonstrate communication-related system calls
    return 0;
}

//info related
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

// Function to demonstrate information-related system calls
void information_related_calls() {
    printf("Information Related System Calls:\n");
    printf("1. Get Process ID\n");
    printf("2. Get Parent Process ID\n");
    printf("3. Get User ID\n");
    printf("Enter your choice: ");

    int info_choice;
    int result = scanf("%d", &info_choice);  // Capture scanf result

    if (result != 1) {  // Handle invalid input
        printf("Failed to read input. Please enter a valid number.\n");
        return;
    }

    switch (info_choice) {
        case 1:
            printf("Process ID: %d\n", getpid());  // Print the process ID
            break;
        case 2:
            printf("Parent Process ID: %d\n", getppid());  // Print the parent process ID
            break;
        case 3:
            printf("User ID: %d\n", getuid());  // Print the user ID
            break;
        default:
            printf("Invalid choice\n");  // Handle invalid input
    }
}

int main() {
    information_related_calls();  // Call the function to demonstrate information-related system calls
    return 0;
}
