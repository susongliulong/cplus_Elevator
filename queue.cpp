#include"queue.h"
#include<stdlib.h>
#include<stdio.h>
bool initQueue(Queue &queue, int capacity, int increment) {
	People *peoples = (People*)malloc(sizeof(People)*capacity);
	if (peoples == NULL)return 0;
	queue.capacity = capacity;
	queue.increment = increment;
	queue.head = queue.tail = 0;
	queue.peoples = peoples;
	queue.size = 0;
	return 1;
}
bool enQueue(Queue &queue, People people) {
	if (queue.size >= queue.capacity) {
		//À©ÈÝ
		People* peoples = (People*)malloc(sizeof(People)*(queue.capacity + queue.increment));
		if (peoples == NULL)return 0;
		for (int i = 0; i < queue.size; i++) {
			peoples[i] = queue.peoples[(queue.head + i) % queue.capacity];
		}
		queue.peoples = peoples;
		queue.head = 0;
		queue.tail = queue.size;
		queue.capacity += queue.increment;
	}
	queue.peoples[queue.tail] = people;
	queue.tail = (queue.tail + 1) % queue.capacity;
	queue.size++;
	return 1;
}

bool insert(Queue &queue, People people) {
	if (queue.size >= queue.capacity) {
		//À©ÈÝ
		People* peoples = (People*)malloc(sizeof(People)*(queue.capacity + queue.increment));
		if (peoples == NULL)return 0;
		for (int i = 0; i < queue.size; i++) {
			peoples[i] = queue.peoples[(queue.head + i) % queue.capacity];
		}
		queue.peoples = peoples;
		queue.head = 0;
		queue.tail = queue.size;
		queue.capacity += queue.increment;
	}
	queue.head = (queue.head - 1 + queue.capacity) % queue.capacity;
	queue.peoples[queue.head] = people;
	queue.size++;
	return 1;
}

bool DeQueue(Queue &queue, People &people) {
	if (queue.size == 0)return 0;
	people = queue.peoples[queue.head];
	queue.head = (queue.head+1) % queue.capacity;
	queue.size--;
	return 1;
}
bool getHead(Queue queue, People &people) {
	if (queue.size == 0)return 0;
	people = queue.peoples[queue.head];
	return 1;
}
bool isEmpty(Queue queue) {
	return queue.size == 0 ? 1 : 0;
}
void printQueue(Queue queue) {
	for (int i = 0; i < queue.size; i++) {
		People people = queue.peoples[(queue.head + i) % queue.capacity];
		printf("id:%d from %d to %d ", people.id,people.inFloor,people.outFloor);
		printf("%d ", people.interTime);
	}
	printf("\n");
}
//int main() {
//	People peoples[20];
//	Queue queue;
//	initQueue(queue, 6, 4);
//	for (int i = 0; i < 20; i++) {
//		peoples[i].interTime = i;
//		insert(queue, peoples[i]);
//		printQueue(queue);
//	}
//	People people;
//	people.interTime = 0;
//	for (int i = 1; i < 11; i++) {
//		DeQueue(queue, people);
//		printQueue(queue);
//	}
//	return 0;
//}