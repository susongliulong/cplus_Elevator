#pragma once
#include"People.h"
//—≠ª∑∂”¡–
typedef struct {
	int head;
	int tail;
	int size;
	int capacity;
	People *peoples;
	int increment;
}Queue;
bool initQueue(Queue &queue, int capacity, int increment);
bool enQueue(Queue &queue, People people);
bool insert(Queue &queue, People people);
bool DeQueue(Queue &queue, People &people);
bool getHead(Queue queue, People &people);
bool isEmpty(Queue queue);
void printQueue(Queue queue);