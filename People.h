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
	int giveUpTime;//最长等待时间
	int waitTime=-1;//等待时间
	int interTime;//下一个人进入电梯时间
	int state = OutSystem;//乘客状态
	float walkTime;//乘客开始进入或离开电梯时刻
}People;
