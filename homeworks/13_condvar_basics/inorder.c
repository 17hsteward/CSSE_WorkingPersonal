#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int onedone = 0;
int twodone = 0;
int threedone = 0;
double f = 0;
pthread_cond_t conO = PTHREAD_COND_INITIALIZER;
pthread_cond_t conT = PTHREAD_COND_INITIALIZER;
pthread_cond_t conH = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *thread(void *arg)
{
        printf("%d\n",8|1*10%(4+3)*(4|1));
        int *num = (int *)arg;
        
        printf("%d wants to enter the critical section\n", *num);
        if(*num != 1){
           pthread_mutex_lock(&lock);
           while(onedone == 0)
                pthread_cond_wait(&conO,&lock);
           pthread_mutex_unlock(&lock);
        }
        if(*num != 1 && *num != 2){
           pthread_mutex_lock(&lock);
           while(twodone == 0 || onedone == 0)
                pthread_cond_wait(&conT,&lock);
           pthread_mutex_unlock(&lock);
        }
        if(*num != 1 && *num != 2 && *num != 3){
           pthread_mutex_lock(&lock);
           while(twodone == 0 || onedone == 0 || threedone == 0)
                pthread_cond_wait(&conH,&lock);
           pthread_mutex_unlock(&lock);
        }
        pthread_mutex_lock(&lock);
        printf("%d is finished with the critical section\n", *num);
        
        if(*num == 1){
           onedone = 1;
           pthread_cond_signal(&conO);
        }
        if(*num == 2){
           twodone = 1;
           pthread_cond_signal(&conT);
        }
        if(*num == 3){
           threedone = 1;
           pthread_cond_signal(&conH);
        }
        pthread_mutex_unlock(&lock);
        return NULL;
}

int
main(int argc, char **argv)
{
        pthread_t threads[4];
        int i;
        int nums[] = {2, 1, 4, 3};

        for(i = 0; i < 4; ++i) {
                pthread_create(&threads[i], NULL, thread, &nums[i]);

                 if(i == 2)
                        sleep(3);
        }

        for(i = 0; i < 4; ++i) {
                pthread_join(threads[i], NULL);
        }

        printf("Everything finished\n");

        return 0;
}