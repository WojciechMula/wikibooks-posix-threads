/*
 *	Przyk³adowy program dla kursu "POSIX Threads" z wikibooks.pl
 *
 *	Temat:	przerywanie w±tków - program tworzy 3 w±tki z ró¿nymi
 *		ustawieniami dotycz±cymi przerywania:
 *		1) dopuszcza przerwanie asynchroniczne
 *		2) dopuszcza przerwanie opó¼nione
 *		3) przez pewien czas w ogóle blokuje przerwania
 *
 *	Autor: Wojciech Mu³a
 *	Ostatnia zmiana: 2010-03-xx
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>	// pause w watek3
#include <errno.h>

#define test_errno(msg) do{if (errno) {perror(msg); exit(EXIT_FAILURE);}} while(0)

void zakonczenie(void* numer) {
	printf("funkcja finalizuj±ca dla w±tku #%d\n", (int)numer);
}
//------------------------------------------------------------------------

void* watek1(void* numer) {
	int i, n;
	
	pthread_cleanup_push(zakonczenie, numer);
	
	errno = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	test_errno("pthread_setcancelstate");
	
	errno = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	test_errno("pthread_setcanceltype");

	printf("\turuchomiono w±tek #%d (przerwanie asynchroniczne)\n", (int)numer);
	while (1) {
		n = 1000000;
		for (i=0; i < n; i++)
			/**/;
	}

	pthread_cleanup_pop(1);
	return NULL;
}
//------------------------------------------------------------------------

void* watek2(void* numer) {
	int i, n;
	pthread_cleanup_push(zakonczenie, numer);
	
	errno = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	test_errno("pthread_setcancelstate");
	
	errno = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	test_errno("pthread_setcanceltype");

	printf("\turuchomiono w±tek #%d (przerwanie opó¼nione)\n", (int)numer);
	while (1) {
		pthread_testcancel();	// punkt przerwania
		n = 1000000;
		for (i=0; i < n; i++)
			/**/;
	}

	pthread_cleanup_pop(1);
	return NULL;
}
//------------------------------------------------------------------------

void* watek3(void* numer) {
	pthread_cleanup_push(zakonczenie, numer);
	errno = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	test_errno("pthread_setcancelstate");
	
	printf("\turuchomiono w±tek #%d (przez 2 sekundy nie mo¿na przerwaæ)\n", (int)numer);
	
	sleep(2);
	
	printf("\tw±tek #%d mo¿na ju¿ przerwaæ\n", (int)numer);
	errno = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	test_errno("pthread_setcancelstate");
	pause();
	
	pthread_cleanup_pop(1);
	return NULL;
}
//------------------------------------------------------------------------

void przerwanie(pthread_t id, const char* napis) {
	printf("%s: wysy³anie sygna³u przerwania do w±tku\n", napis);
	errno = pthread_cancel(id);
	test_errno("pthread_cancel");
	
	printf("%s: wys³ano, oczekiwanie na zakoñczenie\n", napis);
	errno = pthread_join(id, NULL);
	test_errno("pthread_join");

	printf("%s: w±tek zakoñczony\n", napis);
}
//------------------------------------------------------------------------

int main() {
	pthread_t id[3];

	/* utworzenie w±tków */
	errno = pthread_create(&id[0], NULL, watek1, (void*)(0));
	test_errno("pthread_create (1)");
	
	errno = pthread_create(&id[1], NULL, watek2, (void*)(1));
	test_errno("pthread_create (2)");
		
	errno = pthread_create(&id[2], NULL, watek3, (void*)(2));
	test_errno("pthread_create (3)");

	/* przerywanie kolejnych w±tków */
	przerwanie(id[0], "#0");
	przerwanie(id[1], "#1");
	przerwanie(id[2], "#2");

	return EXIT_SUCCESS;
}
//------------------------------------------------------------------------
