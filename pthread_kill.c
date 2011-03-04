/*
 *	Przyk³adowy program dla kursu "POSIX Threads" z wikibooks.pl
 *
 *	Temat: wysy³anie sygna³ow UNIX-owych do w±tków
 *
 *	Autor: Wojciech Mu³a
 *	Ostatnia zmiana: 2010-03-xx
 */

#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>	// sleep

#define test_errno(msg) do{if (errno) {perror(msg); exit(EXIT_FAILURE);}} while(0)


pthread_t main_id;	// id g³ównego w±tek

// funkcja w±tku
void* watek(void* nieuzywany) {
	puts("\tw±tek siê rozpocz±³");
	sleep(1);
	
	puts("\tw±tek wysy³a sygna³ SIGUSR1 do g³ównego w±tku");
	errno = pthread_kill(main_id, SIGUSR1);
	test_errno("pthread_kill");

	return NULL;
}
//------------------------------------------------------------------------

int main() {
	pthread_t id;
	int signum;
	sigset_t mask;

	// blokowanie SIGUSR1
	sigemptyset(&mask);
	sigaddset(&mask, SIGUSR1);

	errno = pthread_sigmask(SIG_BLOCK, &mask, NULL);
	test_errno("pthread_kill");

	// odczyt id g³ównego w±tku
	main_id = pthread_self();

	// utworzenie w±tku
	errno = pthread_create(&id, NULL, watek, NULL);
	test_errno("pthread_create");


	// oczekiwanie na sygna³
	puts("w±tek g³ówny oczekuje na sygna³");
	
	sigwait(&mask, &signum);
	test_errno("sigwait");
	if (signum == SIGUSR1)
		puts("w±tek g³ówny otrzyma³ sygna³ SIGUSR1");
	else
		printf("w±tek g³ówny otrzyma³ sygna³ %d\n", signum);

	return EXIT_SUCCESS;
}
//------------------------------------------------------------------------
