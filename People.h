#pragma once
#define WaitInQueue 2
#define MoveInElevator 3
#define InElevator 4
#define OutElevator 5
#define OutSystem 1
typedef struct {
	int id=-1;
	int inFloor;
	int outFloor;
	int giveUpTime;//��ȴ�ʱ��
	int waitTime=-1;//�ȴ�ʱ��
	int interTime;//��һ���˽������ʱ��
	int state = OutSystem;//�˿�״̬
	float walkTime;//�˿Ϳ�ʼ������뿪����ʱ��
}People;
