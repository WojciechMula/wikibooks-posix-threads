/*
 *	Przyk³adowy program dla kursu "POSIX Threads" z wikibooks.pl
 *
 *	Temat:	zakoñczenie w±tku z poziomu funkcji wyw³ywanych w w±tku
 *		za pomoc± 
 *
 *	Autor: Wojciech Mu³a
 *	Ostatnia zmiana: 2010-03-xx
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>

#define test_errno(msg) do{if (errno) {perror(msg); exit(EXIT_FAILURE);}} while(0)


void koniec_watku(int licznik, int limit) {
	int i;
	for (i=0; i < licznik; i++) putchar(' ');
	printf("licznik = %d, limit = %d\n", licznik, limit);

	if (licznik == limit)
		/* zakoñczenie w±tku w którego kontek¶cie wykonywana jest ta funkcja */
		pthread_exit(NULL);
	else
		koniec_watku(licznik+1, limit);
}
//------------------------------------------------------------------------

void* watek(void* arg) {
	koniec_watku(0, 5);
	return NULL;
}
//------------------------------------------------------------------------

int main() {
	pthread_t id;

	/* utworzenie w±tku */
	errno = pthread_create(&id, NULL, watek, NULL);
	test_errno("pthread_create");

	/* oczekiwanie na jego zakoñczenie */
	errno = pthread_join(id, NULL);
	test_errno("pthread_join");

	return EXIT_SUCCESS;
}
//------------------------------------------------------------------------
