#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"Elevator.h"
#include"People.h"
#include"queue.h"
#include"Stack.h"
#include"Building.h"
int main() {
	//�˵���Ϊ
	People * createPeoples(int number, int minFloor, int maxFloor, int busy);//���ɲ��Գ˿���Ϣ
	void isPeopleComeInSystem(Building &building, float &Time, Elevator &elevator);//Ӧ״̬�Ƿ����ϵͳ�����Ŷ�
	void isPeopleOutSystem(Building &building, float &Time, Elevator &elevator);//��Ӧ״̬�Ƿ��뿪ϵͳ
	void isPeopleOutElevator(Building &building, float &Time, Elevator &elevator);//��Ӧ״̬�Ƿ��뿪����
	void isPeopleInElevator(Building &building, float &Time, Elevator &elevator);//��Ӧ״̬�Ƿ�������
	void peopleAction(Building &building, float &Time, Elevator &elevator);//��Ӧ������ʼ������ݻ����뿪����
	
	//������Ϊ
	void initElevator(Elevator &elevator);//��ʼ�����ݲ���
	int controller(Building &building, float &Time, Elevator &elevator);//���ݽ�����ض�����״̬ת���ж�
	bool iscallUpOrDowmEmpty(Building &building, float &Time, Elevator elevator, int direction);//�����������жϵ�ǰ¥�����ϣ����£��Ƿ�Ϊ��
	void elevatorAction(Building &building, float &Time, Elevator &elevator);//���ݶ����������ţ���ʼ�ƶ�
	void ElevatorRun(Building &building, float &Time, Elevator &elevator, int direction);//��Ӧ�����ƶ�������״̬�ı䣨���٣����٣����٣�
	float endTime;
	int testChoice;
	while (1) {
		printf("��������Խ�����ʱ��,����Ϊ��λ,��ȷ��С�����һλ\n");
		scanf("%f", &endTime);
		printf("��������Ե�ģʽ:");
		printf("1����߷�ʱ���,0������ͨʱ���\n");
		scanf("%d", &testChoice);
		float Time = 0.0;//ȫ��ʱ�������������ʱ
		Building building;//ȫ�ֱ�����ǰϵͳ
		building.testPeoples = createPeoples(Population, 0, MaxFloor, testChoice);
		building.amount = 0;
		Elevator elevator;
		initElevator(elevator);
		//��ʼ������ϵͳ
		for (int i = 0; i <= MaxFloor; i++) {
			initStack(elevator.stacks[i], 5, 3);
			initQueue(elevator.queues[i], 5, 3);
			elevator.callDown[i] = 0;
			elevator.callUp[i] = 0;
			elevator.callCar[i] = 0;
		}
		for (; Time < endTime; Time += 0.1) {
			isPeopleComeInSystem(building, Time, elevator);
			isPeopleOutSystem(building, Time, elevator);
			elevatorAction(building, Time, elevator);
			peopleAction(building, Time, elevator);
			isPeopleInElevator(building, Time, elevator);
			isPeopleOutElevator(building, Time, elevator);
		}
	}
	return 0;
}
