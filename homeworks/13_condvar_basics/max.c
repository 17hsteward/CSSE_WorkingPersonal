#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int max = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condmax = PTHREAD_COND_INITIALIZER;

void *thread(void *arg)
{
	char *letter = (char *)arg;
	
	
	printf("%c wants to enter the critical section\n", *letter);
    pthread_mutex_lock(&lock);
	while(max>= 3){
		pthread_cond_wait(&condmax,&lock);
	}
	pthread_mutex_unlock(&lock);
	max++;
	printf("%c is in the critical section\n", *letter);
	sleep(1);
	printf("%c has left the critical section\n", *letter);
	pthread_cond_signal(&condmax);
    max--;
	return NULL;
}

int
main(int argc, char **argv)
{
	pthread_t threads[8];
	int i;
	char *letters = "abcdefgh";

	for(i = 0; i < 8; ++i) {
		pthread_create(&threads[i], NULL, thread, &letters[i]);

		if(i == 4)
			sleep(4);
	}

	for(i = 0; i < 8; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("Everything finished...\n");

	return 0;
}
