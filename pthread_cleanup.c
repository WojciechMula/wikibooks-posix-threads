/*
 *	Przyk³adowy program dla kursu "POSIX Threads" z wikibooks.pl
 *
 *	Temat: funkcje finalizuj±ce (cleanup)
 *
 *	Autor: Wojciech Mu³a
 *	Ostatnia zmiana: 2010-03-xx
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>	// sleep

#define test_errno(info) do {if (errno) {perror(info); exit(EXIT_FAILURE);}} while(0)

/* funkcja finalizuj±ca */
void zwolnij_pamiec(void* adres) {
	printf("zwalnianie pamiêci spod adresu %p\n", adres);
	free(adres);
}
//------------------------------------------------------------------------

void* watek(void* id) {
	char* tablica1 = malloc(100);
	char* tablica2 = NULL;
	printf("w±tek #%d zaalokowa³ 100 bajtów pod adresem %p\n", (int)id, tablica1);

	pthread_cleanup_push(zwolnij_pamiec, tablica1);
	if (tablica1) {
		tablica2 = malloc(200);
		printf("w±tek #%d zaalokowa³ 200 bajtów pod adresem %p\n", (int)id, tablica2);
		pthread_cleanup_push(zwolnij_pamiec, tablica2);

		if ((int)id > 0)
			/* w±tek siê koñczy w tym punkcie, funkcje finalzuj±ce
			   zostan± uruchomione */
			pthread_exit(NULL);

		pthread_cleanup_pop(1);
	}
	
	pthread_cleanup_pop(1);

	printf("w±tek #%d zakoñczy³ siê\n", (int)id);
	return NULL;
}
//------------------------------------------------------------------------

int main() {
	pthread_t id1;
	pthread_t id2;

	/* utworzenie 2 w±tków */
	errno = pthread_create(&id1, NULL, watek, (void*)(0));
	test_errno("pthread_create (1)");

	errno = pthread_create(&id2, NULL, watek, (void*)(1));
	test_errno("pthread_create (2)");

	/* oczekiwanie na zakoñczenie */
	pthread_join(id1, NULL);
	pthread_join(id2, NULL);

	return EXIT_SUCCESS;
}
//------------------------------------------------------------------------
