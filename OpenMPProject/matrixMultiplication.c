/*
Write an OpenMP program that performs matrix calculation. Run your code
and collect data based on the following criteria
a) Use 1, 4, 8 and 16 threads on a 50x50 matrix
b) Use 1, 4, 8 and 16 threads on a 500x500 matrix
c) Parallelize only the outer loop for both conditions defined above
d) Parallelize only the inner loop for both conditions defined above
Collect timing data for each case; average the result based on ten test runs
each. Make tables and graphs of your average timing data and put them in the
submitted report. After you have reported your results, try to explain them as
best as possible.
*/


#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

#define NORMAL_PARALLELISM 0
#define OUTERLOOP_PARALLELISM 1
#define INNERLOOP_PARALLELISM 2

typedef enum {
    Randomize,
    ZeroInit
}INIT_MATRIX;

/*
START
CONFIG
*/
#define METHOD INNERLOOP_PARALLELISM

#define ROW 50
#define COLUMN 50
#define MAX_RAND 100
#define NUM_THREADS 16
#define TEST_SIZE 10

/*
END 
CONFIG
*/


void multiplyMatrices(int **, int **, int **);
void initMatrix(int **, INIT_MATRIX);
void printMatrix(int **);
int** allocateMatrix();
void freeMatrix(int **);

int main(){
    omp_set_num_threads(NUM_THREADS);
    srand(time(NULL));

    double averageTime = 0.0;

    for (size_t i = 0; i < TEST_SIZE; i++)
    {
        int **matrix1 = allocateMatrix(), **matrix2 = allocateMatrix(), **result = allocateMatrix();
        double startTime = 0.0, endTime = 0.0;

        initMatrix(matrix1, Randomize);
        initMatrix(matrix2, Randomize);
        initMatrix(result, ZeroInit);
        
        startTime = omp_get_wtime();
        multiplyMatrices(matrix1, matrix2, result);
        #pragma omp barrier
        endTime = omp_get_wtime();
        averageTime += (endTime - startTime);
        printf("\nStart Time: %lf\nEnd Time: %lf\nElapsed Time: %lf\n\n", startTime, endTime, endTime-startTime);
        freeMatrix(matrix1);
        freeMatrix(matrix2);
        freeMatrix(result);
    }

    averageTime /= TEST_SIZE;

    printf("\n\nMatrix Size: Row %d Column %d\nNumber of Threads: %d\nParallel Method: %d\nAverage Time: %lf\n", ROW, COLUMN, NUM_THREADS, METHOD, averageTime);
    

    
    // int **matrix1 = allocateMatrix(), **matrix2 = allocateMatrix(), **result = allocateMatrix();
    // double startTime = 0.0, endTime = 0.0;

    // initMatrix(matrix1, Randomize);
    // initMatrix(matrix2, Randomize);
    // initMatrix(result, ZeroInit);
    
    // startTime = omp_get_wtime();
    // multiplyMatrices(matrix1, matrix2, result);
    // endTime = omp_get_wtime();

    // printf("MATRIX 1: \n");
    // printMatrix(matrix1);
    // printf("\n\nMATRIX 2: \n");
    // printMatrix(matrix2);
    // printf("\n\nRESULT MATRIX: \n");
    // printMatrix(result);

    

}

void multiplyMatrices(int **m1, int **m2, int **result){
    #if (METHOD == NORMAL_PARALLELISM)
        printf("\n---NORMAL---\n");
        #pragma omp parallel for collapse(3)
        for (int i = 0; i < ROW; i++)
            for (int j = 0; j < COLUMN; j++)
                for (int k = 0; k < COLUMN; k++)
                    result[i][j] += m1[i][k] * m2[k][j];
        return;
    #endif

    #if (METHOD == OUTERLOOP_PARALLELISM)
        printf("\n---OUTERLOOP Parallelism---\n");
        #pragma omp parallel for
        for(int i = 0; i < ROW; i++){
            for (int j = 0; j < COLUMN; j++){
                for (int k = 0; k < COLUMN; k++)
                result[i][j] += m1[i][k] * m2[k][j];
            }
            
        }
    #endif

    #if (METHOD == INNERLOOP_PARALLELISM)
        printf("\n---INNERLOOP Parallelism---\n");
        for (int i = 0; i < ROW; i++)
        #pragma omp parallel for
            for (int j = 0; j < COLUMN; j++)
                for (int k = 0; k < COLUMN; k++)
                    result[i][j] += m1[i][k] * m2[k][j];

    #endif

}

void freeMatrix(int **matrix) {
    for (int i = 0; i < ROW; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Allocate 2D matrix
int **allocateMatrix() {
    int **matrix = (int **)malloc(ROW * sizeof(int *));
    for (int i = 0; i < ROW; i++) {
        matrix[i] = (int *)calloc(COLUMN, sizeof(int));
    }
    return matrix;
}

void initMatrix(int **matrix, INIT_MATRIX method){
    switch (method)
    {
    case Randomize:
        for (int i = 0; i < ROW; i++)
            for (int j = 0; j < COLUMN; j++)
                matrix[i][j] = rand() % MAX_RAND + 1;
        break;

    case ZeroInit:
        for (int i = 0; i < ROW; i++)
            for (int j = 0; j < COLUMN; j++)
                matrix[i][j] = 0;
        break;

    default:
        break;
    }
}

void printMatrix(int **matrix) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COLUMN; j++) {
            printf("%d ", matrix[i][j]);  // Adjust spacing/precision as needed
        }
        printf("\n\n");
    }
}
