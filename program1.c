#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>

void writer(int* global_int, int* read_count, sem_t* mutex, sem_t* rw_mutex) {
    
    // Writer Process waits for all the Reader Processes to exit
    sem_wait(rw_mutex);
    printf("Writer process started\n");
    
    *global_int = 1;
    printf("Writer changes global_int = 1\n");
    
    // Allows other Readers and Writers to enter again.
    sem_post(rw_mutex);
    printf("Writer process ended\n");
}
void reader(int id, int* global_int, int* read_count, sem_t* mutex, sem_t* rw_mutex) {
    
    // Use mutex to make sure only one reader updates read_count at a time.
	sem_wait(mutex);
	*read_count++;
	if (*read_count == 1) {
	    // Keep Writers from entering if there is at least one reader reading
	    sem_wait(rw_mutex);
	}
	printf("Reader %d started reading.\n", id + 1);
	sem_post(mutex);
	
	printf("Reader %d read the Variable global_int = %d\n", id + 1, *global_int);
	
	// Use mutex to make sure only one reader updates read_count at a time.
	sem_wait(mutex);
	*read_count--;
	if (*read_count == 0) {
	    // Allow Writers to enter if no more readers are reading.
	    sem_post(rw_mutex);
	}
	printf("Reader %d finishes reading\n", id + 1);
	sem_post(mutex);
}

int main() {
    
    // num_processes number of reader processes.
    int num_processes = 5;
    pid_t pids[num_processes];
    int i;
    
    // Must create Global Variables in Shared Memory for all Processes to access.
    // Ensures only one reader updates read_count at a time.
    sem_t* mutex = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
                           MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    // Locks if at least one reader is inside to block any writers from entering.
    sem_t* rw_mutex = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
                           MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    // Number of Readers.
    int *read_count = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                           MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    // Variable being written to and read by the processes.
    int *global_int = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                           MAP_SHARED | MAP_ANONYMOUS, -1, 0);
                           
    sem_init(mutex, 1, 1);
    sem_init(rw_mutex, 1, 1);
    *read_count = 0;
    *global_int = 0;
    
    // Creates 2 reader processes first.
    for (i = 0; i < 2; i++) {
        pids[i] = fork();

        if (pids[i] == 0) { // Reader Process
            reader(i, global_int, read_count, mutex, rw_mutex);
            exit(0); // Reader exits, so it doesn't perform any tasks beyond this line.
        }
        // Parent process continues to the next iteration
    }
    
    // Creates One Writer Process
    pid_t writer_pid = fork();
    if (writer_pid == 0) {
        writer(global_int, read_count, mutex, rw_mutex);
        exit(0);
    }
    
    // Create the remaining reader processes here.
    for (i = 2; i < num_processes; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {
            reader(i, global_int, read_count, mutex, rw_mutex);
            exit(0);
        }
    }

    // Parent process waits for all child processes to finish.
    for (i = 0; i < num_processes; i++) {
        waitpid(pids[i], NULL, 0);
        printf("Child process %d joined\n", i + 1);
    }
    waitpid(writer_pid, NULL, 0);
    printf("Writer process joined\n");

    printf("All child processes finished\n");
    
    // Deallocate shared memory.
    munmap(global_int, sizeof(int));
    munmap(read_count, sizeof(int));
    munmap(mutex, sizeof(sem_t));
    munmap(rw_mutex, sizeof(sem_t));
    return 0;
}
