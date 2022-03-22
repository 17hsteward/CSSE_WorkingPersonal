/* Copyright 2016 Rose-Hulman
   But based on idea from http://cnds.eecs.jacobs-university.de/courses/caoslab-2007/
   */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void waits(int signum){
    int status;
    wait(&status);
    printf("Background command finished\n");
}

int main() {
    char command[82];
    char *parsed_command[2];
    //takes at most two input arguments
    // infinite loop but ^C quits
    
    while (1) {
        printf("SHELL%% ");
        fgets(command, 82, stdin);
        command[strlen(command) - 1] = '\0';//remove the \n
        int len_1;
        for(len_1 = 0;command[len_1] != '\0';len_1++){
            if(command[len_1] == ' ')
                break;
        }
        
        parsed_command[0] = command;
        if(len_1 == strlen(command)){
            printf("Command is '%s' with no arguments\n", parsed_command[0]); 
            parsed_command[1] = NULL;
        }else{
            command[len_1] = '\0';
            parsed_command[1] = command + len_1 + 1;
            printf("Command is '%s' with argument '%s'\n", parsed_command[0], parsed_command[1]); 
        }
        int BG = 0;
        if(command[0]=='B'&& command[1] == 'G'){
            BG = 1;
        }
        if(BG==1){
          char* commandBG = command + 2;
          pid_t pid_1 = fork();
          if(pid_1==0){
             //pid_t pid_3 = fork();
            //  if(pid_3==0){
                 execlp(commandBG,commandBG,parsed_command[1],NULL);
                 exit(4);
            //  }
            //  else{
            //  int status;
            //  wait(&status);
            //  printf("Background command finished\n");
            //  exit(5);
            //   }
          }
          signal(SIGCHLD,waits);
        }
        else{
        pid_t pid_2 = fork();
        if(pid_2 == 0){
           execlp(command,parsed_command[0],parsed_command[1],NULL);
           exit(3);
        }
        else{
            int status;
            wait(&status); 
         }
       }
     }
        
    }

