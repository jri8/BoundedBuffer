//need something to tell consumers that theres nothing else to consume
//need to tell producers theres nothing else to produce
//join after these complete
//makes sure the number of threads produced are the number of threads that you join
//get it working wiht a buffer size of 1
//possible problems: cause a deadlock, initialize wrong variables first

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t m;
pthread_mutex_t f, g;
pthread_cond_t fill, empty, inc;

int num_consumer, num_producer, buffer_size, num_loops;
int numfull = 0, fillptr = 0, useptr = 0;
int filled = 0, got = 0;

int *buffer;


void do_fill(int value){
	buffer[fillptr] = value;
	fillptr = (fillptr + 1) % buffer_size;
	numfull++;
}

int do_get() {
	int tmp = buffer[useptr];
	useptr = (useptr + 1) % buffer_size;
	numfull--;
	return tmp;
}

void *consumer (void *arg){

	while(got < num_loops){
		pthread_mutex_lock(&m);
		while (numfull == 0 && got < num_loops){
	//		printf("consumer %d got = %d\t",(int)pthread_self(),got);
			printf("\tconsumer %d is waiting\n",(int)pthread_self());
			pthread_cond_wait(&fill, &m);
	//		printf("consumer %d woke up got = %d\n",(int)pthread_self(), got);
		}
		int tmp = do_get();
		got++;

		pthread_cond_signal(&empty);
		pthread_mutex_unlock(&m);
		printf("%d %d %d\n", tmp, (int)pthread_self(),got);

	}

}

void *producer (void *arg){
	int i;
	for(i = 0; i < num_loops; i++){
	//	printf("producer %d lock m\n", (int)pthread_self());
		pthread_mutex_lock(&m);
		while (numfull == buffer_size){
	//		printf("producer %d is waiting\n", (int)pthread_self());
			pthread_cond_wait(&empty, &m);

	//		printf("producer %d woke up\n", (int)pthread_self());
		}


		do_fill(i);
	//	pthread_mutex_lock(&f);
//		filled++;
//		pthread_mutex_unlock(&f);

	//	printf("producer %d singal fill\n", (int)pthread_self());
		pthread_cond_signal(&fill);
	//	printf("producer %d unlock m\n", (int)pthread_self());
		pthread_mutex_unlock(&m);
	}

}


int main (int argc, char* argv[]){

	if (argc != 5){
		printf("Incorrect number of command line arguments\n");
		exit(0);
	}


	num_consumer = atoi(argv[1]);
	num_producer = atoi(argv[2]);
	buffer_size = atoi(argv[3]);
	num_loops = atoi(argv[4]);

	buffer = (int *) malloc(buffer_size * sizeof(int));
	
	printf("consumers: %d\n producers: %d\n buffer size: %d\n loops: %d\n", num_consumer, num_producer, buffer_size, num_loops);

	pthread_t pid[10], cid[10];
	int i;

	for (i = 0; i < num_consumer; i++){
		pthread_create(&cid[i], NULL, consumer, NULL);
		printf("consumer %d = %d\n", i, (int) cid[i]);
	}
	for (i = 0; i < num_producer; i ++){
		pthread_create(&pid[i], NULL, producer, NULL);
		printf("producer %d = %d\n",i, (int)pid[i]);
	}

	for (i = 0; i < num_consumer; i++)
		pthread_join(cid[i], NULL);
	printf("consumers joined\n");

	for (i = 0; i < num_producer; i++)
		pthread_join(pid[i], NULL);
	printf("producers joined\n");
	
	printf("parent complete\n");
	return 0;
}
