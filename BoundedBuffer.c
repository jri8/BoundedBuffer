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
#include <stdbool.h>

pthread_mutex_t m;
pthread_mutex_t f, g;
pthread_cond_t fill, empty, inc;

int num_consumer, num_producer, buffer_size, num_loops;
int numfull = 0, fillptr = 0, useptr = 0;
int filled = 0, got = 0;
int cons_sleep = 0, prod_sleep = 0;
bool cons_done = false;
bool prod_done = false;

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
	
	while(got < num_loops-cons_sleep){
		pthread_mutex_lock(&m);
		while (numfull == 0 && got < num_loops-cons_sleep){
	//		printf("\tconsumer %d is waiting\n",(int)pthread_self());
			cons_sleep = cons_sleep + 1;
			pthread_cond_wait(&fill, &m);
			cons_sleep = cons_sleep -1;
		}
		
		got = got + 1;
		int tmp = do_get();

	//	cons_done = (got >= num_loops-num_consumer);
	//	cons_done == false ? printf("cons_done = false\n") : printf("cons_done = true\n");

//			printf("%d\n",tmp);
		printf("%d %d %d\n", tmp, (int)pthread_self(),got);
	

		pthread_cond_signal(&empty);
		pthread_mutex_unlock(&m);
	}
//	int i = 0;
//	printf("%d %d\n", (int)pthread_self(), num_consumer);
//	for (i; i < num_consumer; i++)
	//	pthread_cond_signal(&fill);

}

void *producer (void *arg){
	while(filled < num_loops - prod_sleep){
		pthread_mutex_lock(&m);
		while (numfull == buffer_size && filled < num_loops - prod_sleep +1){
			prod_sleep = prod_sleep + 1;
	//		printf("producer is waiting %d\n",(int)pthread_self());
			pthread_cond_wait(&empty, &m);
			prod_sleep = prod_sleep - 1;
	//		printf("producer %d woke up\n", (int)pthread_self());
		}

	//	if (!prod_done){
				
				
		//	printf("%d filled  %d\n", (int)pthread_self(),filled);
		do_fill(filled);
		filled++;
	//	prod_done = (filled >= num_loops);
	//	prod_done == false ? printf("prod_done = false\n") : printf("prod_done = true\n");
		pthread_cond_signal(&fill);
		pthread_mutex_unlock(&m);
	//}

	}

//	int i = 0;
//	for (i; i < prod_sleep; i++)
//		pthread_cond_signal(&empty);

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
