#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>

struct node { //node in doubly linked list
	int value;

	struct node *next;
	struct node *prev;
};

int p1,p2,c1,c2;
int N = 3;
pthread_t thread1, thread2, thread3, thread4;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
struct node *head = NULL;
struct node *tail = NULL;

int randInt(){
	int r = rand() % 51; //random int between 0 and 50
	return r;
}

void init(){ //initialize doubly linked list with 3 nodes
	head = (struct node*) malloc(sizeof(struct node));
	tail = (struct node*) malloc(sizeof(struct node));
	struct node *middle = (struct node*) malloc(sizeof(struct node));
	head->value = randInt();
	//head->pos = 1;
	middle->value = randInt();
	//middle->pos = 2;
	tail->value = randInt();
	//tail->pos = 3;
	head->prev = NULL;
	head->next = middle;
	middle->prev = head;
	middle->next = tail;
	tail->prev = middle;
	tail->next = NULL;
}

void display(char name[]){ //iterate through list
	struct node *ptr = head;
	printf("%s",name);
	while(ptr != NULL){
		printf("%d ",ptr->value);
		ptr = ptr->next;
	}
	printf("\n");
}

void *producer1(){ //producer1 adds a new odd integer in the list
	//int length = tail->pos - head->pos;
	while(N < 30){
		//printf("!!%d!!",length);
		pthread_mutex_trylock(&mutex1);
		display("Producer 1-before:");
		struct node *new = (struct node*) malloc(sizeof(struct node));
		int val = randInt();
		while(val % 2 != 1){
			val = randInt();
		}
		new->value = val;
		//new->pos = length+2;
		new->next = NULL;
		new->prev = tail;
		tail->next = new;
		tail = new;
		//length = tail->pos - head->pos;
		display("Producer 1-after:");
		N++;
		pthread_mutex_unlock(&mutex1);
		sleep(1);
	}
	printf("Buffer is full\n");
	sleep(5);
	producer1();
	//exit(0); //end the program
}

void *producer2(){ //producer2 adds a new even integer in the list
	//int length = tail->pos - head->pos;
	while(N < 30){
		pthread_mutex_trylock(&mutex1);
		display("Producer 2-before:");
        	struct node *new = (struct node*) malloc(sizeof(struct node));
        	int val = randInt();
        	while(val % 2 != 0){
        	        val = randInt();
        	}
        	new->value = val;
		//new->pos = length+2;
        	new->next = NULL;
        	new->prev = tail;
		tail->next = new;
		tail = new;
		//length = tail->pos - head->pos;
		display("Producer 2-after:");
		N++;
		pthread_mutex_unlock(&mutex1);
		sleep(1);
	}
	printf("Buffer is full\n");
	sleep(5);
	producer2();
	//exit(0); //end the program
}


void *consumer1(){ //consume first value in list if odd
	//int length = tail->pos - head->pos;
	int val = head->value;
	while(N > 0){
		if(val % 2 == 1){ //if first value is odd
			sleep(1);
			pthread_mutex_trylock(&mutex1);
			display("Consumer 1-before:");
			struct node *newHead = head->next;
			newHead->prev = NULL;
			head = newHead;
			display("Consumer 1-after:");
			//length = tail->pos - head->pos;
			N--;
			pthread_mutex_unlock(&mutex1);
		}
		val = head->value;
	}
	/*if(length <= 0){
		pthread_mutex_trylock(&mutex1);
		display("Consumer 1-before:");
		head = NULL;
		display("Consumer 1-after:");
		pthread_mutex_unlock(&mutex1);
	}*/
	printf("Buffer is empty\n");
	sleep(5);
	consumer1();
        //exit(0); //end the program
}

void *consumer2(){ //consume first value in list if even
	//int length = tail->pos - head->pos;
	int val = head->value;
	while(N > 0){
                if(val % 2 == 0){ //if first value is even
			sleep(1);
			pthread_mutex_trylock(&mutex1);
                	display("Consumer 1-before:");
                	struct node *newHead = head->next;
                	newHead->prev = NULL;
                	head = newHead;
                	display("Consumer 1-after:");
                	//length = tail->pos - head->pos;
			N--;
			pthread_mutex_unlock(&mutex1);
		}
		val = head->value;
        }
	/*if(length <= 0){
		pthread_mutex_trylock(&mutex1);
		display("Consumer 2-before:");
		head = NULL;
		display("Consumer 2-after:");
		pthread_mutex_unlock(&mutex1);
	}*/
	printf("Buffer is empty\n");
	sleep(5);
	consumer2();
        //exit(0); //end the program
}


int main(){
	init(); //initialize first 3 nodes
	display("Beginning:"); //display current list
	//producer1();
	//consumer1();
	//pid = fork();
	if( (p1 = pthread_create(&thread1,NULL,&producer1,NULL)) ){
		printf("Thread creation failed: %d\n", p1);
	}
	if( (p2 = pthread_create(&thread2,NULL,&producer2,NULL)) ){
		printf("Thread creation failed: %d\n", p2);
	}
	if( (c1 = pthread_create(&thread3,NULL,&consumer1,NULL)) ){
		printf("Thread creation failed: %d\n", c1);
	}
	if( (c2 = pthread_create(&thread4,NULL,&consumer2,NULL)) ){
		printf("Thread creation failed: %d\n", c2);
	}
	/*if(pid < 0){
		printf("Fork failure.\n");
	}else if(pid > 0){
		printf("Parent PID=%d\n",pid);
		consumer1(length);
	}else{
		printf("Child PID=%d.\n",pid);
		producer1(length);
		consumer1(length);
	}*/
	pthread_join(thread1,NULL);
	pthread_join(thread3,NULL);
	pthread_join(thread2,NULL);
	pthread_join(thread4,NULL);
	return 0;

}
