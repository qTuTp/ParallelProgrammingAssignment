/*
Write a helloworld program using mpi. This program must run with 4
processes.
Comment on the how MPI processes map to processors/cores.
Write an enhanced version of the same said program above to enable the
user to input the number of cores/processors
*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MAIN_PROCESS 0

#define ENHANCED 1

int main() {
    #if !ENHANCED
        //Start of parallel process 
        MPI_Init(NULL, NULL); 

        //Each process will initialize the following 2 variable and get their our rank, and the total of process created
        int *processRank = (int *)malloc(sizeof(int));
        int *processTotal = (int *)malloc(sizeof(int));
        MPI_Comm_rank(MPI_COMM_WORLD, processRank);  
        MPI_Comm_size(MPI_COMM_WORLD, processTotal);  

        printf("This is process %d / %d, Hello World\n", *processRank + 1, *processTotal);
        free(processRank);
        free(processTotal);
        MPI_Finalize();  // End of parallel process
    #else
        MPI_Init(NULL, NULL);
        //Each process will initialize the following 2 variable and get their our rank, and the total of process created
        int processRank, processTotal;
        MPI_Comm_rank(MPI_COMM_WORLD, &processRank);  
        MPI_Comm_size(MPI_COMM_WORLD, &processTotal);  
        int processNum = 0;
        if (processRank == MAIN_PROCESS){
            printf("Please enter how many process you wish to use (Available: %d): ", processTotal); //Exclude Main Process
            fflush(stdout);
            scanf("%d", &processNum);
            if (processNum > processTotal){
                printf("Max Process is %d, set num of process used to %d\n", processTotal, processTotal);
                fflush(stdout);
                processNum = processTotal;
            }
        }

        MPI_Bcast(&processNum, 1, MPI_INT, MAIN_PROCESS, MPI_COMM_WORLD);

        if (processRank < processNum){
            printf("This is process %d, Hello World\n", processRank);
        }

        MPI_Finalize();
    #endif

    return 0;


}
