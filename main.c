#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>

struct node { //node in doubly linked list
	int value;
	int pos;

	struct node *next;
	struct node *prev;
};

int pid;
pthread_t thread;
//initialize list
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
	head->pos = 1;
	middle->value = randInt();
	middle->pos = 2;
	tail->value = randInt();
	tail->pos = 3;
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

void producer1(){ //producer1 adds a new odd integer in the list
	int length = tail->pos - head->pos;
	while(length < 30){
		//printf("!!%d!!",length);
		display("Producer 1-before:");
		struct node *new = (struct node*) malloc(sizeof(struct node));
		int val = randInt();
		while(val % 2 != 1){
			val = randInt();
		}
		new->value = val;
		new->pos = length+2;
		new->next = NULL;
		new->prev = tail;
		tail->next = new;
		tail = new;
		length = tail->pos - head->pos;
		display("Producer 1-after:");
	}
	printf("Buffer is full\n");
	//exit(0); //end the program
}

void producer2(){ //producer2 adds a new even integer in the list
	int length = tail->pos - head->pos;
	while(length < 30){
		display("Producer 2-before:");
        	struct node *new = (struct node*) malloc(sizeof(struct node));
        	int val = randInt();
        	while(val % 2 != 0){
        	        val = randInt();
        	}
        	new->value = val;
		new->pos = length+2;
        	new->next = NULL;
        	new->prev = tail;
		tail->next = new;
		tail = new;
		length = tail->pos - head->pos;
		display("Producer 2-after:");
	}
	printf("Buffer is full\n");
	//exit(0); //end the program
}


void consumer1(){ //consume first value in list if odd
	int length = tail->pos - head->pos;
	while(length > 0){
		int val = head->value;
		while(val % 2 == 0){ //while first value is even
			sleep(1);
			val = head->value;
		}
		display("Consumer 1-before:");
		struct node *newHead = head->next;
		newHead->prev = NULL;
		head = newHead;
		display("Consumer 1-after:");
		length = tail->pos - head->pos;
	}
	display("Consumer 1-before:");
	head = NULL;
	display("Consumer 1-after:");
        //exit(0); //end the program
}

void consumer2(){ //consume first value in list if even
	int length = tail->pos - head->pos;
	while(length > 0){
                int val = head->value;
                while(val % 2 == 0){ //while first value is even
                        sleep(1);
                        val = head->value;
                }
                display("Consumer 1-before:");
                struct node *newHead = head->next;
                newHead->prev = NULL;
                head = newHead;
                display("Consumer 1-after:");
                length = tail->pos - head->pos;
        }
	display("Consumer 2-before:");
	head = NULL;
	display("Consumer 2-after:");
        //exit(0); //end the program
}


int main(){
	init(); //initialize first 3 nodes
	display("Beginning:"); //display current list
	producer1();
	consumer1();
	/*pid = fork();
	if(pid < 0){
		printf("Fork failure.\n");
	}else if(pid > 0){
		printf("Parent PID=%d\n",pid);
		consumer1(length);
	}else{
		printf("Child PID=%d.\n",pid);
		producer1(length);
		consumer1(length);
	}*/
	return 0;

}
