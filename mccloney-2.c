#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>

struct node { //node in doubly linked list
	int value;
	struct node *next;
	struct node *prev;
};

int p1,p2,c1,c2; //thread for each producer and consumer
int t; //thread for timer
int N = 3; //length of linked list
pthread_t thread1, thread2, thread3, thread4, thread5;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
struct node *head = NULL;
struct node *tail = NULL;

int randInt(){ //return random integer
	int r = rand() % 51; //random int between 0 and 50
	return r;
}

void init(){ //initialize doubly linked list with 3 nodes
	head = (struct node*) malloc(sizeof(struct node));
	tail = (struct node*) malloc(sizeof(struct node));
	struct node *middle = (struct node*) malloc(sizeof(struct node));
	head->value = randInt();
	middle->value = randInt();
	tail->value = randInt();
	head->prev = NULL;
	head->next = middle;
	middle->prev = head;
	middle->next = tail;
	tail->prev = middle;
	tail->next = NULL;
}

void display(char name[]){ //iterate through list
	struct node *ptr = head;
	printf("%s ",name);
	while(ptr != NULL){
		printf("%d ",ptr->value); //print node's random integer
		ptr = ptr->next;
	}
	printf("\n");
}

void *producer1(){ //producer1 adds a new odd integer in the list
	while(N < 30){
		sleep(1); //slow down
		pthread_mutex_trylock(&mutex1); //attempt to lock thread
		display("Producer 1-before:");
		struct node *new = (struct node*) malloc(sizeof(struct node)); //create new node
		int val = randInt();
		while(val % 2 != 1){ //make sure random integer is odd
			val = randInt();
		}
		new->value = val;
		new->next = NULL;
		if(N > 0){
			new->prev = tail;
			tail->next = new;
			tail = new;
		}else{
			head = new;
			tail = new;
		}
		display("	    after:");
		N++;
		pthread_mutex_unlock(&mutex1); //unlock thread
	}
	printf("Buffer is full\n"); //print message
	sleep(5); //wait
	producer1(); //repeat
}

void *producer2(){ //producer2 adds a new even integer in the list
	while(N < 30){
		sleep(1); //slow down
		pthread_mutex_trylock(&mutex1); //attempt to lock thread
		display("Producer 2-before:");
        	struct node *new = (struct node*) malloc(sizeof(struct node)); //create new node
        	int val = randInt();
        	while(val % 2 != 0){ //make sure random integer is even
        	        val = randInt();
        	}
        	new->value = val;
        	new->next = NULL;
		if(N > 0){
        		new->prev = tail;
			tail->next = new;
			tail = new;
		}else{
			head = new;
			tail = new;
		}
		display("	    after:");
		N++;
		pthread_mutex_unlock(&mutex1); //unlock thread
	}
	printf("Buffer is full\n"); //print message
	sleep(5); //wait
	producer2(); //repeat
}


void *consumer1(){ //consume first value in list if odd
	int val = head->value;
	while(N > 0){
		val = head->value;
		sleep(1); //slow down
		if(val % 2 == 1){ //if first value is odd
			pthread_mutex_trylock(&mutex1); //attempt to lock thread
			display("Consumer 1-before:");
			if(N > 1){
				struct node *newHead = head->next;
				newHead->prev = NULL;
				head = newHead;
			}else{
				head = NULL;
			}
			display("	    after:");
			N--;
			pthread_mutex_unlock(&mutex1); //unlock thread
		}
	}
	printf("Buffer is empty\n"); //print message
	sleep(5); //wait
	consumer1(); //repeat
}

void *consumer2(){ //consume first value in list if even
	int val = head->value;
	while(N > 0){
		val = head->value;
		sleep(1); //slow down
                if(val % 2 == 0){ //if first value is even
			pthread_mutex_trylock(&mutex1); //attempt to lock thread
                	display("Consumer 2-before:");
			if(N > 1){
                		struct node *newHead = head->next;
                		newHead->prev = NULL;
                		head = newHead;
			}else{
				head = NULL;
			}
                	display("	    after:");
			N--;
			pthread_mutex_unlock(&mutex1); //unlock thread
		}
        }
	printf("Buffer is empty\n"); //print message
	sleep(5); //wait
	consumer2(); //repeat
}

void *timer(){
	sleep(30); //wait 30 seconds
	display("Final:");
	printf("Program ended manually after 30 seconds\n");
	exit(0); //exit program
}


int main(){
	init(); //initialize first 3 nodes
	display("Beginning:"); //display current list
	//create a thread for each producer and consumer
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
	if( (t = pthread_create(&thread5,NULL,&timer,NULL)) ){ //timer ends program after 30 seconds
		printf("Thread creation failed: %d\n", t);
	}
	pthread_join(thread1,NULL); //join all four threads and timer
	pthread_join(thread2,NULL);
	pthread_join(thread3,NULL);
	pthread_join(thread4,NULL);
	pthread_join(thread5,NULL);
	return 0;

}
