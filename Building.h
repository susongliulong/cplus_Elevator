
typedef struct {
	int elevatorNumber;
	Elevator *elevators;
	int amount;//��ǰϵͳ�ڲ�����
	People *testPeoples;
	int intervalOfPeople = 0;//������ϵͳ���˵�ʱ�̣������жϵ����Ƿ�Ӧ�ù���
	int endFlag = 0;//ģ�������־
}Building;
