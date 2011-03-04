/*
 *	Przyk³adowy program dla kursu "POSIX Threads" z wikibooks.pl
 *
 *	Temat: hello world z kilku w±tków
 *
 *	Autor: Wojciech Mu³a
 *	Ostatnia zmiana: 2010-03-xx
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>

#define test_errno(msg) do{if (errno) {perror(msg); exit(EXIT_FAILURE);}} while(0)

/* funkcja wykonywana w w±tku - nic specjalnego nie robi */
void* watek(void* _arg) {
	puts("Witaj w równoleg³ym ¶wiecie!");
	return NULL;
}
//------------------------------------------------------------------------

#define N 5	/* liczba w±tków */

int main() {
	pthread_t id[N];
	int i;

	/* utworzenie kilku w±tków w±tku */
	for (i=0; i < N; i++) {
		errno = pthread_create(&id[i], NULL, watek, NULL);
		test_errno("Nie powiod³o siê pthread_create");
	}

	/* oczekiwanie na jego zakoñczenie */
	for (i=0; i < N; i++) {
		errno = pthread_join(id[i], NULL);
		test_errno("pthread_join");
	}

	return EXIT_SUCCESS;
}
//------------------------------------------------------------------------
