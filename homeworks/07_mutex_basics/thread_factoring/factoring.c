/* Copyright 2016 Rose-Hulman Institute of Technology

Here is some code that factors in a super dumb way.  We won't be
attempting to improve the algorithm in this case (though that would be
the correct thing to do).

Modify the code so that it starts the specified number of threads and
splits the computation among them.  You can be sure the max allowed 
number of threads is 50.  Be sure your threads actually run in parallel.

Your threads should each just print the factors they find, they don't
need to communicate the factors to the original thread.

ALSO - be sure to compile this code with -pthread or just used the 
Makefile provided.

 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

unsigned long long int target;
int numThreads;

void factor(int ID){
   for (int i = 2+ID; i <= target/2 ; i = i + numThreads) {
    /* You'll want to keep this testing line in.  Otherwise it goes so
       fast it can be hard to detect your code is running in
       parallel. Also test with a large number (i.e. > 3000) */
    printf("thread %d testing %llu\n",ID+1,i);
    if (target % i == 0) {
      printf("%llu is a factor\n", i);
    }
  }
}
int main(void) {
  /* you can ignore the linter warning about this */
  unsigned long long int i, start = 0;
  
  pthread_t tid[50];
  printf("Give a number to factor.\n");
  scanf("%llu", &target);
  printf("How man threads should I create?\n");
  scanf("%d", &numThreads);
  if (numThreads > 50 || numThreads < 1) {
    printf("Bad number of threads!\n");
    return 0;
  }
  for( i = 0; i<numThreads;i++){
    printf("running thread\n");
    pthread_create(&tid[i],NULL,factor,i);
  }
  for( i = 0;i<numThreads;i++){
    pthread_join(tid[i],NULL);
  }

  // for (i = 2; i <= target/2; i = i + 1) {
  //   /* You'll want to keep this testing line in.  Otherwise it goes so
  //      fast it can be hard to detect your code is running in
  //      parallel. Also test with a large number (i.e. > 3000) */
  //   printf("testing %llu\n", i);
  //   if (target % i == 0) {
  //     printf("%llu is a factor\n", i);
  //   }
  // }
  return 0;
}

