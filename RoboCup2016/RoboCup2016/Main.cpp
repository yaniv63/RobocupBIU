#include "Brain/Brain.h"

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

