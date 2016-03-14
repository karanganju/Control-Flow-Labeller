#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <signal.h>
#include <sys/time.h>
using namespace std;

int NUM_THREADS;
int work_time;
int victim_time;
int mult;
timer_t timer;

void usage(){
	printf("./spy SPY_TIME VICTIM_TIME MULT\n");
	exit(0);
}

timespec calc_deadline(int nanosecs){
	struct timespec ret;
	ret.tv_nsec = nanosecs;
	ret.tv_sec = 0;
	if(ret.tv_nsec >= 1000000000) {
	    ret.tv_sec += ret.tv_nsec / 1000000000;
	    ret.tv_nsec %= 1000000000;
	}
	return ret;
}

void alarmed_busy_wait(){
	//Set alarm here!
	struct itimerspec interval; 
    interval.it_interval.tv_sec = 0;
    interval.it_interval.tv_nsec = 0;
    interval.it_value.tv_sec = 0;
    interval.it_value.tv_nsec = work_time;
    if (timer_settime (timer, 0, &interval, NULL) == -1) printf("Unable to set timer!\n"); 

	int i = 0;
	while(1) i=(i+1)%10000000;
}

void alarm_handler(int signum){
	timespec rel = calc_deadline(NUM_THREADS*(victim_time+work_time));
	clock_nanosleep(CLOCK_MONOTONIC, 0, &rel, NULL);
	alarmed_busy_wait();
}

void *thread_func(void* arg){
	long long int tid = (long long int)arg;
	timespec rel = calc_deadline(tid*(victim_time+work_time));
	if(tid != 0) clock_nanosleep(CLOCK_MONOTONIC, 0, &rel, NULL);
	//Set alarm handler
	struct sigaction action;
	action.sa_handler = alarm_handler;
	if (sigaction (SIGALRM, &action, NULL) == -1) printf("Not able to create timer signal handler.\n");
	//Set timer
	if (timer_create (CLOCK_MONOTONIC, NULL, &timer) == -1) printf("Not able to create timer.\n"); 
	alarmed_busy_wait();
	pthread_exit(NULL);
}

int main(int argc, char* argv[]){
	if(argc != 4) usage();
	else {
		work_time = atoi(argv[1]);
		victim_time = atoi(argv[2]);
		mult = atoi(argv[3]);
		NUM_THREADS = mult*ceil(double(work_time)/double(victim_time));
	}

	cpu_set_t bitmask;
	CPU_ZERO(&bitmask);
	CPU_SET(1, &bitmask);
	sched_setaffinity(0, sizeof(cpu_set_t), &bitmask);

	struct timeval tval_result;
	gettimeofday(&tval_result, NULL);
    printf("Started at: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);

	pthread_t threads[NUM_THREADS];
	for (long long int i = 0; i < NUM_THREADS; ++i)
	{
		int ret = pthread_create(&threads[i],NULL,thread_func,(void *)i);
		if (ret) {
			printf("%d - Thread Creation failed!\n",ret);
			exit(-1);
		}
	}
	pthread_exit(NULL);
}