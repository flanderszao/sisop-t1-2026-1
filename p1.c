#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int count = 0;
FILE *fptr;

void* increase(){
    count++;
    return NULL;
}

int main(int argc, char *argv[]){
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <num_forks>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    clock_t inicio = clock();
    int buf, one = 1, fd[2];
    pipe(fd);

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            //ADICIONAR LÓGICA
            read(fd[0], &buf, sizeof(buf));
            count = buf;
            while (count < 1000000000){
                write(fd[1], &one, sizeof(one));
            }
            //printf("Child %d created (PID: %d)\n", i + 1, getpid());
            return 0;
        } else {
            while (count < 1000000000){
            read(fd[0], &buf, sizeof(buf));
            count = count + buf;
            }
            // Parent process
        }
    }

    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("Tempo: %.6f s\n", tempo);

    fptr = fopen("fork.txt", "a");

    fprintf(fptr, "\nEm %d Forks: %d | Tempo: %.6f s", n, count, tempo);

    fclose(fptr);

    return 0;
}