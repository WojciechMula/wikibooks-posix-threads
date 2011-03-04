#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sched.h>

#include <errno.h>
#include <unistd.h>

void* watek(void* numer) {
	while (1) {
		printf("w±tek #%d dzia³a na CPU#%d\n",
			(int)numer,
			sched_getcpu()
		);
		usleep(20*1000);
	}

	return NULL;
}
//------------------------------------------------------------------------

#define test_errno(info) do {if (errno) {perror(info); exit(EXIT_FAILURE);}} while(0)

int main(int argc, char* argv[]) {
	pthread_t id1, id2;
	pthread_attr_t attr1, attr2;
	cpu_set_t cpuset;

	puts("pocz±tek programu");

	errno = pthread_attr_init(&attr1);
	test_errno("pthread_attr_init (1)");
	
	errno = pthread_attr_init(&attr1);
	test_errno("pthread_attr_init (2)");

	if (argc > 1) { // ustawienie maski, tylko gdy s± jakie¶ argumenty

		// pierwszy w±tek tylko na procesorze #0
		CPU_ZERO(&cpuset);
		CPU_SET(0, &cpuset);
		errno = pthread_attr_setaffinity_np(&attr1, sizeof(cpuset), &cpuset);
		test_errno("pthread_attr_setaffinity_np (1)");
		
		// drugi w±tek tylko na procesorze #1
		CPU_ZERO(&cpuset);
		CPU_SET(1, &cpuset);
		errno = pthread_attr_setaffinity_np(&attr2, sizeof(cpuset), &cpuset);
		test_errno("pthread_attr_setaffinity_np (2)");
	}

	/* utworzenie dwóch w±tków */
	errno = pthread_create(&id1, &attr1, watek, (void*)(0));
	test_errno("pthread_create (1)");
	
	errno = pthread_create(&id2, &attr2, watek, (void*)(1));
	test_errno("pthread_create (2)");

	/* chwilka na pracê w±tków */
	sleep(2);

	/* koniec procesu, bez ogl±dania siê na w±tki */
	puts("koniec programu");

	pthread_attr_destroy(&attr1);
	pthread_attr_destroy(&attr2);

	return EXIT_SUCCESS;
}
//------------------------------------------------------------------------
