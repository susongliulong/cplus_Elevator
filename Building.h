
typedef struct {
	int elevatorNumber;
	Elevator *elevators;
	int amount;//当前系统内部人数
	People *testPeoples;
	int intervalOfPeople = 0;//最后进入系统的人的时刻，用来判断电梯是否应该关门
	int endFlag = 0;//模拟结束标志
}Building;
