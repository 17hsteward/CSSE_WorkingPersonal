#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/mman.h>

// static double *glob_var;  // for use on the last two parts
static double *glob_var;

int main(void)
{
  int fd[2];
  char *rcvrs[] = {"./fast_receiver", "./slow_receiver",
		   "./unreliable_receiver"};
  char *sndrs[] = {"./fast_sender", "./slow_sender", "./unreliable_sender"};
  int nrcvrs = 3;
  int nsndrs = 3;
  glob_var = mmap(NULL, nrcvrs*nsndrs*sizeof(double), PROT_READ | PROT_WRITE, 
		  MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  for (int i=0; i<nrcvrs; i++) {
    for (int j=0; j<nsndrs; j++) {
      int pid = fork();
      if(pid < 0){
        printf("fork failed\n");
        return 1;
      }
        else if(pid == 0){

      struct timeval start, end;
      double dt;
      int status1, status2;
      int pid_2, pid_3;
      printf("testing %s\n", sndrs[j]);
      gettimeofday(&start, NULL);
      pid_2 =fork();
      if(pid_2 <0){
        printf("fork failed\n");
        return 1;
      }
      else if(pid_2==0){
        execl(sndrs[j], sndrs[j], "-1", NULL);
        

      
      //printf("I would like to test %s with %s\n", rcvrs[i], sndrs[j]);
      //sleep(1);
      
     
      printf("Exec has failed\n");
      return 1;
      }
      pid_3 = fork();
      if (pid_3 < 0) {
	     printf("fork failed\n");
	     return 1;
	     }
       else if(pid_3 ==0){
          execl(rcvrs[i], rcvrs[i], "-1", NULL);

	        printf("exec failed\n");
	        return 1;
       }
       printf("waiting for %s to finish\n", sndrs[j]);
      	waitpid(pid_2, &status1, 0);
       	waitpid(pid_3, &status2, 0);
	     gettimeofday(&end, NULL);
	     dt = end.tv_sec-start.tv_sec+(end.tv_usec-start.tv_usec)/1000000.0;
      	if (WEXITSTATUS(status1) || WEXITSTATUS(status2)) {
	  glob_var[i*nsndrs+j] = -1;
	} else {
	  glob_var[i*nsndrs+j] = dt;
	}
        printf("Done testing %s with %s, %f sec required\n", sndrs[j],
	      
         rcvrs[i], dt);
	      return 0;
     // gettimeofday(&end, NULL);
      //dt = end.tv_sec-start.tv_sec+(end.tv_usec-start.tv_usec)/1000000.0;
     // printf("Done testing %s with %s, %f sec required\n", sndrs[j],
	    // rcvrs[i], dt);
    }
  }
}
  printf("waiting for all tests to complete\n");
  for(int i=0;i<nrcvrs*nsndrs;i++){
    wait(NULL);
  }
  double lowestTime = 10000;
  for (int i=0; i<nrcvrs; i++) {
    for (int j=0; j<nsndrs; j++) {
      if (glob_var[i*nsndrs+j] > 0) {
          if(glob_var[i*nsndrs+j] < lowestTime)
              lowestTime = glob_var[i*nsndrs+j];
      }
    }
  }
  printf("The fastest overall runtime was %f seconds\n", lowestTime);
  printf("Testing complete\n");
  return 0;
}
