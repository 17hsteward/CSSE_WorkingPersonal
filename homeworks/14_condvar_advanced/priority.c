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
int work = 0;
int max = 0;

void *thread(void *arg)
{
    
	int *num = (int *) arg;
	
	
	
	printf("%d wants to enter the critical section\n", *num);
	if(*num>max){
		max = *num;
	}
	while(*num!= max){
		pthread_mutex_lock(&lock);
		pthread_cond_wait(&conWork,&lock);
		pthread_mutex_unlock(&lock);
	}
    pthread_mutex_lock(&lock);
	printf("%d has entered the critical section\n", *num);
	sleep(1);
	printf("%d is finished with the critical section\n", *num);
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
