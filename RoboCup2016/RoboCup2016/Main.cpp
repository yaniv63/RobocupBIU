#include "Brain/Brain.h"
#include "Communication/Communication.h"

void Debug()
{
	cout << "A";
	int a;
	cin >> a;
}

int main(int argc, char* argv[])
{
	Brain::GetInstance()->RunThread();
	Vision::GetInstance()->Run();
	Communication::GetInstance();

	/*bool flag;
	RoboCupGameControlData Data;
	while (1){
		flag = Communication::GetInstance()->getFlag();
		if (flag){
			cout << "Flag was set" << endl;
			Data = Communication::GetInstance()->ReadDataAndClearFlag();
			flag = Communication::GetInstance()->getFlag();
			if (flag) cout << "After ReadDataAndClearFlag, flag is set. Not good!" << endl;
			else 	  cout << "After ReadDataAndClearFlag, flag is down as needed!" << endl;
			printf("header %s, packet number %d ,state %d,secRemaining %d,team number %d \n",Data.header,Data.packetNumber,Data.state,Data.secsRemaining,Data.teams[0].teamNumber);
		}
	}
*/
	char key;
	cout << "Do you want to free all the engines? [y/n]" << endl;
	cin >> key;
	if (key == 'y' || key == 'Y')
	{
		Motion::GetInstance()->FreeAllEngines();
	}

	cout << "Program finished" << endl;

	return 0;
}

