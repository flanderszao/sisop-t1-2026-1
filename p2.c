#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>

FILE *fptr;

static const char *SEM_NAME = "/sem_p2";

int main(int argc, char *argv[]){
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <num_forks>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);

    struct timeval inicio, fim;
    gettimeofday(&inicio, NULL);

    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }

    int *count = (int *) shmat(shmid, NULL, 0);
    if (count == (void *) -1) {
        perror("shmat");
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }

    *count = 0;

    sem_unlink(SEM_NAME);

    sem_t *sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0644, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        shmdt(count);
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            sem_close(sem);
            sem_unlink(SEM_NAME);
            shmdt(count);
            shmctl(shmid, IPC_RMID, NULL);
            return 1;
        }

        if (pid == 0) {
            //ADICIONAR LÓGICA
            while (1) {
                sem_wait(sem);

                if (*count >= 1000000000) {
                    sem_post(sem);
                    break;
                }

                (*count)++;   /* P1: sincronizado com semáforo */

                sem_post(sem);
            }

            shmdt(count);     /* filho desanexa */
            sem_close(sem);
            _exit(0);
        } 
    }

    for (int i = 0; i < n; i++) {
        int status;
        wait(&status);
    }

    int final_count = *count;

    if (shmdt(count) == -1) {
        perror("shmdt");
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl IPC_RMID");
    }

    sem_close(sem);
    sem_unlink(SEM_NAME);

    gettimeofday(&fim, NULL);
    double tempo = (fim.tv_sec - inicio.tv_sec)
                 + (fim.tv_usec - inicio.tv_usec) / 1e6;

    printf("\nEm %d Forks: %d | Tempo: %.6f s\n", n, final_count, tempo);

    fptr = fopen("forksem.txt", "a");
    if (fptr == NULL) {
        perror("fopen");
        return 1;
    }

    fprintf(fptr, "\nEm %d Forks: %d | Tempo: %.6f s", n, final_count, tempo);

    fclose(fptr);

    return 0;
}