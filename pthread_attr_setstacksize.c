/*
 *	Przyk³adowy program dla kursu "POSIX Threads" z wikibooks.pl
 *
 *	Temat: zmiana rozmiaru stosu w±tku
 *
 *	Autor: Wojciech Mu³a
 *	Ostatnia zmiana: 2010-03-xx
 */

#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <limits.h>
#include <errno.h>

#define test_errno(msg) do{if (errno) {perror(msg); exit(EXIT_FAILURE);}} while(0)

#define N (100*1024)

/* w±tek u¿ywa sporej tablicy alokowanej na stosie */
void* watek(void* arg) {
	char tablica[N];
	int i;
	for (i=0; i < N; i++)
		tablica[i] = 0;

	return NULL;
}
//------------------------------------------------------------------------

int main(int argc, char* argv[]) {
	pthread_t	id;
	pthread_attr_t	attr;
	size_t		rozmiar;

	errno = pthread_attr_init(&attr);
	if (errno) {
		perror("pthread_attr_init");
		return EXIT_FAILURE;
	}

	if (argc > 1) {
		rozmiar = atoi(argv[1]);
		printf("rozmiar stosu ustalony przez u¿ytkownika: %u\n", rozmiar);
		printf("minimalny rozmiar stosu: %u\n", PTHREAD_STACK_MIN);

		errno = pthread_attr_setstacksize(&attr, rozmiar);
		test_errno("pthread_attr_setstacksize");
	}
	else {
		pthread_attr_getstacksize(&attr, &rozmiar);
		printf("domy¶lny rozmiar stosu: %u\n", rozmiar);
	}

	errno = pthread_create(&id, &attr, watek, NULL);
	test_errno("pthread_create");

	pthread_join(id, NULL);
	puts("w±tek zakoñczony");

	pthread_attr_destroy(&attr);
}
//------------------------------------------------------------------------
