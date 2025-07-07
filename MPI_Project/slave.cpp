/*
Modify the hello.c program so that the slave processes do not issue the print
statements, but each sends a message back to the master containing their
rank after the master receives each message, it print out a message
containing the slave's rank. The program should produce output such as:
Master: Hello slaves give me your messages
Message received from process 1 : Hello back
Message received from process 2 : Hello back
Message received from process 3 : Hello back
Now, all print statements are issued by the master.
Clue: MPI_Comm_rank returns the processes rank using the argument rank..
b)
Alter the code so that each process send a different message back, e.g.:
Master: Hello slaves give me your messages
Message received from process 1 : Hello, I am John
Message received from process 2 : Hello, I am Mary
Message received from process 3 : Hello, I am Susan
*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAIN_PROCESS 0
#define TAG 99

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
        printf("This is main process, child process can start to send me message....\n");

        char msg[256] = {0};
        
        for (int childRank = 1; childRank < *processSize; childRank++){
            MPI_Recv(msg, 256, MPI_CHAR, childRank, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Msg from Process %d: %s\n", childRank, msg);
            memset(msg, 0, sizeof(msg));
        }
    }else{ //Other child process
        char msg[256];
        #if UNIQUE_MESSAGE
            char name[20];
            strcpy(name, PREDEF_NAME[(*processRank - 1) % 10]);
            snprintf(msg, 256 - 1, "This is %s, from process %d", name, *processRank);
        #else
            strcpy(msg, "This is child process...");
        #endif
        MPI_Send(msg, strlen(msg) + 1, MPI_CHAR, MAIN_PROCESS, TAG, MPI_COMM_WORLD);
    }

    free(processRank);
    free(processSize);
    MPI_Finalize();  // End of parallel process

    return 0;
}