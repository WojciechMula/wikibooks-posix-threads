/*
 *	Przyk³adowy program dla kursu "POSIX Threads" z wikibooks.pl
 *
 *	Temat: atrybuty w±tku - wypisanie domy¶lnych warto¶ci
 *
 *	Autor: Wojciech Mu³a
 *	Ostatnia zmiana: 2010-03-xx
 */

#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <limits.h>	// PTHREAD_STACK_MIN
#include <errno.h>

#define test_errno(msg) do{if (errno) {perror(msg); exit(EXIT_FAILURE);}} while(0)

void wyswietl_atrybuty(const pthread_attr_t* attr) {
	int x;
	size_t rozmiar;
	void* addr;
	struct sched_param param;

	puts("atrybuty w±tku");

	// rodzaj w±tku
	printf("* rodzaj: ");
	errno = pthread_attr_getdetachstate(attr, &x);
	test_errno("pthread_attr_getdetachstate");

	switch (x) {
		case PTHREAD_CREATE_JOINABLE:
			puts("joinable");
			break;
		case PTHREAD_CREATE_DETACHED:
			puts("detached");
			break;
		default:
			puts("???");
	}

	// adres i rozmiar stosu
	errno = pthread_attr_getstackaddr(attr, &addr);
	test_errno("pthread_attr_getstackaddr");
	printf("* adres stosu: %p\n", addr);

	errno = pthread_attr_getstacksize(attr, &rozmiar);
	test_errno("pthread_attr_getstacksize");
	printf("* rozmiar stosu: %d (minimalny %d)\n", rozmiar, PTHREAD_STACK_MIN);

	// rozmiar obszaru zabezpieczaj±cego stosu
	errno = pthread_attr_getguardsize(attr, &rozmiar);
	test_errno("pthread_attr_getguardsize");
	printf("* rozmiar obszaru zabezpieczaj±cego: %d\n", rozmiar);

	// szeregowanie
	errno = pthread_attr_getinheritsched(attr, &x);
	test_errno("pthread_attr_getinheritsched");
	switch (x) {
		case PTHREAD_INHERIT_SCHED:
			puts("* parametry szeregowania dziedziczone");
			break;

		case PTHREAD_EXPLICIT_SCHED:
			puts("* parametry podawane bezpo¶rednio");
			
			//
			printf("   - algorytm szeregowania: ");
			errno = pthread_attr_getschedpolicy(attr, &x);
			test_errno("pthread_attr_getschedpolicy");
			switch (x) {
				case SCHED_OTHER:
					puts("SCHED_OTHER");
					break;
				case SCHED_RR:
					puts("SCHED_RR");
					break;
				case SCHED_FIFO:
					puts("SCHED_FIFO");
					break;
				default:
					puts("???");
			}

			// 
			errno = pthread_attr_getschedparam(attr, &param);
			test_errno("pthread_attr_getschedparam");
			printf("   - priorytet: %d\n", param.sched_priority);
			break;

		default:
			puts("???");
	}

	// zakres szeregowania
	errno = pthread_attr_getscope(attr, &x);
	test_errno("pthread_attr_getscope");
	
	printf("* zakres szeregowania: ");
	switch (x) {
		case PTHREAD_SCOPE_PROCESS:
			puts("proces");
			break;
		case PTHREAD_SCOPE_SYSTEM:
			puts("system");
			break;
		default:
			puts("???");
	}
}
//------------------------------------------------------------------------

int main() {
	pthread_attr_t attr;

	errno = pthread_attr_init(&attr);
	test_errno("pthread_attr_init");
		
	wyswietl_atrybuty(&attr);
	return EXIT_SUCCESS;
}
//------------------------------------------------------------------------
