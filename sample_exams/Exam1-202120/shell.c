#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
int order_num = 0;
int main(int argc, char** argv) {
    char command[10];
    while (1) {
        fgets(command, 10, stdin);
        if (command[0] == 'O') {
            order_num++;
            int pid = fork();
            if(pid < 0 ){
                perror("fork failed");
                exit(4);
            }
            if(pid == 0){
                char buff[10];
                sprintf(buff,"%d",order_num);
                execlp("./order.bin","order.bin",buff,NULL);
                exit(0);
            }
        } else if(command[0] == 'D'){
            //add code here for part 5
            int input_num = atoi(command+2);
        }
        else {
            printf("Invalid command. Shell terminated.\n");
            break;
        }
    }
    return 0;
}