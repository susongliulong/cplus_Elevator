#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"Elevator.h"
#include"People.h"
#include"queue.h"
#include"Stack.h"
#include"Building.h"


bool iscallUpOrDowmEmpty(Building &building, float &Time, Elevator elevator, int direction) {
	if (direction == 1) {
		int i = elevator.Floor + 1;
		for (; i <= MaxFloor; i++) {
			if (elevator.callDown[i] != 0 || elevator.callUp[i] != 0 || elevator.callCar[i] != 0) {
				return 0;
			}
		}
		return 1;
	}
	else {
		int i = elevator.Floor - 1;
		for (; i >= 0; i--) {
			if (elevator.callDown[i] != 0 || elevator.callUp[i] != 0 || elevator.callCar[i] != 0) {
				return 0;
			}
		}
		return 1;
	}

}
int controller(Building &building, float &Time, Elevator &elevator) {
	//若State≠Idle，则返回原来状态
	if (elevator.state == GoingDown || elevator.state == GoingUp) {
		return elevator.state;
	}
	//开门判断
	if (elevator.state != CloseDoor && elevator.D1 == 0 && (elevator.callDown[elevator.Floor] != 0 ||
		elevator.callUp[elevator.Floor] != 0 || elevator.callCar[elevator.Floor] != 0)) {
		return OpenDoor;
	}
	//关门判断,排队的所有人都进入到电梯中
	if (elevator.D1 == 1 && ((isEmpty(elevator.queues[elevator.Floor]) || elevator.capacity <= elevator.size) && elevator.D3 == 0)) {
		elevator.D3 = 1;
		printf("%.1f时刻，电梯准备关门\n", Time);
		elevator.checkElevator = Time;
		elevator.elevatorOpenOrCloseDoorTime = Time;
		elevator.callDown[elevator.Floor] = elevator.callUp[elevator.Floor] = 0;
	}
	if (elevator.D3 == 1 && elevator.state != CloseDoor) {
		//4秒以内没有人来排队，关门
		//遍历building. testPeoples，找到最早出现当前楼层人的时间间隔
		int interval = 0;
		if (elevator.capacity <= elevator.size) {
			return CloseDoor;
		}
		if (building.amount >= Population) {
			//后面没有人，直接关门
			if (Time - elevator.checkElevator >= 4 || elevator.capacity < elevator.size) {
				elevator.checkElevator = Time;
				return CloseDoor;
			}
		}
		for (int i = building.testPeoples[building.amount].id; i < Population&&building.amount < Population; i++) {
			if (building.testPeoples[i].inFloor != elevator.Floor) {
				interval += building.testPeoples[i].interTime;
			}
		}
		if (interval - Time + building.intervalOfPeople > 4) {
			if (Time - elevator.checkElevator >= 4) {
				return CloseDoor;
			}
		}
	}
	//移动判断
	if (elevator.D1 == 0 && elevator.callCar[elevator.Floor] == 0
		&& (elevator.callDown[elevator.Floor] == 0 || elevator.callUp[elevator.Floor] == 0)) {
		
		if (elevator.Floor<MaxFloor&&elevator.direction == 1 && !iscallUpOrDowmEmpty(building, Time, elevator, 1)) {
			elevator.state = GoingUp;
			return elevator.state;
		}
		if (elevator.Floor>0 &&elevator.direction == 0 && !iscallUpOrDowmEmpty(building, Time, elevator, 0)) {
			elevator.state = GoingDown;
			return elevator.state;
		}
		int min = MaxFloor;
		for (int i = 0; i <=MaxFloor; i++) {
			if (elevator.Floor == i)continue;
			if (elevator.callDown[i] != 0 && min > i) {
				min = i;
			}
			if (elevator.callUp[i] != 0 && min > i) {
				min = i;
			}
			if (elevator.callCar[i] != 0 && min > i) {
				min = i;
			}
		}
		//如果Floor>j，则置State为GoingDown；如果Floor<j，则置State为GoingUp。
		if (min == MaxFloor)min = 1;
		int tempState = elevator.state;
		if (min > elevator.Floor) {
			elevator.state = GoingUp;
		}
		else if (min < elevator.Floor) {
			elevator.state = GoingDown;
		}
		else {
			//min==elevator.floor，此时只有一种情况，模拟结束
			if (building.endFlag == 0) {
				printf("%.1f时刻，电梯停留在第一层,等候新的乘客进入\n", Time);
				printf("模拟结束\n");
				building.endFlag++;
			}
		}
	}
	return elevator.state;
}

