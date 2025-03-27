#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Defines a struct, with two integers that will be passed as parameters to the thread's function.
typedef struct {
	int m;
	int n;
} ThreadParams;

void *thread_function(void *arg); // Thread Function Declaration.

int main() {
	pthread_t a_thread; // Thread Declaration
	
	// Define the parameters that will be passed to the function.
	ThreadParams params;
	params.m = 9692;
	params.n = 2813;
	
	void *result; // Result of m+n returned by the thread.
	
	// Create the thread and wait for the thread to return the result with join.
	pthread_create(&a_thread, NULL, thread_function, &params);
	pthread_join(a_thread, &result);
	
	// Print m + n = result.
	printf("Outside Thread\n");
	printf("%d",params.m);
	printf(" + ");
	printf("%d",params.n);
	printf(" = ");
	printf("%d\n",(int)(intptr_t)result); // Cast the result from void* to int for printing.
}
void *thread_function(void *arg) {

	printf("Inside Thread\n");
	ThreadParams* params = (ThreadParams*)arg;
	int m = params->m;
	int n = params->n;
	
	// Casts the result to match the data type of result.
	pthread_exit((void*)(intptr_t)(m+n));
}
