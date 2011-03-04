/*
 *	Przyk³adowy program dla kursu "POSIX Threads" z wikibooks.pl
 *
 *	Temat: 	sekcja krytyczna z u¿yciem mutexów 
 *		je¶li zdefiniowane zostanie BLOKADA, mutex blokuje
 *		dostêp do zmiennej, w przeciwnym razie w±tki zmieniaj±
 *		j± bez ¿adnej synchronizacji, co mo¿e prowadziæ do b³êdu
 *
 *	Autor: Wojciech Mu³a
 *	Ostatnia zmiana: 2010-03-xx
 */

#define _POSIX_C_SOURCE	200809L
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>

#define test_errno(msg) do{if (errno) {perror(msg); exit(EXIT_FAILURE);}} while(0)


#define	N 10	/* liczba w±tków */
#define K 1000	/* liczba iteracji (z t± warto¶ci± nale¿y eksperymentowaæ) */

pthread_mutex_t blokada;
int licznik = 0;		// globalny licznik, powinien byæ chroniony blokad±

void ms_sleep(unsigned ms) {
	struct timespec req;
	req.tv_sec  = (ms / 1000);
	req.tv_nsec = (ms % 1000 * 1000000);
	nanosleep(&req, NULL);
}
//------------------------------------------------------------------------

void* watek(void* numer) {
	int i;
	for (i=0; i < K; i++) {
#ifdef BLOKADA
		errno = pthread_mutex_lock(&blokada);
		test_errno("pthread_mutex_lock");
#endif
		licznik = licznik + 1;
		ms_sleep(1);
#ifdef BLOKADA
		errno = pthread_mutex_unlock(&blokada);
		test_errno("pthread_mutex_unlock");
#endif
	}

	return NULL;
}
//------------------------------------------------------------------------

int main() {
	pthread_t id[N];
	int i;
	
	printf("licznik = %d\n", licznik);

	errno = pthread_mutex_init(&blokada, NULL);
	test_errno("pthread_mutex_init");

	/* utworzenie w±tku */
	for (i=0; i < N; i++) {
		errno = pthread_create(&id[i], NULL, watek, (void*)i);
		test_errno("pthread_create");
	}

	/* oczekiwanie na jego zakoñczenie */
	for (i=0; i < N; i++) {
		errno = pthread_join(id[i], NULL);
		test_errno("pthread_join");
	}

	printf("licznik = %d, spodziewana warto¶æ = %d %s\n",
		licznik,
		N*K,
		(licznik != N*K ? "B£¡D!!!" : "")
	);

	return EXIT_SUCCESS;
}
//------------------------------------------------------------------------