void ElevatorRun(Building &building, float &Time, Elevator &elevator, int direction) {

	float runTime = 5.1;
	float slowTime = 1.4;
	char *move = "上升";
	if (direction == 0) {
		runTime = 6.1;
		slowTime = 2.3;
		move = "下降";
	}
	if (Time - elevator.elevatorRunTime <= 1.5) {
		if (elevator.elevatorStartRunTimeFlag == 1) {
			printf("%.1f时刻，电梯在加速%s\n", Time, move);
			elevator.elevatorStartRunTimeFlag = 0;
		}
	}
	else if (Time - elevator.elevatorRunTime <= runTime + 1.5) {
		if (elevator.elevatorRunTimeFlag == 1) {
			printf("%.1f时刻，电梯在%s\n", Time, move);
			elevator.elevatorRunTimeFlag = 0;
		}
	}
	else {
		int flag = 0;
		if (direction == 1) {
			if (elevator.callCar[elevator.Floor + 1] == 1 || elevator.callUp[elevator.Floor + 1] == 1 ||
				((elevator.Floor + 1 == 1 || elevator.callDown[elevator.Floor + 1] == 1) && iscallUpOrDowmEmpty(building, Time, elevator, 1))) {
				flag = 1;
			}
		}
		else {
			if (elevator.callCar[elevator.Floor - 1] == 1
				|| elevator.callDown[elevator.Floor - 1] == 1 ||
				((elevator.Floor - 1 == 1 || elevator.callUp[elevator.Floor - 1] == 1) && iscallUpOrDowmEmpty(building, Time, elevator, 0))) {
				flag = 1;
			}
		}
		if (iscallUpOrDowmEmpty(building, Time, elevator, 1) || iscallUpOrDowmEmpty(building, Time, elevator, 0)) {
			flag = 1;
		}
		else {
			building.endFlag = 0;
		}
		if (flag == 1) {
			if (Time - elevator.elevatorRunTime <= runTime + 1.5 + slowTime) {
				if (elevator.elevatorSlowTimeFlag == 1) {
					printf("%.1f时刻，该楼层方向电梯在减速\n", Time);
					elevator.elevatorSlowTimeFlag = 0;
				}
			}
			else {
				elevator.elevatorSlowTimeFlag = 1;
			}
		}
		if (elevator.elevatorSlowTimeFlag == 1) {
			if (direction == 1) elevator.Floor += 1;
			else elevator.Floor -= 1;
			printf("%.1f时刻，电梯%s到%d楼\n", Time, move, elevator.Floor);
			elevator.state = Idle;
			elevator.elevatorRunTime = Time;
			elevator.elevatorOpenOrCloseDoorTime = Time;
			elevator.elevatorRunTimeFlag = elevator.elevatorSlowTimeFlag = elevator.elevatorStartRunTimeFlag = 1;
		}
	}
}
void elevatorAction(Building &building, float &Time, Elevator &elevator) {
	int state = controller(building, Time, elevator);
	if (state == OpenDoor) {
		if (Time - elevator.elevatorOpenOrCloseDoorTime >= 2) {
			printf("%.1f时刻，电梯开门完成，等待人进出\n", Time);
			elevator.D1 = 1;
		}
	}
	else if (state == CloseDoor) {
		if (Time - elevator.elevatorOpenOrCloseDoorTime >= 2 + 4) {
			printf("%.1f时刻，电梯等待四秒后，无人进出，在%d层关门完成,开始移动\n", Time, elevator.Floor);
			elevator.state = Idle;
			elevator.D1 = 0;
			elevator.D3 = 0;
			elevator.callCar[elevator.Floor] = 0;
			elevator.callDown[elevator.Floor] = elevator.callUp[elevator.Floor] = 0;
			elevator.elevatorRunTime = Time;
		}
	}
	else if (state == GoingUp) {
		//上升预购5秒的时间
		elevator.state = GoingUp;
		elevator.direction = 1;
		ElevatorRun(building, Time, elevator, 1);
	}
	else if (state == GoingDown) {
		elevator.state = GoingDown;
		elevator.direction = 0;
		ElevatorRun(building, Time, elevator, 0);
	}
}
void initElevator(Elevator &elevator) {
	elevator.D1 = elevator.D2 = elevator.D3 = 0;
	elevator.size = 0;
	elevator.Floor = 1;
	elevator.capacity = 18;
	elevator.state = Idle;
	elevator.callCar = (int*)malloc(sizeof(int)*(MaxFloor + 1));
	elevator.checkElevator = INT_MAX;
	elevator.elevatorRunTime = 0;
}