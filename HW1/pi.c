#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

long thread_count; 
long long num_of_tosses; 
pthread_mutex_t mutex;
double pi_estimate = 0.0;

void * Thread_sum(void * rank);

int main(int argc, char *argv[]) {
	

	long       thread;  /* Use long in case of a 64-bit system */ 
	pthread_t* thread_handles; 
	

	thread_count= strtol(argv[1], NULL, 10); 
	num_of_tosses = strtoll(argv[2], NULL, 10);
	 
	thread_handles= (pthread_t*) malloc(thread_count*sizeof(pthread_t)); 
	pthread_mutex_init(&mutex, NULL); 
	//產生thread並執行function 
	for (thread = 0; thread < thread_count; thread++)  
		pthread_create(&thread_handles[thread], NULL, Thread_sum, (void*)thread);  
	//等當前thread執行完成才繼續 
	for (thread = 0; thread < thread_count; thread++) 
		pthread_join(thread_handles[thread], NULL); 
	
	pi_estimate = 4*(pi_estimate/(double)num_of_tosses);
	printf("%f\n", pi_estimate);
	
	pthread_mutex_destroy(&mutex); 
	free(thread_handles); 
    return 0;
}

void * Thread_sum(void * rank)
{
	long toss = (long) rank; 
	long long i;
	long long my_n = num_of_tosses/thread_count;
	long my_first_i = my_n * toss;
	long my_last_i = my_first_i + my_n;
	
	
	double number_in_circle = 0.0;
	unsigned int rand_num = rand();

	srand(time(NULL));
	for(i = my_first_i; i < my_last_i ; i++){
		double x = rand_r(&rand_num)/(double)RAND_MAX * 2 - 1;
		double y = rand_r(&rand_num)/(double)RAND_MAX * 2 - 1;
		
		if(x*x + y*y <= 1){
			number_in_circle++;
		}
		
	}
	
	pthread_mutex_lock(&mutex); 
	pi_estimate += number_in_circle;
	pthread_mutex_unlock(&mutex);

    return NULL;
    
}
