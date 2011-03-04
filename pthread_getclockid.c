/*
 *	Przyk³adowy program dla kursu "POSIX Threads" z wikibooks.pl
 *
 *	Temat: odczyt czasu CPU, jaki zu¿y³ w±tek
 *
 *	Autor: Wojciech Mu³a
 *	Ostatnia zmiana: 2010-03-xx
 */

#define _POSIX_C_SOURCE	200809L

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <pthread.h>
#include <unistd.h>
#include <time.h>	// sleep
#include <string.h>	// strerror

#define test_errno(msg) do{if (errno) {perror(msg); exit(EXIT_FAILURE);}} while(0)

// funkcja zwraca czas w milisekundach dla wskazanego zegara
long clock_ms(const clockid_t id_zegara);

// funkcja zwraca czas CPU dla w±tku (w milisekundach)
long get_thread_time(pthread_t id);

/* parametry w±tku */
typedef struct {
	int id;	// numer
	int n;	// liczba iteracji
} parametry;

// funkcja w±tku
void* watek(void* _arg) {
	parametry* arg = (parametry*)_arg;
	int i;
	printf("w±tek #%d uruchomiony, dwa razy wykona %d pustych pêtli\n",
		(int)arg->id,
		(int)arg->n
	);

	for (i=0; i < arg->n; i++)
		/* zu¿ycie czasu procesora */;

	sleep(2);
	
	for (i=0; i < arg->n; i++)
		/* zu¿ycie czasu procesora */;

	/* podsumowanie pracy */
	printf("w±tek #%d zakoñczony, zu¿y³ %ldms czasu procesora\n",
		(int)arg->id,
		clock_ms(CLOCK_THREAD_CPUTIME_ID)
	);
	return NULL;
}
//------------------------------------------------------------------------

#define N	10	// liczba w±tków

int main() {
	pthread_t id[N];
	parametry param[N];
	int i;

	srand(time(NULL));

	printf("pocz±tek programu, uruchomianie zostanie %d w±tków\n", N);

	/* utworzenie w±tku */
	for (i=0; i < N; i++) {
		param[i].id = i;
		param[i].n = rand() % 100000000 + 1;

		errno = pthread_create(&id[i], NULL, watek, &param[i]);
		test_errno("pthread_create");
	}

	/* stan na mniej wiêcej pó³metku */
	sleep(1);
	puts("po oko³o sekundzie w±tki zu¿y³y:");
	for (i=0; i < N; i++)
		printf("* #%d: %ldms\n", i, get_thread_time(id[i]));

	/* oczekiwanie na zakoñczenie w±tków */
	for (i=0; i < N; i++) {
		errno = pthread_join(id[i], NULL);
		test_errno("pthread_join");
	}

	/* jeszcze podsumowanie */
	puts("");
	printf("g³ówny w±tek zu¿y³ %ldms czasu procesora\n", clock_ms(CLOCK_THREAD_CPUTIME_ID));
	printf("proces zu¿y³ %ldms czasu procesora\n", clock_ms(CLOCK_PROCESS_CPUTIME_ID));

	return EXIT_SUCCESS;
}
//------------------------------------------------------------------------

long get_thread_time(pthread_t id) {
	clockid_t id_zegara;

	errno = pthread_getcpuclockid(id, &id_zegara);
	test_errno("pthread_getcpuclockid");

	return clock_ms(id_zegara);
}
//------------------------------------------------------------------------

long clock_ms(const clockid_t id_zegara) {
	struct timespec czas;
	if (clock_gettime(id_zegara, &czas)) {
		perror("clock_gettime");
		exit(EXIT_FAILURE);
	}
	else
		return (czas.tv_sec * 1000) +
		       (czas.tv_nsec/1000000);
}
//------------------------------------------------------------------------
