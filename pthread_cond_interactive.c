#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

pthread_mutex_t	mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t	cond = PTHREAD_COND_INITIALIZER;
int value = 0;

void* thread(void* arg) {
	int  id  = (int)arg;
	char run = 1;
	
	printf("thread #%d started\n", id);
	
	while (run) {
		pthread_mutex_lock(&mut);
		pthread_cond_wait(&cond, &mut);
		printf("thread #%d woke up, value = %d\n", id, value);
		if (value < 0)
			run = 0;
		
		pthread_mutex_unlock(&mut);
	}

	printf("thread #%d finished\n", id);
}

int main() {
	char command[256];
	char c;
	pthread_t id;
	int i;
	int n = 5;

	for (i=0; i < n; i++)
		if (pthread_create(&id, NULL, thread, (void*)(i+1)))
			perror("thread create");

	while (value >= 0) {
		gets(command);
		if (strlen(command) == 0)
			continue;

		switch (command[0]) {
			case 's':
			case 'S':
				value = strtol(command+1, NULL, 0);
				printf("values = %d\n", value);
				pthread_cond_signal(&cond);
				break;

			case 'b':
			case 'B':
				value = strtol(command+1, NULL, 0);
				printf("values = %d\n", value);
				pthread_cond_broadcast(&cond);
				break;

			case 'q':
			case 'Q':
				value = -1;
				pthread_cond_broadcast(&cond);
				break;
		}
	}
}
