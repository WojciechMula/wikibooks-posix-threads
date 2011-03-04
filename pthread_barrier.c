/*
 *	Przyk³adowy program dla kursu "POSIX Threads" z wikibooks.pl
 *
 *	Temat: bariery
 *
 *	Autor: Wojciech Mu³a
 *	Ostatnia zmiana: 2010-03-xx
 */

#define _POSIX_C_SOURCE 200809L

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define test_errno(msg) do{if (errno) {perror(msg); exit(EXIT_FAILURE);}} while(0)

pthread_barrier_t	bariera;

void* watek(void* numer) {
	int s, status;
	
	s = rand() % 4 + 1; // oczekiwanie 1-4 s
	printf("\tw±tek #%d rozpoczêty, zostanie wstrzymany na %d sekund\n", (int)numer, s);

	sleep(s);	

	printf("\tw±tek #%d osi±gn±³ barierê\n", (int)numer);
	status = pthread_barrier_wait(&bariera);
	switch (status) {
		case 0: // ok
			break;

		case PTHREAD_BARRIER_SERIAL_THREAD:
			printf(
				"\twszystkie w±tki osi±gnê³y barierê "
				"(PTHREAD_BARRIER_SERIAL_THREAD w w±teku #%d)\n",
				(int)numer
			);
			break;

		default:
			fprintf(stderr, "pthread_barrier_wait: %s\n", strerror(status));
			break;
	}
	return NULL;
}
//------------------------------------------------------------------------

#define N 10	/* liczba w±tków */

int main() {
	int i;
	pthread_t	id[N];

	srand(time(NULL));

	printf("zostanie uruchomionych %d w±tków\n", N);

	/* inicjalizacja bariery - N w±tków */
	errno = pthread_barrier_init(&bariera, NULL, N);
	test_errno("pthread_barrier_init");

	/* utworzenie N w±tków */
	for (i=0; i < N; i++) {
		errno = pthread_create(&id[i], NULL, watek, (void*)i);
		test_errno("pthread_create");
	}

	/* oczekiwaie na doj¶cie do bariery wszystkich w±tków */
	for (i=0; i < N; i++) {
		errno = pthread_join(id[i], NULL);
		test_errno("pthread_join");
	}

	/* zwolnienie bariery */
	errno = pthread_barrier_destroy(&bariera);
	test_errno("pthread_barrier_destroy");
	
	return EXIT_SUCCESS;
}
//------------------------------------------------------------------------
