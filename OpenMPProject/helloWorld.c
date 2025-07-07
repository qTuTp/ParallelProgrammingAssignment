// Write a helloworld program using openmp. This program must identify that
// different threads are printing helloworld. There should be 10 threads.
// Then alter the number of threads to 5 by altering the environment variable
// with a command.
// Then enable a user input to suggest the number of threads to be created.

#include <stdio.h>
#include <omp.h>

#define CLAUSE_SET_THREADS 0
#define ENVIRONMENT_VARIABLE_THREADS 1
#define USER_INPUT_SET_THREADS 2

#define METHOD USER_INPUT_SET_THREADS

int main(){
    #if METHOD == CLAUSE_SET_THREADS
        #pragma omp parallel num_threads(10)
        {
            int ID = omp_get_thread_num();

            printf("Hello World: ID %d\n", ID);
            
        }
    #endif
    #if METHOD == ENVIRONMENT_VARIABLE_THREADS
    /*
        Command to set environment variable in powershell:
        <$env:OMP_NUM_THREADS = "5">
    */
        #pragma omp parallel
        {
            int ID = omp_get_thread_num();

            printf("Hello World: ID %d\n", ID);
            
        }
    #endif

    #if METHOD == USER_INPUT_SET_THREADS
        int numThreads = 1;
        printf("Please enter the number of threads to run: ");
        scanf("%d", &numThreads);

        #pragma omp parallel num_threads(numThreads)
        {
            int ID = omp_get_thread_num();
            printf("Hello World: ID %d\n", ID);
        }
    #endif

    

    return 0;

   
}