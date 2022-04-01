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
        if(order_num > 3){
            printf("Accept No More Orders. Closing system now.\n");
            exit(0);
        }
        int pid = fork();
        if(pid<0)
        {
            perror("Fork Failed\n");
            exit(4);
        }
        else if(pid == 0){
          int pid_1 = fork();
          if(pid_1<0)
        {
            perror("Fork Failed\n");
            exit(4);
        }
        else if(pid_1 == 0){
            char buff[10];
            sprintf(buff, "%d", order_num);
            execlp("./order.bin","order.bin",buff,NULL);
            printf("Execlp failed\n");
            exit(2);
        }
         wait(NULL);
         pid_1 = fork();
         if(pid_1==0){
         char buff[10];
            sprintf(buff, "%d", order_num);
            execlp("./cook.bin","cook.bin",buff,NULL);
            printf("Execlp failed\n");
            exit(2);
        }
        }
        alarm(4);
        int status;
        wait(&status);
        if(WEXITSTATUS(status) > 0){
                    printf("Order %d is aborted.\n", order_num);
                    return 0;
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