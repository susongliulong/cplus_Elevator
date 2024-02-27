#pragma once
#define GoingUp 2
#define Idle 3
#define GoingDown -1
#define OpenDoor 1
#define CloseDoor 0
#include"Stack.h"
#include"queue.h"
#include"Config.h"
typedef struct {
	int Floor;//���ݵ�ǰ¥��
	int direction;//�������з���
	int D1;//���ݹ��ŵ�ʱ��ֵΪ0�����ݿ������˽���ֵΪ1
	int D2;//��ĳ��¥��ͣ��300tʱ��ֵΪ0
	int D3;//�����������ŵ������˽�������,����׼������ֵΪ1���������ֵΪ0��
	int capacity;//��������
	int size;
	int state;
	int *callCar;//�����ڲ�¥�㰴ť
	int callUp[MaxFloor+1];//�����������ϵİ�ť
	int callDown[MaxFloor+1];//�����������µİ�ť
	float checkElevator;//û���˽���ʱ���������Ƿ�Ӧ�ù���
	float elevatorRunTime;//���ݿ�ʼ����ʱ��
	float elevatorOpenOrCloseDoorTime=0;//����׼��������ʱ��
	int elevatorRunTimeFlag = 1;//����ƽ�����б�־
	int elevatorStartRunTimeFlag = 1;//���ݿ�ʼ���У����٣���־
	int elevatorSlowTimeFlag = 1;//���ݼ��ٱ�־
	Stack stacks[MaxFloor + 1];//
	Queue queues[MaxFloor + 1];//ÿ��¥�ĺ򲹶���
}Elevator;
