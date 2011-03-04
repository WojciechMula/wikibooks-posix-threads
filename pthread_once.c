/*
 *	Przyk³adowy program dla kursu "POSIX Threads" z wikibooks.pl
 *
 *	Temat: funkcje wykonywane jednokrotnie
 *
 *	Autor: Wojciech Mu³a
 *	Ostatnia zmiana: 2010-03-xx
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>

#define test_errno(msg) do{if (errno) {perror(msg); exit(EXIT_FAILURE);}} while(0)

/* obiekt gwarantuj±cy jednokrotne wykonanie, musi zostaæ zainicjowany */
pthread_once_t program_gotowy = PTHREAD_ONCE_INIT;

void inicjalizacja() {
	/* inicjalizacja, np. prekalkulowanie jaki¶ tablic,
	   otwieranie pliku logowania itp. */
	puts("Rozpoczynanie programu");
}
//------------------------------------------------------------------------

void* watek(void* numer) {
	pthread_once(&program_gotowy, inicjalizacja);

	printf("Uruchomiono watek nr %d\n", (int)numer);
	return NULL;
}
//------------------------------------------------------------------------

#define N 10 /* liczba w±tków */

int main() {
	pthread_t id[N];
	int i;

	/* utworzenie w±tków */
	for (i=0; i < N; i++) {
		errno = pthread_create(&id[i], NULL, watek, (void*)i);
		test_errno("pthread_create");
	}

	/* oczekiwanie na jego zakoñczenie */
	for (i=0; i < N; i++) {
		errno = pthread_join(id[i], NULL);
		test_errno("pthread_join");
	}

	return EXIT_SUCCESS;
}
//------------------------------------------------------------------------
