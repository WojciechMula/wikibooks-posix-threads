/*
 *	Przyk³adowy program dla kursu "POSIX Threads" z wikibooks.pl
 *
 *	Temat: blokady zapis/odczy (rwlock)
 *
 *	Autor: Wojciech Mu³a
 *	Ostatnia zmiana: 2010-03-xx
 */

#define _POSIX_C_SOURCE	200809L
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>

void ms_sleep(const unsigned ms);
#define test_errno(msg) do{if (errno) {perror(msg); exit(EXIT_FAILURE);}} while(0)


pthread_rwlock_t	blokada;
int			wartosc;	// obiekt chroniony blokad±

/* w±tek zmienia warto¶æ */
void* pisarz(void* numer) {
	while (1) {
		printf(" pisarz #%d czeka na dostêp\n", (int)numer);
		errno = pthread_rwlock_wrlock(&blokada);
		test_errno("pthread_rwlock_wrlock");
		printf(" pisarz #%d ustawia now± warto¶æ\n", (int)numer);

		ms_sleep(113);
		
		printf(" pisarz #%d zwalnia blokadê\n", (int)numer);
		errno = pthread_rwlock_unlock(&blokada);
		test_errno("pthread_rwlock_unlock");
		
		ms_sleep(317);
		
	}

	return NULL;
}
//------------------------------------------------------------------------

/* w±tek tylko odczytuje warto¶æ */
void* czytelnik(void* numer) {
	int errno;
	while (1) {
		printf("  czytelnik #%d czeka na dostêp\n", (int)numer);
		errno = pthread_rwlock_rdlock(&blokada);
		test_errno("pthread_rwlock_rdlock");
		printf("  czytelnik #%d odczytuje warto¶æ\n", (int)numer);

		ms_sleep(13);
		
		printf("  czytelnik #%d zwalnia blokadê\n", (int)numer);
		errno = pthread_rwlock_unlock(&blokada);
		test_errno("pthread_rwlock_unlock");
		
		ms_sleep(13);
		
	}
	return NULL;
}
//------------------------------------------------------------------------

#define N 5 /* liczba w±tków */
#define K 2

int main() {
	pthread_t id;
	int i;

	pthread_rwlock_init(&blokada, NULL);
	
	/* utworzenie K w±tków pisz±cych */
	for (i=0; i < K; i++) {
		errno = pthread_create(&id, NULL, pisarz, (void*)i);
		test_errno("pthread_create");
	}

	/* utworzenie N w±tków czytaj±cych */
	for (i=0; i < N; i++) {
		errno = pthread_create(&id, NULL, czytelnik, (void*)i);
		test_errno("pthread_create");
	}

	/* kilka sekund na pracê w±tków i koniec */
	ms_sleep(1500);

	return EXIT_SUCCESS;
}
//------------------------------------------------------------------------

void ms_sleep(const unsigned ms) {
	struct timespec req;
	req.tv_sec  = (ms / 1000);
	req.tv_nsec = (ms % 1000 * 1000000);
	nanosleep(&req, NULL);
}
//------------------------------------------------------------------------
