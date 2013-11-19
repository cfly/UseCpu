/*
 * cc.c
 *
 *  Created on: Nov 19, 2013
 *      Author: caof
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>

long getmmsecond() {
	/*
	 long ms; // Milliseconds
	 time_t s;  // Seconds
	 struct timespec spec;

	 clock_gettime(CLOCK_REALTIME, &spec);

	 s = spec.tv_sec;
	 return ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
	 */
	struct timeval te;
	gettimeofday(&te, NULL); // get current time
	long long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000; // caculate milliseconds
	// printf("milliseconds: %lld\n", milliseconds);
	return milliseconds;
}

#define COUNT 1

const int INTERVAL = 100;
int usecpu(int c) {
	int i;
	int busySpan[COUNT];
	int idleSpan[COUNT];
	long startTime;
	long runTime;
	for (i = 0; i < COUNT; i++) {
		busySpan[i] = INTERVAL * ((double) c / 100);
		idleSpan[i] = INTERVAL - busySpan[i];
	}
	i = 0;
	while (1) {
		i %= COUNT;
		startTime = getmmsecond();
		while ((runTime = getmmsecond() - startTime) <= busySpan[i]) {
//			printf("getmmsecond():\%ld\tstartTime:\%ld\trunTime:%ld\n",
//					getmmsecond(), startTime, runTime);
		}
//		printf("getmmsecond():\%ld\tstartTime:\%ld\trunTime:%ld\n",
//				getmmsecond(), startTime, runTime);
//		printf("usleep(idleSpan[i]):%d\n", idleSpan[i]);
		usleep(idleSpan[i] * 1000);
		i++;
	}
	return 0;
}

pthread_t ntid;

void printids(const char *s) {
	pid_t pid;
	pthread_t tid;
	pid = getpid();
	tid = pthread_self();
	printf("%s pid %u tid %u (0x%x)\n", s, (unsigned int) pid,
			(unsigned int) tid, (unsigned int) tid);
}

void *thr_fn(void *cpuPercent) {
	//printids("new thread: ");
	usecpu((int)cpuPercent);
	return ((void *) 0);
}

int main(int argc, char **argv) {
	printf("process start\n");
	int *cpuPercent = (int*)atoi(argv[1]);
	int duration = atoi(argv[2]);
	printf("this process will use %d%%cpu %dsecond\n", atoi(argv[1]),duration);
//	if(sizeof(argv)!=3){
//		printf("usage java org.caofei.Java cpu% second");
//		printf("excemp: java org.caofei.Java 15 60");
//		return -1;
//	}
//	if (!args[0].matches("\\d{1,2}")) {
//		printf("cpu percent is number");
//		return -2;
//	}
//	int c = Integer.parseInt(args[0]);
//	if (c < 0 && c > 100) {
//		printf("cpu percent range [0-100]");
//		return -3;
//	}
//	if (!args[1].matches("\\d+")) {
//		printf("second is number");
//		return -4;
//	}
//	int second = Integer.parseInt(args[1]);



	//	usecpu(15);
	int err;
	long availableProcessors;
	availableProcessors = sysconf(_SC_NPROCESSORS_ONLN);
	//	printf("availableProcessors:%ld\n", availableProcessors);
	int i = 0;
	for (; i < availableProcessors; ++i) {
		err = pthread_create(&ntid, NULL, thr_fn, cpuPercent);
		if (err)
			usecpu(30);
//		printf("can't create thread: %s\n", strerror(err));
	}
	sleep(duration);
	printf("process end\n");
	exit(0);
}
