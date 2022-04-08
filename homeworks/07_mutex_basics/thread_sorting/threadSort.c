#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

// max number of values for each thread to sort
#define MAX_VALS_PER_THREAD 1000
// max threads allowed
#define MAX_N_SIZE 100
int vals_per_thread;
int n;
/* other global variable instantiations can go here */

/* Uses a brute force method of sorting the input list. */
void BruteForceSort(int inputList[], int inputLength) {
  int i, j, temp;
  for (i = 0; i < inputLength; i++) {
    for (j = i+1; j < inputLength; j++) {
      if (inputList[j] < inputList[i]) {
        temp = inputList[j];
        inputList[j] = inputList[i];
        inputList[i] = temp;
      }
    }
  }
}

/* Uses the bubble sort method of sorting the input list. */
void BubbleSort(int inputList[], int inputLength) {
  char sorted = 0;
  int i, temp;
  while (!sorted) {
    sorted = 1;
    for (i = 1; i < inputLength; i++) {
      if (inputList[i] < inputList[i-1]) {
        sorted = 0;
        temp = inputList[i-1];
        inputList[i-1] = inputList[i];
        inputList[i] = temp;
      }
    }
  }
}

/* Merges two arrays.  Instead of having two arrays as input, it
 * merges positions in the overall array by re-ording data.  This 
 * saves space. */
void Merge(int *array, int left, int mid, int right) {
  int tempArray[MAX_VALS_PER_THREAD];
  int pos = 0, lpos = left, rpos = mid;
  while (lpos <= mid && rpos <= right) {
    if (array[lpos] < array[rpos]) {
      tempArray[pos++] = array[lpos++];
    } else {
      tempArray[pos++] = array[rpos++];
    }
  }
  while (lpos < mid)  tempArray[pos++] = array[lpos++];
  while (rpos <= right)tempArray[pos++] = array[rpos++];
  int iter;
  for (iter = 0; iter < pos; iter++) {
    array[iter+left] = tempArray[iter];
  }
  return;
}

/* Divides an array into halfs to merge together in order. */
void MergeSort(int array[], int inputLength) {
  int mid = inputLength/2;
  if (inputLength > 1) {
    MergeSort(array, mid);
    MergeSort(array + mid, inputLength - mid);
    // merge's last input is an inclusive index
    printf("calling merge 0->%d, 1->%d\n mid %d\n",array[0], array[1], mid); 
    Merge(array, 0, mid, inputLength - 1);
  }
}

// you might want some globals, put them here

// here's a global I used you might find useful
char* descriptions[] = {"brute force","bubble","merge"};

// I wrote a function called thread dispatch which parses the thread
// parameters and calls the correct sorting function
//
// you can do it a different way but I think this is easiest
typedef struct arg{
   int group;
   int* array;
   int start;
   int end;
};

void* thread_dispatch(void* data) { 
  struct arg* thread = (struct arg*) data;
  for(int i = 0;i<n;i++){
  if(thread[i].group == 1){
    printf("Sorting indexes %d-%d with brute force\n", thread[i].start,thread[i].end);
    BruteForceSort(thread[i].array+thread[i].start,thread[i].end-thread[i].start+1);
  }
  if(thread[i].group == 2){
    printf("Sorting indexes %d-%d with bubble\n",thread[i].start,thread[i].end);
    BubbleSort(thread[i].array+thread[i].start,thread[i].end-thread[i].start+1);
  }
  if(thread[i].group == 3){
    printf("Sorting indexes %d-%d with merge\n",thread[i].start,thread[i].end);
    MergeSort(thread[i].array+thread[i].start,thread[i].end-thread[i].start+1);
  }
  }
}

int main(int argc, char** argv) {

    if(argc < 3) {
        printf("not enough arguments!\n");
        exit(1);
    }

    // I'm reading the value n (number of threads) for you off the
    // command line
    n = atoi(argv[1]);
    pthread_t tid[n];
    if(n <= 0 || n > MAX_N_SIZE || n % 3 != 0) {
        printf("bad n value (number of threads) %d.  Must be a multiple of 3.\n", n);
        exit(1);
    }

    // I'm reading the number of values you want per thread
    // off the command line
   vals_per_thread = atoi(argv[2]);
    if(vals_per_thread <= 0 || vals_per_thread > MAX_VALS_PER_THREAD) {
        printf("bad vals_per_thread value %d\n", vals_per_thread);
        exit(1);
    }

    int total_nums = n * vals_per_thread;
    int* data_array = malloc(sizeof(int) * total_nums);
    if(data_array == NULL) {
        perror("malloc failure");
        exit(1);
    }

    // initialize the test data for sort
    for(int i = 0; i < total_nums; i++) {
        // big reverse sorted list
        data_array[i] = total_nums - i;
        // the test would be more traditional if we used random
        // values, but this makes it easier for you to visually
        // inspect and ensure you're sorting everything
    }

    // create your threads here
    int group;
    struct arg threads[n];
    for(int i=0;i<n;i++){
      if((i+1)%3==0){
        threads[i].group = 3;
      }
      else if((i+1)%2==0){
        threads[i].group = 2;
      }
      else{
        threads[i].group = 1;
      }
      threads[i].start = i*vals_per_thread;
      threads[i].end = (i+1)*vals_per_thread-1;
      threads[i].array = data_array;
      pthread_create(&tid[i],NULL,thread_dispatch,&threads[i]);
    }
    // wait for them to finish
    for(int i = 0; i<n; i++){
      pthread_join(tid[i],NULL);
    }
    // print out the algorithm summary statistics

    // print out the result array so you can see the sorting is working
    // you might want to comment this out if you're testing with large data sets
    printf("Result array:\n");
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < vals_per_thread; j++) {
            printf("%d ", data_array[i*vals_per_thread + j]);
        }
        printf("\n");
    }

    free(data_array); // we should free what we malloc
}
