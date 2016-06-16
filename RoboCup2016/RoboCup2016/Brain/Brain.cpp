#include "Brain.h"
#include "MenuOption.h"

// Declartion of the thread method.
void* BrainActionAsync(void*);
MenuOption ShowMenuAndGetOptionFromUser();

Brain* Brain::m_instance = NULL;
Brain* Brain::GetInstance()
{
    if(m_instance == NULL)
    {
    	m_instance = new Brain();
        return m_instance;
    }
    else
    {
        return m_instance;
    }
}

Brain::Brain()
{

}

Brain::~Brain()
{

}

void Brain::RunThread()
 {
	pthread_t brainThread;

	if (1 == pthread_create(&brainThread, NULL, BrainActionAsync, NULL)) {
		fprintf(stderr, "Couldn't create Brain thread\n");
		exit(1);
	}
}

void* BrainActionAsync(void*)
{
	// Sleep until the Vision thread will print its header.
	//usleep(1000000);

	bool terminate = false;
	Motion* motion = Motion::GetInstance();
	motion->StartEngines();

	while (!terminate)
	{
		MenuOption menuOption = ShowMenuAndGetOptionFromUser();
		switch (menuOption)
		{
			case Play:
			{
				Log::GetInstance()->Info("Starting to play", "Brain");
				StateMachine fsm;
				fsm.Run();
				terminate = true;
				break;
			}
			case Stand:
			{
				motion->RunAction(ActionPage::Init);
				break;
			}
			case Kick:
			{
				motion->RunAction(ActionPage::Kick);
				break;
			}
			case Exit:
			{
				terminate = true;
				break;
			}
			case GetTilt:
			{
				HeadTilt currentTilt = motion->GetHeadTilt();
				cout << "Pan: " << currentTilt.Pan << ", Tilt:" << currentTilt.Tilt << endl;
				break;
			}
			case SetTilt:
			{
				float pan,tilt;
				cout << "Enter pan to set:" << endl;
				cin >> pan;
				cout << "Enter tilt to set:" << endl;
				cin >> tilt;
				motion->SetHeadTilt(HeadTilt(tilt, pan));
				break;
			}

			//TODO: Finish options

			case Walk:
			{
				motion->StartWalking();
				while (MotionStatus::FALLEN == STANDUP)
				{
					printf( "Robot is walking!\n");
					//keep walking
				}
			}

			case TurnLeft:
			{
				break;
			}

			case TurnRight:
			{
				break;
			}

			case StopWalking:
			{
				motion->StopWalking();
				break;
			}

			case Run:
			{
				motion->StartWalking();
				for (int i=0; i<30; i++)
				{
					usleep(250*1000);
					Walking::GetInstance()->X_MOVE_AMPLITUDE = i;
				}
				for (int i=30; i>0; i--)
				{
					usleep(250*1000);
					Walking::GetInstance()->X_MOVE_AMPLITUDE = i;
				}
			}

			case Reset:
			{
				break;
			}
		}
	}
	return NULL;
}

MenuOption ShowMenuAndGetOptionFromUser()
{
	MenuOption menuOption;
	cout << "Please choose an option:" << endl;
	for (int option = MenuOption::Play ; option != MenuOption::Exit ; option++)
	{
		menuOption = static_cast<MenuOption>(option);
		cout << menuOption << " - " << MenuOptionString[menuOption] << endl;
	}

	int userOption;
	cin >> userOption;
	menuOption = static_cast<MenuOption>(userOption);
	return menuOption;
}



