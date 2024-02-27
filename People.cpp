
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"Elevator.h"
#include"People.h"
#include"queue.h"
#include"Stack.h"
#include"Building.h"
void initPeople(People &people) {
	people.id = people.inFloor = people.outFloor =
		people.giveUpTime = people.state = people.waitTime
		= people.walkTime = people.interTime = -1;
}
/*
	创建乘客
*/
People * createPeoples(int number, int minFloor, int maxFloor, int busy) {
	People *peoples;
	peoples = (People*)malloc(sizeof(People)*number);
	for (int i = 0; i < number; i++) {
		initPeople(peoples[i]);
		peoples[i].id = i;
		peoples[i].outFloor = minFloor + rand() % (maxFloor - minFloor+1);
		peoples[i].inFloor = minFloor + rand() % (maxFloor - minFloor+1);
		/*if (i == 0) {
			peoples[0].inFloor = 1; peoples[0].outFloor = 3;
		}*/
		peoples[i].inFloor = 1;
		if (peoples[i].outFloor == peoples[i].inFloor) {
			peoples[i].outFloor = abs(peoples[i].inFloor - 3);
		}
		peoples[i].giveUpTime = abs(peoples[i].outFloor - peoples[i].inFloor) * 30;//正常人上一层楼梯30秒
	}
	//模拟不同的场景
	for (int i = 0; i < number; i++) {
		//高峰时
		if (busy == 1) {
			peoples[i].interTime = 1 + rand() % 4;//每隔1到4秒有一个人进入电梯系统
		}
		else {
			//平常情况下
			if (i % 4 == 1) {
				peoples[i].interTime = 5;
			}
			else {
				peoples[i].interTime = 60 * 2;
			}
		}
	}
	return peoples;
}
void isPeopleComeInSystem(Building &building, float &Time, Elevator &elevator) {
	if (building.amount >= Population) {
		return;
	}
	People people = building.testPeoples[building.amount];
	int interval = INT_MAX;
	if (building.amount > 0) {
		interval = building.testPeoples[building.amount - 1].interTime;
	}
	if (Time == 0 || Time - building.intervalOfPeople >= interval) {
		people.state = WaitInQueue;
		people.waitTime = Time;//后续对于所有不在电梯的乘客开始记时
		building.amount++;
		enQueue(elevator.queues[people.inFloor], people);
		building.intervalOfPeople = Time;
		//按电梯排队等候
		int inFloor = people.inFloor;
		int outFloor = people.outFloor;
		if (inFloor - outFloor > 0) {
			elevator.callDown[inFloor] = 1;
		}
		else {
			elevator.callUp[inFloor] = 1;
		}
		printf("%.1f时刻，编号为%d的人进入系统，在%d楼排队\n", Time, people.id, people.inFloor);
	}
}
void isPeopleOutSystem(Building &building, float &Time, Elevator &elevator) {
	//在外面排队的人超过最大等待时间
	People head;
	for (int i = 0; i <= MaxFloor; i++) {
		if (i == elevator.Floor)continue;
		while (getHead(elevator.queues[i], head) && Time - head.waitTime > head.giveUpTime) {
			printf("%.1f时刻，编号为%d的人在%d楼由于等待时间过长离开系统\n", Time, head.id, head.inFloor);
			//从队头删除元素
			DeQueue(elevator.queues[i], head);
		}
	}
}
void isPeopleOutElevator(Building &building, float &Time, Elevator &elevator) {
	if (!isEmpty_Stack(elevator.stacks[elevator.Floor])) {
		People top = getTop(elevator.stacks[elevator.Floor]);
		if (top.state == OutElevator && Time - top.walkTime >= 2.5) {
			printf("%.1f时刻，编号为%d的人在%d楼已经离开电梯\n", Time, top.id, elevator.Floor);
			elevator.size--;
			elevator.elevatorOpenOrCloseDoorTime = Time;
			pop(elevator.stacks[elevator.Floor]);
		}
	}
}
void isPeopleInElevator(Building &building, float &Time, Elevator &elevator) {
	People head;
	initPeople(head);
	if (getHead(elevator.queues[elevator.Floor], head) &&
		head.state == MoveInElevator && Time - head.walkTime >= 2.5) {
		printf("%.1f时刻，编号为%d的人在%d楼已经进入电梯\n", Time, head.id, elevator.Floor);
		elevator.size++;
		elevator.elevatorOpenOrCloseDoorTime = Time;
		DeQueue(elevator.queues[elevator.Floor], head);
		head.state = InElevator;
		push(elevator.stacks[head.outFloor], head);
		elevator.callCar[head.outFloor] = 1;
	}
}
void peopleAction(Building &building, float &Time, Elevator &elevator) {

	//电梯中人离开
	if (elevator.callCar[elevator.Floor] == 1 && elevator.D1 == 1 &&
		elevator.state == Idle) {
		People top;
		if (!isEmpty_Stack(elevator.stacks[elevator.Floor]) &&
			getTop(elevator.stacks[elevator.Floor]).state == InElevator) {
			top = pop(elevator.stacks[elevator.Floor]);
			top.state = OutElevator;
			top.walkTime = Time;
			push(elevator.stacks[elevator.Floor], top);
			printf("%.1f时刻，编号为%d的人在%d楼开始离开电梯\n", Time, top.id, elevator.Floor);
		}
	}
	//外面的人进入电梯
	if (isEmpty_Stack(elevator.stacks[elevator.Floor]) && elevator.state == Idle && elevator.D1 == 1 && elevator.size < elevator.capacity) {
		People people;
		initPeople(people);
		if (getHead(elevator.queues[elevator.Floor], people) && people.state == WaitInQueue) {
			printf("%.1f时刻，编号为%d的人在%d楼开始进入电梯\n", Time, people.id, elevator.Floor);
			DeQueue(elevator.queues[elevator.Floor], people);
			people.state = MoveInElevator;
			people.walkTime = Time;
			insert(elevator.queues[elevator.Floor], people);//返回队列头部
		}
	}
}