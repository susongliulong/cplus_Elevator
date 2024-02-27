#pragma once
#include"People.h"
typedef struct {
	int size;
	People *peoples;
	int capacity;
	int increment;
}Stack;
bool initStack(Stack &stack,int capacity, int increment);
bool push(Stack &stack,People people);
People pop(Stack &stack);
People getTop(Stack stack);
bool isEmpty_Stack(Stack stack);
void printStack(Stack stack);

