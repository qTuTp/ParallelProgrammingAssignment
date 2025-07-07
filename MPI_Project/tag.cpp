/*
This MUST be a separate program, not combined with Part B. If you
combine it with part B you will lose marks.

In the previous programs, let’s say tag of 99 is used in messages. Modify the
program from task b) above so that the master process sends messages to
each slave, but with the tag of 100 and the slave waits for message with a tag
of 100. Confirm program works.
Repeat but make the slaves wait for tag 101, and check program hangs.
Why?
*/


#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAIN_PROCESS 0
#define MASTER_TAG 100
#define SLAVE_TAG 101

#define UNIQUE_MESSAGE 1

const char* PREDEF_NAME[10] = { "John",
                                "David",
                                "Mary",
                                "Arthur",
                                "Tilly",
                                "Roland",
                                "Angela",
                                "Ishmael",
                                "Abby",
                                "Jack"};

int main(){
    //Start of parallel process 
    MPI_Init(NULL, NULL); 

    //Each process will initialize the following 2 variable and get their our rank, and the total of process created
    int *processRank = (int *)malloc(sizeof(int));
    int *processSize = (int *)malloc(sizeof(int));
    MPI_Comm_rank(MPI_COMM_WORLD, processRank);  
    MPI_Comm_size(MPI_COMM_WORLD, processSize);

    if (*processRank == MAIN_PROCESS){ //Only main process
        printf("This is main process, will start sending message....\n");
        fflush(stdout);

        char msg[256] = {0};
        
        for (int childRank = 1; childRank < *processSize; childRank++){
            #if UNIQUE_MESSAGE
                char name[20];
                strcpy(name, PREDEF_NAME[(childRank - 1) % 10]);
                snprintf(msg, 256 - 1, "Hey Process %d, Your name is %s", childRank, name);
            #else
                strcpy(msg, "This is child process...");
            #endif
            MPI_Send(msg, strlen(msg) + 1, MPI_CHAR, childRank, MASTER_TAG, MPI_COMM_WORLD);
            
            memset(msg, 0, sizeof(msg));
        }
    }else{ //Other child process
        char msg[256] = {0};
        MPI_Recv(msg, 256, MPI_CHAR, MAIN_PROCESS, SLAVE_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("This is Process %d, Receive message from Main Process: %s\n", *processRank, msg);
        fflush(stdout);
    }

    free(processRank);
    free(processSize);
    MPI_Finalize();  // End of parallel process

    return 0;
}