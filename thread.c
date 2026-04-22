#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

int count = 0;
FILE *fptr;

void* increase(){
    while(count<1000000000){
        count++;
    }

    return NULL;
}

int main(int argc, char *argv[]){
    int n = atoi(argv[1]);
    clock_t inicio = clock();

    pthread_t thread[n];
    for (int i=0; i<n; i++){
        pthread_create(&thread[i], NULL, increase, NULL);
    }

    for (int i=0; i<n; i++){
        pthread_join(thread[i], NULL);
    }

    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("%d\n", count);
    printf("Tempo: %.6f s\n", tempo);

    fptr = fopen("thread.txt", "a");

    fprintf(fptr, "\nEm %d Threads: %d | Tempo: %.6f s", n, count, tempo);

    fclose(fptr);

    return 0;
}