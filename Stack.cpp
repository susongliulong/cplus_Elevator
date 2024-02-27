#include"Stack.h"
#include<stdlib.h>
#include<stdio.h>
bool initStack(Stack &stack,int capacity, int increment) {
	People *peoples = (People*)malloc(sizeof(People)*capacity);
	if (peoples == NULL) {
		return 0;
	}
	stack.capacity = capacity;
	stack.increment = increment;
	stack.peoples = peoples;
	stack.size = 0;
	return 1;
}
bool push(Stack &stack, People people) {
	if (stack.size >= stack.capacity) {
		//resize
		stack.peoples = (People*)realloc(stack.peoples, sizeof(People)*(stack.increment + stack.capacity));
		if (stack.peoples == NULL){
			return 0;
		}
		stack.capacity += stack.increment;
	}
	stack.peoples[stack.size++] = people;
	return 1;
}
People pop(Stack &stack) {
	People people = stack.peoples[stack.size - 1];
	stack.size--;
	return people;
}
People getTop(Stack stack) {
	return stack.peoples[stack.size - 1];
}
bool isEmpty_Stack(Stack stack) {
	return stack.size == 0 ? 1 : 0;
}
void printStack(Stack stack) {
	while (stack.size!=0)
	{
		People people = pop(stack);
		printf("id:%d from %d to %d ", people.id, people.inFloor, people.outFloor);
	}
	printf("\n");
}