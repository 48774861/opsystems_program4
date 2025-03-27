#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>

sem_t mutex; // Ensures only one reader updates read_count at a time.
sem_t rw_mutex; // Locks if at least one reader is inside to block any writers from entering.
int read_count = 0; // Counts the number of readers
int global_int = 0;

void writer() {
    
    printf("Writer process started\n");
    sem_wait(&rw_mutex);
    
    global_int = 1;
    printf("Writer changes global_int = 1\n");
    
    sem_post(&rw_mutex);
    printf("Writer process ended\n");
}
void reader(int id) {
    
    printf("Reader %d started reading.\n", id + 1);
    
	sem_wait(&mutex);
	read_count++;
	if (read_count == 1) {
	    sem_wait(&rw_mutex);
	}
	sem_post(&mutex);
	
	printf("Reader %d read the Variable global_int = %d\n", id + 1, global_int);
	
	sem_wait(&mutex);
	read_count--;
	if (read_count == 0) {
	    sem_wait(&rw_mutex);
	}
	sem_post(&mutex);
	
	printf("Reader %d finishes reading\n", id + 1);
}

int main() {
    sem_init(&mutex, 1, 1);
    sem_init(&rw_mutex, 1, 1);
    
    int num_processes = 5;
    pid_t pids[num_processes];
    int i;

    for (i = 0; i < num_processes; i++) {
        pids[i] = fork();

        if (pids[i] == 0) { // Child Process starts and ends here.
            reader(i);
            // exit(0); // Exit child process
        }
        // Parent process continues to the next iteration
    }
    
    pid_t writer_pid = fork();
    if (writer_pid == 0) { // One Writer Process
        writer();
    }

    // Parent process waits for all child processes to finish
    for (i = 0; i < num_processes; i++) {
        waitpid(pids[i], NULL, 0);
        printf("Child process %d joined\n", i + 1);
    }
    waitpid(writer_pid, NULL, 0);
    printf("Writer process joined\n");

    printf("All child processes finished\n");
    return 0;
}
