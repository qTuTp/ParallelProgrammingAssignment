#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Q1 "helloWorld"
#define Q2 "slave"
#define Q3 "tag"

int main() {
    int question;
    int processNum;
    char exeFileName[100] = {0};

    printf("Enter Question no: ");
    scanf("%d", &question);

    switch (question)
    {
    case 1:
        strcpy(exeFileName, Q1);
        break;
    case 2:
        strcpy(exeFileName, Q2);
        break;
    case 3:
        strcpy(exeFileName, Q3);
        break;
    
    default:
        strcpy(exeFileName, Q1);
        break;
    }

    printf("Enter number of MPI processes to create: ");
    scanf("%d", &processNum);


    char command[100];
    snprintf(command, sizeof(command), "mpiexec -n %d ./%s.exe", processNum, exeFileName);

    printf("Executing: <%s>\n", command);
    int result = system(command);

    if (result != 0) {
        printf("Failed to launch MPI program.\n");
    }

    return 0;
}
