/*
 *	Przyk�adowy program dla kursu "POSIX Threads" z wikibooks.pl
 *
 *	Temat: wysy�anie sygna�ow UNIX-owych do w�tk�w
 *
 *	Autor: Wojciech Mu�a
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


pthread_t main_id;	// id g��wnego w�tek

// funkcja w�tku
void* watek(void* nieuzywany) {
	puts("\tw�tek si� rozpocz��");
	sleep(1);
	
	puts("\tw�tek wysy�a sygna� SIGUSR1 do g��wnego w�tku");
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

	// odczyt id g��wnego w�tku
	main_id = pthread_self();

	// utworzenie w�tku
	errno = pthread_create(&id, NULL, watek, NULL);
	test_errno("pthread_create");


	// oczekiwanie na sygna�
	puts("w�tek g��wny oczekuje na sygna�");
	
	sigwait(&mask, &signum);
	test_errno("sigwait");
	if (signum == SIGUSR1)
		puts("w�tek g��wny otrzyma� sygna� SIGUSR1");
	else
		printf("w�tek g��wny otrzyma� sygna� %d\n", signum);

	return EXIT_SUCCESS;
}
//------------------------------------------------------------------------
