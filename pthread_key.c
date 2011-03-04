/*
 *	Przyk³adowy program dla kursu "POSIX Threads" z wikibooks.pl
 *
 *	Temat: prywatne dane w±tków
 *
 *	Autor: Wojciech Mu³a
 *	Ostatnia zmiana: 2010-03-xx
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define test_errno(info) do {if (errno) {perror(info); exit(EXIT_FAILURE);}} while(0)

pthread_key_t	klucz;

/* funkcja wypsuje wiersz, poprzedz±j±c go prefiksem przypisanym do w±tku */
void wyswietl(const char* napis) {
	char* prefiks = (char*)pthread_getspecific(klucz);
	if (prefiks == NULL)
		/* nale¿y zabezpieczyæ siê przed sytuacj±, gdy wywo³uj±cy
		   w±tek nie przyporz±dkowa³ nic do klucza */
		puts(napis);
	else
		printf("%s: %s\n", prefiks, napis);
}
//---------------------------------------------------------------------------

/* destruktor klucza */
void destruktor(void* napis) {
	printf("wywo³ano destruktor, adres pamiêci do zwolnienia: %p ('%s')\n",
		napis,
		(char*)napis
	);
	free(napis);
}
//---------------------------------------------------------------------------

void* watek(void* napis) {
	/* ustawienie prefiksu w lokalnych danych w±tku */
	int status = pthread_setspecific(klucz, napis);
	if (status)
		fprintf(stderr, "pthread_setspecific: %s\n", strerror(status));
	else
		printf("adres napisu: %p ('%s')\n", napis, (char*)napis);

	wyswietl("Witaj w równoleg³ym ¶wiecie!");
	sleep(1);
	wyswietl("W±tek wykonuje pracê");
	sleep(1);
	wyswietl("W±tek zakoñczony");
	return NULL;
}
//---------------------------------------------------------------------------

char* strdup(const char*);

#define N 3

int main() {
	pthread_t id[N];
	int i;
	char* prefiks[3] = {"***", "!!!", "###"}; // prefiksy dla komunikatów z w±tków

	/* utworzenie klucza */
	errno = pthread_key_create(&klucz, destruktor);
	test_errno("pthread_key_create");

	/* utworzenie w±tków */
	for (i=0; i < N; i++) {
		errno = pthread_create(&id[i], NULL, watek, (void*)strdup(prefiks[i % 3]));
		test_errno("pthread_create");
	}
	
	/* oczekiwanie na ich zakoñczenie */
	for (i=0; i < N; i++)
		pthread_join(id[i], NULL);

	/* usuniêcie klucza */
	errno = pthread_key_delete(klucz);
	test_errno("pthread_key_delete");

	return EXIT_SUCCESS;
}
//---------------------------------------------------------------------------

char* strdup(const char* s) {
	char *d = NULL;
	if (s) {
		d = (char*)malloc(strlen(s)+1);
		if (d) 
			strcpy(d, s);
	}
	return d;
}
//---------------------------------------------------------------------------
