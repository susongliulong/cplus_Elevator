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
	int Floor;//电梯当前楼层
	int direction;//电梯运行方向
	int D1;//电梯关门的时候值为0，电梯开门有人进出值为1
	int D2;//在某层楼上停留300t时，值为0
	int D3;//电梯门正开着但是无人进出电梯,电梯准备关门值为1，其他情况值为0；
	int capacity;//电梯容量
	int size;
	int state;
	int *callCar;//电梯内部楼层按钮
	int callUp[MaxFloor+1];//电梯外面向上的按钮
	int callDown[MaxFloor+1];//电梯外面向下的按钮
	float checkElevator;//没有人进出时，检查电梯是否应该关门
	float elevatorRunTime;//电梯开始运行时刻
	float elevatorOpenOrCloseDoorTime=0;//电梯准备开关门时间
	int elevatorRunTimeFlag = 1;//电梯平稳运行标志
	int elevatorStartRunTimeFlag = 1;//电梯开始运行（加速）标志
	int elevatorSlowTimeFlag = 1;//电梯减速标志
	Stack stacks[MaxFloor + 1];//
	Queue queues[MaxFloor + 1];//每层楼的候补队列
}Elevator;
