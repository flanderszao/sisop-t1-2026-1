#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
int count = 0;

void* increase(){
    while(count<1000000000){
        count++;
    }
}

int main(int argc, char *argv[]){
    int n = atoi(argv[1]);

    pthread_t thread[n];
    for (int i=0; i<n; i++){
        pthread_create(&thread[i], NULL, increase, NULL);
    }

    for (int i=0; i<n; i++){
        pthread_join(thread[i], NULL);
    }

    printf("%d", count);
    return 0;
}