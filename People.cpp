
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
	�����˿�
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
		peoples[i].giveUpTime = abs(peoples[i].outFloor - peoples[i].inFloor) * 30;//��������һ��¥��30��
	}
	//ģ�ⲻͬ�ĳ���
	for (int i = 0; i < number; i++) {
		//�߷�ʱ
		if (busy == 1) {
			peoples[i].interTime = 1 + rand() % 4;//ÿ��1��4����һ���˽������ϵͳ
		}
		else {
			//ƽ�������
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
		people.waitTime = Time;//�����������в��ڵ��ݵĳ˿Ϳ�ʼ��ʱ
		building.amount++;
		enQueue(elevator.queues[people.inFloor], people);
		building.intervalOfPeople = Time;
		//�������ŶӵȺ�
		int inFloor = people.inFloor;
		int outFloor = people.outFloor;
		if (inFloor - outFloor > 0) {
			elevator.callDown[inFloor] = 1;
		}
		else {
			elevator.callUp[inFloor] = 1;
		}
		printf("%.1fʱ�̣����Ϊ%d���˽���ϵͳ����%d¥�Ŷ�\n", Time, people.id, people.inFloor);
	}
}
void isPeopleOutSystem(Building &building, float &Time, Elevator &elevator) {
	//�������Ŷӵ��˳������ȴ�ʱ��
	People head;
	for (int i = 0; i <= MaxFloor; i++) {
		if (i == elevator.Floor)continue;
		while (getHead(elevator.queues[i], head) && Time - head.waitTime > head.giveUpTime) {
			printf("%.1fʱ�̣����Ϊ%d������%d¥���ڵȴ�ʱ������뿪ϵͳ\n", Time, head.id, head.inFloor);
			//�Ӷ�ͷɾ��Ԫ��
			DeQueue(elevator.queues[i], head);
		}
	}
}
void isPeopleOutElevator(Building &building, float &Time, Elevator &elevator) {
	if (!isEmpty_Stack(elevator.stacks[elevator.Floor])) {
		People top = getTop(elevator.stacks[elevator.Floor]);
		if (top.state == OutElevator && Time - top.walkTime >= 2.5) {
			printf("%.1fʱ�̣����Ϊ%d������%d¥�Ѿ��뿪����\n", Time, top.id, elevator.Floor);
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
		printf("%.1fʱ�̣����Ϊ%d������%d¥�Ѿ��������\n", Time, head.id, elevator.Floor);
		elevator.size++;
		elevator.elevatorOpenOrCloseDoorTime = Time;
		DeQueue(elevator.queues[elevator.Floor], head);
		head.state = InElevator;
		push(elevator.stacks[head.outFloor], head);
		elevator.callCar[head.outFloor] = 1;
	}
}
void peopleAction(Building &building, float &Time, Elevator &elevator) {

	//���������뿪
	if (elevator.callCar[elevator.Floor] == 1 && elevator.D1 == 1 &&
		elevator.state == Idle) {
		People top;
		if (!isEmpty_Stack(elevator.stacks[elevator.Floor]) &&
			getTop(elevator.stacks[elevator.Floor]).state == InElevator) {
			top = pop(elevator.stacks[elevator.Floor]);
			top.state = OutElevator;
			top.walkTime = Time;
			push(elevator.stacks[elevator.Floor], top);
			printf("%.1fʱ�̣����Ϊ%d������%d¥��ʼ�뿪����\n", Time, top.id, elevator.Floor);
		}
	}
	//������˽������
	if (isEmpty_Stack(elevator.stacks[elevator.Floor]) && elevator.state == Idle && elevator.D1 == 1 && elevator.size < elevator.capacity) {
		People people;
		initPeople(people);
		if (getHead(elevator.queues[elevator.Floor], people) && people.state == WaitInQueue) {
			printf("%.1fʱ�̣����Ϊ%d������%d¥��ʼ�������\n", Time, people.id, elevator.Floor);
			DeQueue(elevator.queues[elevator.Floor], people);
			people.state = MoveInElevator;
			people.walkTime = Time;
			insert(elevator.queues[elevator.Floor], people);//���ض���ͷ��
		}
	}
}