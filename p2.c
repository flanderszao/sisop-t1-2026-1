#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int count = 0;
FILE *fptr;

void* increase(){
    count++;
}

int main(int argc, char *argv[]){
    int n = atoi(argv[1]);
    clock_t inicio = clock();
   
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // ADICIONAR LÓGICA COM SEMÁFOROS
            // Child process
            // printf("Child %d created (PID: %d)\n", i + 1, getpid());
            return 0;
        }
    }

    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("Tempo: %.6f s\n", tempo);

    fptr = fopen("thread.txt", "a");

    fprintf(fptr, "\nEm %d Threads: %d | Tempo: %.6f s", n, count, tempo);

    fclose(fptr);

    return 0;
}