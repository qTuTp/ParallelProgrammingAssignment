
/*
Using a for work-sharing construct add the values of two vectors together into
a third vector. Write the program with static schedule and then rewrite it with
dynamic schedule. What are your conclusions?
*/

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

#define N 1000000
#define MAX_RAND 100

void vectorInit(int[]);

int main() {
    srand(time(NULL));
    // int v1[N], v2[N], result[N] = {0};
    int *v1 = malloc(N * sizeof(int));
    int *v2 = malloc(N * sizeof(int));
    int *result = malloc(N * sizeof(int));


    int threadCount[16] = {0};
    double startTime = 0.0, endTime = 0.0;

    vectorInit(v1);
    vectorInit(v2);

    startTime = omp_get_wtime();
    #pragma omp parallel for num_threads(16) schedule(dynamic, 100) shared(threadCount)
    for (int i = 0; i < N; i++) {
        result[i] = v1[i] + v2[i];
        // printf("Thread %d ||| %d + %d = %d\n", omp_get_thread_num(), v1[i], v2[i], result[i]);
        threadCount[omp_get_thread_num()]++;
    }
    endTime = omp_get_wtime();

    printf("\n\n---Final Result---\n");
    for (int i = 0; i < 16; i ++){
        printf("Thread %d Work Load = %d\n", i, threadCount[i]);
    }
    printf("Time Taken: %lf", endTime-startTime);

    free(v1);
    free(v2);
    free(result);
    return 0;

}

void vectorInit(int vector[]){
    for (int i = 0; i < N; i ++){
        vector[i] = rand() % MAX_RAND;
    }
}