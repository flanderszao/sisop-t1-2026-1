#include <stdlib.h>
#include <stdio.h>
int count = 0;

void* increase(){
    while(count<1000000000){
        count++;
    }
}

int main(int argc, char *argv[]){
    pid_t pid = fork();
    

    printf("%d", count);
    return 0;
}