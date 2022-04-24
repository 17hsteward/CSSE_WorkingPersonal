/* Copyright 2021 Rose-Hulman */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

/**
  Imagine a group of friends are getting together to play music, but
  they are arriving at different times.  Arriving can happen at any
  time (e.g. when some other friends are playing).

  There are 3 different kinds of friends - drummers, singers, and
  guitarists.  It takes one of each kind to make a band, plus only
  1 band can be playing at once.  Once those conditions are met, the
  players can start playing and stop playing in any order.  However,
  all 3 players must stop playing before a new set of 3 can start
  playing.

  Example output:

  drummer arrived
  drummer arrived
  guitarist arrived
  guitarist arrived
  singer arrived
  drummer playing
  guitarist playing
  singer playing
  singer arrived
  singer arrived
  drummer arrived
  guitarist arrived
  drummer finished playing
  guitarist finished playing
  singer finished playing
  singer playing
  guitarist playing
  drummer playing
  singer finished playing
  guitarist finished playing
  drummer finished playing
  guitarist playing
  drummer playing
  singer playing
  guitarist finished playing
  drummer finished playing
  singer finished playing
  Everything finished.


 **/

int DRUM = 0;
int SING = 1;
int GUIT = 2;
int drum = 0;
int sing = 0;
int guit = 0;
int instnum = 0;
int done = 0;
pthread_cond_t conDrum = PTHREAD_COND_INITIALIZER;
pthread_cond_t conSing = PTHREAD_COND_INITIALIZER;
pthread_cond_t conGuit = PTHREAD_COND_INITIALIZER;
pthread_cond_t conH = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
char* names[] = {"drummer", "singer", "guitarist"};



// because the code is similar, we'll just have one kind of thread
// and we'll pass its kind as a parameter
void* friend(void * kind_ptr) {
	int kind = *((int*) kind_ptr);
	printf("%s arrived\n", names[kind]);
  if(names[kind]=="drummer"){
     pthread_mutex_lock(&lock);
     while(drum == 1){
       pthread_cond_wait(&conDrum,&lock);
     }
     drum = 1;
     pthread_mutex_unlock(&lock);
  }
  if(names[kind]=="singer"){
     pthread_mutex_lock(&lock);
     while(sing == 1){
       pthread_cond_wait(&conSing,&lock);
     }
     sing = 1;
     pthread_mutex_unlock(&lock);
  }
  if(names[kind]=="guitarist"){
     pthread_mutex_lock(&lock);
     while(guit == 1){
       pthread_cond_wait(&conGuit,&lock);
     }
     guit = 1;
     pthread_mutex_unlock(&lock);
  }
  while(instnum!=3){
    instnum++;
    pthread_mutex_lock(&lock);
     while(instnum != 3){
       pthread_cond_wait(&conH,&lock);
     }
     if(instnum==3){
     pthread_cond_signal(&conH);
     }
     pthread_mutex_unlock(&lock);
  }
  printf("%s playing\n", names[kind]);
	sleep(1);
	printf("%s finished playing\n", names[kind]);
  if (done == 0){
     instnum = 0;
     done = 1;
  }

  if(names[kind]=="drummer"){
    pthread_mutex_lock(&lock);
    drum = 0;
    pthread_cond_signal(&conDrum);
    done++;
    pthread_mutex_unlock(&lock);
  }
   if(names[kind]=="singer"){
     pthread_mutex_lock(&lock);
    sing = 0;
    pthread_cond_signal(&conSing);
    done++;
    pthread_mutex_unlock(&lock);
  }
   if(names[kind]=="guitarist"){
    pthread_mutex_lock(&lock);
    guit = 0;
    pthread_cond_signal(&conGuit);
    done++;
    pthread_mutex_unlock(&lock);
  }
  if(done == 4 )
  done = 0;
	return NULL;
}

pthread_t friends[100];
int friend_count = 0;

void create_friend(int* kind) {
	pthread_create(&friends[friend_count], NULL, friend, kind);
	friend_count++;
}

int main(int argc, char **argv) {

	create_friend(&DRUM);
	create_friend(&DRUM);
	create_friend(&GUIT);
	create_friend(&GUIT);
	sleep(1);
	create_friend(&SING);
	create_friend(&SING);
	create_friend(&DRUM);
	create_friend(&GUIT);
	create_friend(&SING);

	// all threads must be created by this point
	// note if you didn't create an equal number of each, we'll be stuck forever
	for (int i = 0; i < friend_count; i++) {
		pthread_join(friends[i], NULL);
	}

	printf("Everything finished.\n");

}
