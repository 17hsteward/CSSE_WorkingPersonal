#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


/**
  In this system there are threads numbered 1-6 and a critical
  section.  The thread numbers are priorities, where thread 6 is
  highest priority, thread 5 is next, etc.

  If the critical section is empty, any thread can enter.  While a
  thread is in the critical section no other threads can enter -
  regardless of priority. When a thread leaves the critical section
  and there are threads waiting, the highest priority waiting thread
  is allowed to enter the critical section.

  Be sure a newly arriving thread can't jump into the critical
  section as the current thread finishes, bypassing priority rules.
  Solve this problem with mutexes/condition variables
 **/
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t conWork = PTHREAD_COND_INITIALIZER;
pthread_cond_t conCheck = PTHREAD_COND_INITIALIZER;
int done = 0;
int max = 0;
int array[6];
int i = 5;
int j = 5;
int currentrun = 0;

		
void *thread(void *arg)
{
 
	int *num = (int *) arg;
	array[*num-1] = *num;
	i = 5;
		while(array[i] == 0)
		{
			i--;
		}
		max = array[i];
	
	
	printf("%d wants to enter the critical section\n", *num);
	while(currentrun == 1){
     pthread_mutex_lock(&lock);
	 pthread_cond_wait(&conCheck,&lock);
    pthread_mutex_unlock(&lock);
	 while(*num!= max){
	 	pthread_mutex_lock(&lock);
		pthread_cond_wait(&conWork,&lock);
		pthread_mutex_unlock(&lock);
	 }
	 currentrun =1;
	 
	}
	array[*num-1] = 0;
	j = 5;
	while(array[j] == 0)
	{
		j--;
	}
	max = array[j];
    pthread_mutex_lock(&lock);
	printf("%d has entered the critical section\n", *num);
	sleep(1);
	printf("%d is finished with the critical section\n", *num);
    currentrun = 0;
	pthread_cond_signal(&conCheck);
	pthread_cond_signal(&conWork);
	pthread_mutex_unlock(&lock);
	return NULL;
}

int
main(int argc, char **argv)
{
    
	int i;
	pthread_t threads[6];
	int nums[] = {2, 1, 4, 3, 5, 6};


	for(i = 0; i < 6; i++) {
		pthread_create(&threads[i], NULL, thread, &nums[i]);

		if(i == 2) sleep(10);
	}

	for(i = 0; i < 6; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("Everything finished.\n");
}
