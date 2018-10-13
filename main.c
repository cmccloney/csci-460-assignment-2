#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>

struct node { //node in doubly linked list
	int value;

	struct node *next;
	struct node *prev;
};

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
	middle->value = randInt();
	tail->value = randInt();
	head->prev = NULL;
	head->next = middle;
	middle->prev = head;
	middle->next = tail;
	tail->prev = middle;
	tail->next = NULL;
}

void display(){ //iterate through list
	struct node *ptr = head;
	while(ptr != NULL){
		printf("%d ",ptr->value);
		ptr = ptr->next;
	}
	printf("\n");
}

void produce(int value){ //produce a new value in list
	struct node *new = (struct node*) malloc(sizeof(struct node));
	new->value = value;
	new->next = NULL;
	new->prev = tail;
}

void consume(){ //consume oldest value in list
	struct node *newHead = head->next;
	newHead->prev = NULL;
	head = newHead;
}

int main(){
	init(); //initialize first 3 nodes
	display(); //display current list
	int length = 3; //length of list
		
	return 0;
}
