/*
 *	Przyk³adowy program dla kursu "POSIX Threads" z wikibooks.pl
 *
 *	Temat: zmienne warunkowe
 *
 *	Autor: Wojciech Mu³a
 *	Ostatnia zmiana: 2010-03-xx
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>	// sleep

pthread_mutex_t	mutex	= PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t	cond	= PTHREAD_COND_INITIALIZER;

char warunek = 0;

void* watek(void* numer) {
	printf("\turuchomiono w±tek #%d\n", (int)numer);
	while (1) {
		pthread_mutex_lock(&mutex);
		do {
			if (warunek)
				break;
			else {
				printf("\tw±tek #%d oczekuje na sygna³...\n", (int)numer);
				pthread_cond_wait(&cond, &mutex);
				printf("\t... w±tek #%d otrzyma³ sygna³!\n", (int)numer);
			}
		} while (1);
		pthread_mutex_unlock(&mutex);
		/* ... */
	}

	return NULL;
}

#define N 5	/* liczba w±tków */

int main() {
	pthread_t id[N];
	int i;

	puts("pocz±tek programu");

	/* utworzenie w±tków */
	for (i=0; i < N; i++) {
		errno = pthread_create(&id[i], NULL, watek, (void*)(i+1));
		if (errno) {
			perror("pthread_create");
			return EXIT_FAILURE;
		}
	}

	/* wysy³anie sygna³ów */

	sleep(1);
	puts("pthread_cond_signal - sygnalizacja");
	pthread_cond_signal(&cond);
	
	sleep(1);
	puts("pthread_cond_broadcast - rozg³aszanie");
	pthread_cond_broadcast(&cond);

	sleep(1);

	/* koñczymy proces, bez ogl±dania siê na w±tki */
	puts("koniec programu");
	return EXIT_SUCCESS;
}
