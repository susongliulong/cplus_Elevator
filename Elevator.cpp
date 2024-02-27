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
	//��State��Idle���򷵻�ԭ��״̬
	if (elevator.state == GoingDown || elevator.state == GoingUp) {
		return elevator.state;
	}
	//�����ж�
	if (elevator.state != CloseDoor && elevator.D1 == 0 && (elevator.callDown[elevator.Floor] != 0 ||
		elevator.callUp[elevator.Floor] != 0 || elevator.callCar[elevator.Floor] != 0)) {
		return OpenDoor;
	}
	//�����ж�,�Ŷӵ������˶����뵽������
	if (elevator.D1 == 1 && ((isEmpty(elevator.queues[elevator.Floor]) || elevator.capacity <= elevator.size) && elevator.D3 == 0)) {
		elevator.D3 = 1;
		printf("%.1fʱ�̣�����׼������\n", Time);
		elevator.checkElevator = Time;
		elevator.elevatorOpenOrCloseDoorTime = Time;
		elevator.callDown[elevator.Floor] = elevator.callUp[elevator.Floor] = 0;
	}
	if (elevator.D3 == 1 && elevator.state != CloseDoor) {
		//4������û�������Ŷӣ�����
		//����building. testPeoples���ҵ�������ֵ�ǰ¥���˵�ʱ����
		int interval = 0;
		if (elevator.capacity <= elevator.size) {
			return CloseDoor;
		}
		if (building.amount >= Population) {
			//����û���ˣ�ֱ�ӹ���
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
	//�ƶ��ж�
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
		//���Floor>j������StateΪGoingDown�����Floor<j������StateΪGoingUp��
		if (min == MaxFloor)min = 1;
		int tempState = elevator.state;
		if (min > elevator.Floor) {
			elevator.state = GoingUp;
		}
		else if (min < elevator.Floor) {
			elevator.state = GoingDown;
		}
		else {
			//min==elevator.floor����ʱֻ��һ�������ģ�����
			if (building.endFlag == 0) {
				printf("%.1fʱ�̣�����ͣ���ڵ�һ��,�Ⱥ��µĳ˿ͽ���\n", Time);
				printf("ģ�����\n");
				building.endFlag++;
			}
		}
	}
	return elevator.state;
}

void ElevatorRun(Building &building, float &Time, Elevator &elevator, int direction) {

	float runTime = 5.1;
	float slowTime = 1.4;
	char *move = "����";
	if (direction == 0) {
		runTime = 6.1;
		slowTime = 2.3;
		move = "�½�";
	}
	if (Time - elevator.elevatorRunTime <= 1.5) {
		if (elevator.elevatorStartRunTimeFlag == 1) {
			printf("%.1fʱ�̣������ڼ���%s\n", Time, move);
			elevator.elevatorStartRunTimeFlag = 0;
		}
	}
	else if (Time - elevator.elevatorRunTime <= runTime + 1.5) {
		if (elevator.elevatorRunTimeFlag == 1) {
			printf("%.1fʱ�̣�������%s\n", Time, move);
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
					printf("%.1fʱ�̣���¥�㷽������ڼ���\n", Time);
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
			printf("%.1fʱ�̣�����%s��%d¥\n", Time, move, elevator.Floor);
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
			printf("%.1fʱ�̣����ݿ�����ɣ��ȴ��˽���\n", Time);
			elevator.D1 = 1;
		}
	}
	else if (state == CloseDoor) {
		if (Time - elevator.elevatorOpenOrCloseDoorTime >= 2 + 4) {
			printf("%.1fʱ�̣����ݵȴ���������˽�������%d��������,��ʼ�ƶ�\n", Time, elevator.Floor);
			elevator.state = Idle;
			elevator.D1 = 0;
			elevator.D3 = 0;
			elevator.callCar[elevator.Floor] = 0;
			elevator.callDown[elevator.Floor] = elevator.callUp[elevator.Floor] = 0;
			elevator.elevatorRunTime = Time;
		}
	}
	else if (state == GoingUp) {
		//����Ԥ��5���ʱ��
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