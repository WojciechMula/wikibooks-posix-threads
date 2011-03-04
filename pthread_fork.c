/*
 *	Przyk³adowy program dla kursu "POSIX Threads" z wikibooks.pl
 *
 *	Temat: pthread_atfork
 *
 *	Autor: Wojciech Mu³a
 *	Ostatnia zmiana: 2010-03-xx
 */

#define _XOPEN_SOURCE	700
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>	// sleep
#include <sys/wait.h>	// waitpid

#define test_errno(msg) do{if (errno) {perror(msg); exit(EXIT_FAILURE);}} while(0)

void* watek(void* numer) {
	printf("\turuchomiono w±tek #%d\n", (int)numer);
	while (1) {
		printf("\t\tw±tek #%d w procesie #%d\n", (int)numer, getpid());
		usleep(700*1000);
	}

	return NULL;
}
//------------------------------------------------------------------------

#define N 3	/* liczba w±tków */

pthread_t id[N];

void inicjalizacja_watkow() {
	int i;
	printf("tworzenie %d w±tków w procesie %d\n", N, getpid());
	
	/* utworzenie w±tków */
	for (i=0; i < N; i++) {
		errno = pthread_create(&id[i], NULL, watek, (void*)(i+1));
		test_errno("pthread_create");
	}
}
//------------------------------------------------------------------------

int main() {
	pid_t pid;

	puts("pocz±tek programu");
	inicjalizacja_watkow();

	/* rejestrowanie funkcji wykonywanej w procesie potomnym */
	errno = pthread_atfork(NULL, NULL, inicjalizacja_watkow);
	test_errno("pthread_atfork");

	sleep(1);

	pid = fork();
	printf("fork => %d\n", pid);
	switch (pid) {
		case -1:
			test_errno("fork");
			break;

		case 0: // proces potomny
			sleep(2);
			break;

		default: // proces nadrzêdny
			waitpid(pid, NULL, 0);
			test_errno("waitpid");
			break;
	}

	/* koñczymy proces, bez ogl±dania siê na w±tki */
	return EXIT_SUCCESS;
}
