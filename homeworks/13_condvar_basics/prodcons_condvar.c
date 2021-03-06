#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFERSIZE 5
int buffer[BUFFERSIZE];
int last_valid_index;
int amount = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;

void *
producer(void *arg)
{
        int i;
        int value = *((int*) arg);

        for(i = 0; i < 10; ++i) {
           pthread_mutex_lock(&lock);
           while(amount == BUFFERSIZE)
                pthread_cond_wait(&empty,&lock);
           buffer[last_valid_index + 1] = value;
           last_valid_index++;
           amount ++;
           pthread_cond_signal(&full);
           pthread_mutex_unlock(&lock);
           printf("Produced value %d, stored at %d\n", value, last_valid_index);
          
           value += 1;
        }

        return NULL;
}

void *
consumer(void *arg)
{
        int value, i;

        for(i = 0; i < 10; ++i) {
                sleep(1);
                pthread_mutex_lock(&lock);
                while(amount == 0)
                  pthread_cond_wait(&full,&lock);
                value = buffer[last_valid_index];
                last_valid_index--;
                amount --;
                pthread_cond_signal(&empty);
                pthread_mutex_unlock(&lock);
                printf("Consumed value %d, stored at %d\n", value, last_valid_index+1);
                
        }

        return NULL;
}

int
main(int argc, char **argv)
{
        pthread_t p1, p2, c1, c2;
        int p1start = 100;
        int p2start = 200;

        last_valid_index = -1; // initially, there is no valid data
        pthread_create(&p1, NULL, producer, &p1start);
        pthread_create(&p2, NULL, producer, &p2start);
        pthread_create(&c1, NULL, consumer, NULL);
        pthread_create(&c2, NULL, consumer, NULL);

        pthread_join(p1, NULL);
        pthread_join(p2, NULL);
        pthread_join(c1, NULL);
        pthread_join(c2, NULL);

        printf("Everything finished...\n");
}