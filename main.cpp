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
	//人的行为
	People * createPeoples(int number, int minFloor, int maxFloor, int busy);//生成测试乘客信息
	void isPeopleComeInSystem(Building &building, float &Time, Elevator &elevator);//应状态是否进入系统，在排队
	void isPeopleOutSystem(Building &building, float &Time, Elevator &elevator);//对应状态是否离开系统
	void isPeopleOutElevator(Building &building, float &Time, Elevator &elevator);//对应状态是否离开电梯
	void isPeopleInElevator(Building &building, float &Time, Elevator &elevator);//对应状态是否进入电梯
	void peopleAction(Building &building, float &Time, Elevator &elevator);//对应动作开始进入电梯或者离开电梯
	
	//电梯行为
	void initElevator(Elevator &elevator);//初始化电梯参数
	int controller(Building &building, float &Time, Elevator &elevator);//电梯进行相关动作，状态转换判断
	bool iscallUpOrDowmEmpty(Building &building, float &Time, Elevator elevator, int direction);//辅助函数，判断当前楼层以上（以下）是否为空
	void elevatorAction(Building &building, float &Time, Elevator &elevator);//电梯动作：开关门，开始移动
	void ElevatorRun(Building &building, float &Time, Elevator &elevator, int direction);//对应电梯移动过程中状态改变（加速，匀速，减速）
	float endTime;
	int testChoice;
	while (1) {
		printf("请输入测试结束的时间,以秒为单位,精确到小数点后一位\n");
		scanf("%f", &endTime);
		printf("输入想测试的模式:");
		printf("1代表高峰时间段,0代表普通时间段\n");
		scanf("%d", &testChoice);
		float Time = 0.0;//全局时间变量，用来记时
		Building building;//全局变量当前系统
		building.testPeoples = createPeoples(Population, 0, MaxFloor, testChoice);
		building.amount = 0;
		Elevator elevator;
		initElevator(elevator);
		//初始化电梯系统
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
