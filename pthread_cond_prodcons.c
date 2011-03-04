#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

pthread_mutex_t	blokada;
pthread_cond_t	element_dostepny;

#define ROZMIAR_STOSU	1000
int stos[ROZMIAR_STOSU];
int wierzcholek_stosu = -1;
int koniec_pracy = 0;	// flaga, oznaczaj¹ca koniec produkcji

int stos_pusty();
void na_stos(int x);
int  ze_stosu();

void* producent(void* arg) {
	int i, n, x;

	n = (int)arg;	// liczb elementów do wyprodukowania
	printf("Producent wyprodukuje %d elementow\n", (int)arg);
	for (i=0; i < n; i++) {
		pthread_mutex_lock(&blokada);
			// dodanie liczby na stos
			x = rand() % n; 
			printf("Producent produkuje liczbe %d/%d o wartosci %d\n", i, n, x);
			na_stos(x);

			// sygnalizacja oczekuj¹cym w¹tkom, ¿e pojawi³a siê nowa wartoœæ
			pthread_cond_signal(&element_dostepny);
		pthread_mutex_unlock(&blokada);

//		usleep(10 * 1000);
	}

	printf("Producent zakoñczy³ pracê\n");
	// po zakoñczeniu, ustawiamy flagê
	pthread_mutex_lock(&blokada);
		koniec_pracy = 1;
		pthread_cond_broadcast(&element_dostepny);
	pthread_mutex_unlock(&blokada);

	return NULL;
}

void* konsument(void* arg) {
	const int id = (int)arg;
	int element;
	while (1) {
		pthread_mutex_lock(&blokada);
		do {
			if (stos_pusty()) {
				if (koniec_pracy) {
					printf("Konsument #%d konczy prace\n", id);
					pthread_mutex_unlock(&blokada);
					return NULL;
				}
			}
			else {
				element = ze_stosu();
				printf("Konsument #%d pobral liczbe %d\n", id, element);
				break;
			}

			pthread_cond_wait(&element_dostepny, &blokada);
		}
		while (1);
		pthread_mutex_unlock(&blokada);
		usleep(50*1000);
	}
}

#define N 10

int main() {
	pthread_t id[N+1];
	int i;

	/* utworzenie w¹tków konsumenta */
	for (i=0; i < N; i++) {
		errno = pthread_create(&id[i], NULL, konsument, (void*)i);
		if (errno) {
			perror("Nie powiod³o siê pthread_create");
			return EXIT_FAILURE;
		}
	}

	/* utworzenie jednego w¹tku producenta */
	errno = pthread_create(&id[N], NULL, producent, (void*)50);
	if (errno) {
		perror("Nie powiod³o siê pthread_create");
		return EXIT_FAILURE;
	}

	/* oczekiwanie na jego zakoñczenie */
	for (i=0; i < N+1; i++)
		pthread_join(id[i], NULL);

	printf("wskaznik stosu=%d\n", wierzcholek_stosu);

	return EXIT_SUCCESS;
}

int stos_pusty() {
	return (wierzcholek_stosu == -1);
}

void na_stos(int x) {
	stos[++wierzcholek_stosu] = x;
}

int ze_stosu() {
	return stos[wierzcholek_stosu--];
}
