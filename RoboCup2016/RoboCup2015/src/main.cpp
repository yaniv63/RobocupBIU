/*
 * main.cpp
 *
 *  Created on: 2011. 1. 4.
 *      Author: robotis
 */

#include "BrainAndMotion.h"
#include "opencv2/flann/logger.h"
#include "gyro/Gyro.h"
#include "AngleEstimator.h"

void 		freeAllEngines(Robot::CM730 *cm730);
string  	convertIntToString(int number);
int compass_Gyro(CM730 *cm730, int id, double & current_angel);
void* 		VisionThread(void*);
void* 		gyroThread(void*);
bool 		runthr= true;
pthread_t 	VisionT = 0;
pthread_t 	gyroT = 0;
double angel = 0;


S_StateStruct StateList[] =
{
		{	NOP 	 		, "NOP"			},
		{	Init 	 		, "Init"		},
		{	Stop  	 		, "Stop"		},
		{	GoToBall 		, "GoToBall"	},
		{	KickBall 		, "KickBall"	},
		{	LookForBall		, "LookForBall" },
		{	FindGate 		, "FindGate"	},
		{	ChangeSpot		, "ChangeSpot"	},
		{	GoForKick		, "GoForKick"	},
		{	EXIT			, "EXIT"		},
};

LinuxCM730 linux_cm730("/dev/ttyUSB0");
CM730 cm730(&linux_cm730);
ofstream logFile;
char playing = 'y';
Ball ball;
sem_t ball_sem;
S_BrainBall BrainBall;
E_State Robotstate = NOP;

void ChangeCurrentDir()
{
    char exepath[1024] = {0};
    if(readlink("/proc/self/exe", exepath, sizeof(exepath)) != -1)
        chdir(dirname(exepath));
}


int main(void)
{

    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    stringstream p;
    p<<"[" << timeinfo->tm_hour<< ":"<< timeinfo->tm_min << ":" << timeinfo->tm_sec <<"]";//defines log files name
	std::string logname = "/home/robot/Logs/log";
	logname+=p.str();
    startLog(logFile,logname);
    cvflann::Logger::setDestination("/home/robot/Logs/log_rami_and_lihen");
    cvflann::Logger::setLevel(4);
    char* test_text = "works";
    cvflann::Logger::log(1,test_text);
	if (sem_init(&ball_sem, 0, 1) != 0)
	{
		printf("Couldn't sem_init\n");
	}
	for (int i=0; i<BrainBall.x_detail ;i++)
	{
		BrainBall.x_place[i] = 0;
	}



    printf( "\n===== Action script for DARwIn =====\n\n");
    ChangeCurrentDir();
    minIni* ini = new minIni(INI_FILE_PATH);
    Action::GetInstance()->LoadFile(MOTION_FILE_PATH);

    //////////////////// Framework Initialize ////////////////////////////
    if(MotionManager::GetInstance()->Initialize(&cm730) == false)
    {
		linux_cm730.SetPortName("/dev/ttyUSB1");
		if(MotionManager::GetInstance()->Initialize(&cm730) == false)
			{
				printf("Fail to initialize Motion Manager!\n");
				return 0;
			}
    }
    Walking::GetInstance()->LoadINISettings(ini);
	usleep(100);
    MotionManager::GetInstance()->LoadINISettings(ini);
	
    MotionManager::GetInstance()->AddModule((MotionModule*)Action::GetInstance());
    MotionManager::GetInstance()->AddModule((MotionModule*)Head::GetInstance());
    MotionManager::GetInstance()->AddModule((MotionModule*)Walking::GetInstance());	
    LinuxMotionTimer linuxMotionTimer;
    linuxMotionTimer.Initialize(MotionManager::GetInstance());
    linuxMotionTimer.Start();
    /////////////////////////////////////////////////////////////////////

    Walking::GetInstance()->m_Joint.SetEnableBody(false);
    Head::GetInstance()->m_Joint.SetEnableBody(false);
    Action::GetInstance()->m_Joint.SetEnableBody(true);
    MotionManager::GetInstance()->SetEnable(true);

    Action::GetInstance()->Start(1);     //Init(stand up) pose

    Action::GetInstance()->Brake();
    while(Action::GetInstance()->IsRunning()) usleep(8*1000);

    cm730.WriteByte(CM730::P_LED_PANNEL, 0x02, NULL);
   MotionManager::GetInstance()->ResetGyroCalibration();

   //pthread_create(&VisionT, NULL, VisionThread,(void *) NULL);
     pthread_create(&gyroT, NULL, gyroThread,(void *) NULL);

    //Gyro gyro(&cm730);
    //gyro.RunThread();
    printf("Press the ENTER key to begin!\n");
    getchar();
	Calibrator* calibrator;

//////
    writeToLog(logFile,"started");

printf("0-Play\n");
printf("1-Kick\n");
printf("2-stand\n");
printf("3-Straight walking\n");
printf("4-Turn right\n");
printf("5-Turn left\n");
printf("6-stop walking\n");
printf("7-reset\n");
printf("8-GetTilt\n");
printf("9-SetTilt\n");
printf("q-Exit\n");

Head::GetInstance()->m_Joint.SetEnableHeadOnly(true, true);

	while(1)
	{
		char c =getchar();

		if (c == '0')
		{
			play();
		}
		else if(c=='d')
		{
			int i=0;
			char e;
			AngleEstimator angle;
			while(e!='q'){
			i++;
			cout <<angle.yaw()<<endl;
			if(i==100)
			{
			 cout<<"enter char"<<endl;
			 cin>>e;
			 i=0;
			}
			}
		}
		else if (c == '1')
		{
			writeToLog(logFile,"kick");
			Action::GetInstance()->Start(4);
			while(Action::GetInstance()->IsRunning()) usleep(8*1000);
		}
		else if(c == '2')
		{
			writeToLog(logFile,"stand");
			Action::GetInstance()->Start(1);
			while(Action::GetInstance()->IsRunning()) usleep(8*1000);
		}
		else if(c == '3')
		{
			if(Walking::GetInstance()->IsRunning() == false )
			{
				fprintf(stderr, "STARTING\n");
				Action::GetInstance()->Start(9);
				while(Action::GetInstance()->IsRunning()) usleep(8*1000);
				//Head::GetInstance()->m_Joint.SetEnableHeadOnly(true, true);
				Walking::GetInstance()->m_Joint.SetEnableBodyWithoutHead(true,true);
				Walking::GetInstance()->X_MOVE_AMPLITUDE = 0;
				Walking::GetInstance()->Y_MOVE_AMPLITUDE = 0;
				Walking::GetInstance()->A_MOVE_AMPLITUDE = 0;
				Walking::GetInstance()->Start();
			}
			else
				Walking::GetInstance()->A_MOVE_AMPLITUDE = 0;
		}
		else if(c == '4')
		{
			if(Walking::GetInstance()->IsRunning() == true )
			{
				fprintf(stderr, "TURNING RIGHT\n");
				Walking::GetInstance()->A_MOVE_AMPLITUDE = -5;

			}
		}
		else if(c == '5')
		{
			if(Walking::GetInstance()->IsRunning() == true )
			{
				fprintf(stderr, "TURNING LEFT\n");
				Walking::GetInstance()->A_MOVE_AMPLITUDE = 5;
			}
		}
		else if(c == '6')
		{
			if(Walking::GetInstance()->IsRunning() == true )
			{
				fprintf(stderr, "STOPPING\n");		
				Walking::GetInstance()->Stop();
				while(Walking::GetInstance()->IsRunning() == 1) usleep(8000);
				Walking::GetInstance()->m_Joint.SetEnableBody(false);
				Head::GetInstance()->m_Joint.SetEnableBody(false);
				Action::GetInstance()->m_Joint.SetEnableBody(true);
				MotionManager::GetInstance()->SetEnable(true);
			}
		}
		else if(c == '7')
		{
			for(int i=JointData::ID_R_SHOULDER_PITCH; i<JointData::NUMBER_OF_JOINTS; i++)
											Reset(&cm730, i);

									Reset(&cm730, CM730::ID_CM);
		}
		else if(c == '8')
		{

			float tilt;
			tilt = GetTilt();
			printf("%f\n", tilt);

		}
		else if(c == '9')
		{
			float tilt,pan;
			cout<<"enter tilt\n";
			cin>>tilt;
			cout<<"enter pan\n";
			cin>>pan;
			SetTilt(pan,tilt);
		}
		else if(c == 'q')	
		{
			break;
		}
	}

	runthr=false;

	writeToLog(logFile,"normal exit");
	endLog(logFile);
	pthread_join(VisionT, NULL);
	if (sem_destroy(&ball_sem) != 0)
	{
		printf("Couldn't sem_destroy\n");
	}
	//try{gyro.stop();} catch (Exception &e) {cout<<e.msg;}
	freeAllEngines(&cm730);
    return 0;
}


void* gyroThread(void*)
{

	while(runthr)
	{

		compass_Gyro(&cm730,CM730::ID_CM,angel);
		cout<<"gyroThread angel = "<<angel<<endl<<endl;

		//MotionManager::GetInstance()->Process();
		cout <<"yaw: "<<MotionManager::GetInstance()->angleEstimator()->yaw()
		<<" roll: "<<MotionManager::GetInstance()->angleEstimator()->roll()
		<<" pitch: "<<MotionManager::GetInstance()->angleEstimator()->pitch()
		<<" fallen: "<<MotionStatus::FALLEN<<endl;


		sleep(0.1);
	}

}


void* VisionThread(void*)
{
	Vision vis;
    vis.OpenFlyCapCamera();

    VideoWriter outputVideo;
    const string outputFile = "//home/robot/Desktop/rami_lihen/demo.avi";
    outputVideo.open(outputFile, CV_FOURCC('M','J','P','G'), 10, Size(FRAME_WIDTH,FRAME_HEIGHT), true);
    vis.CreateStreamWindow();
    S_BrainBall BrainBallCounter;

	for (int i=0; i<BrainBall.x_detail ;i++)
	{
		BrainBallCounter.x_place[i] = 0;
	}

    Mat m;
    sleep(2);

    while (runthr)
    {
    	vis.GetImage(m);
    	//outputVideo.write(m);
    	if( vis.key == 'q')
    			playing = vis.key;

    	if (vis.m_load_file)
    	{
    		vis.LoadColorsFromFile();
    	}
    	else if (vis.m_save_file)
    	{
    		vis.saveHSVtofile();
    	}

    	if (vis.m_calibrate && vis.m_calib_open)
    	{
    		vis.createGreen();
    		vis.m_calib_open=0;
    		vis.m_calib_close=1;
    	}
    	else if (vis.m_calibrate)
    	{
    		vis.editColorRange(m);
    		vis.showGreen(m);
    	}
    	else if (!vis.m_calibrate && vis.m_calib_close)
    	{
    		vis.closeGreen();
    		vis.m_calib_open=1;
    		vis.m_calib_close=0;
    	}


    	vis.motionTilt= GetTilt();


//////////////////////////////////////////////////////////////////////////////

    	ball = vis.Find_Ball(m);
    	outputVideo.write(m);
    	//updating machine learn array
    	int xIncreasePlace = ((int)ball.center.x)/65;
    	if (BrainBallCounter.x_place[xIncreasePlace] < 20 )
    	{
    		if (ball.radius > 10)
    			BrainBallCounter.x_place[xIncreasePlace]+=2;  //find ball x array place and increase the spot value
    		else if (BrainBallCounter.x_place[xIncreasePlace] > 0)
    			BrainBallCounter.x_place[xIncreasePlace]--;
    	}


    	for (int i=0; i <BrainBallCounter.x_detail;i++)
    	{
    		if (i!=xIncreasePlace && BrainBallCounter.x_place[i]>0)
    			BrainBallCounter.x_place[i]--;
    	}

    	//updating ball location
    	int max = 0;
    	bool present = false;

    	for (int i=0; i <BrainBallCounter.x_detail;i++)
    	{
    		if (BrainBallCounter.x_place[i]>10)
    		{
    			if (present == true && BrainBallCounter.x_place[max] < BrainBallCounter.x_place[i])
    			{
    				max=i;
    			}
    			else if (present==false)
    			{
    				max=i;
    				present=true;
    			}
    		}
    	}

    	/////////////////////////////////////////////
    	if (sem_wait(&ball_sem) != 0)
    		printf("Couldn't sem_wait\n");

    	for (int i=0; i<BrainBall.x_detail ;i++)
    	{
    		BrainBall.x_place[i] = 0;
    	}

    	if (present)
    	{
    		BrainBall.x_place[max] = 1;
    		BrainBall.found= true;
    		BrainBall.radius= ball.radius;
    		BrainBall.x_ball_spot=max;
    		BrainBall.center = ball.center;
    	}
    	else
    	{
    		BrainBall.found= false;
    	}

    	if (sem_post(&ball_sem) != 0)
    		printf("Couldn't sem_post\n");

    	E_State st = Robotstate;
    	//////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////


    	if (ball.found && ball.radius > 0)
    	{
    		cv::circle(m, ball.center, 1, Scalar(255,0,0),4,4);
    		cv::circle(m, ball.center, (int)ball.radius+1, Scalar(255,0,0),4,4);

    		//Elimelech, fix it!
    		Point2f BallDist;
    		if (ball.center.y + ball.radius < FRAME_HEIGHT-1)
    		{
    			BallDist = vis.uv2xy(ball.center.x ,ball.center.y + (int)ball.radius);
    			cv::circle(m, Point(ball.center.x ,ball.center.y + (int)ball.radius), 1, Scalar(0,0,255),4,4);
    		}
    		else
    			BallDist = vis.uv2xy(ball.center.x ,FRAME_HEIGHT-1);


    		char text[70];
    		sprintf(text, "(x,y)=(%d,%d) || ball radius= %d || tilt= %f || (u,v)=(%d,%d)",(int)BallDist.x,(int)BallDist.y,(int)ball.radius, vis.motionTilt,(int)ball.center.x ,(int)ball.center.y);
    		putText(m, text, Point(10,10), Font_Type, FootNoteFont_Scale, Font_Colour,Font_Thickness);
    	}

    	char text[70];
		sprintf(text, "BrainBallCounter [%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]",BrainBallCounter.x_place[0]
														,BrainBallCounter.x_place[1]
														,BrainBallCounter.x_place[2]
														,BrainBallCounter.x_place[3]
														,BrainBallCounter.x_place[4]
														,BrainBallCounter.x_place[5]
														,BrainBallCounter.x_place[6]
														,BrainBallCounter.x_place[7]
														,BrainBallCounter.x_place[8]
														,BrainBallCounter.x_place[9]    );
		putText(m, text, Point(0,30), Font_Type, FootNoteFont_Scale, Font_Colour,Font_Thickness);

		sprintf(text, "BrainBall [%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]",BrainBall.x_place[0]
														,BrainBall.x_place[1]
														,BrainBall.x_place[2]
														,BrainBall.x_place[3]
														,BrainBall.x_place[4]
														,BrainBall.x_place[5]
														,BrainBall.x_place[6]
														,BrainBall.x_place[7]
														,BrainBall.x_place[8]
														,BrainBall.x_place[9]    );
		putText(m, text, Point(0,50), Font_Type, FootNoteFont_Scale, Font_Colour,Font_Thickness);


		sprintf(text, "******************* %s ********************", StateList[st].stateString.c_str() );
		putText(m, text, Point(0,70), Font_Type, FootNoteFont_Scale, Font_Colour,1);

    	vis.ShowImage(m);
    }

	vis.CloseCamera();

	return NULL;
}


#define TURN_LEFT 		0
#define TURN_RIGHT 		1
#define SIZE_OF_X_ARRAY 10
int FindTurnDirection(){

	if (sem_wait(&ball_sem) != 0)
		printf("Couldn't sem_wait\n");

	S_BrainBall ball_location = BrainBall;

	if (sem_post(&ball_sem) != 0)
		printf("Couldn't sem_post\n");


	int max_value_index=0, max_value=0;
	for (int i=0; i <SIZE_OF_X_ARRAY;i++){
		writeToLog(logFile,"FindTurnDirection: index is ",convertIntToString(i));
		writeToLog(logFile,"FindTurnDirection: value is ",convertIntToString(ball_location.x_place[i]));
		if (ball_location.x_place[i]>max_value){
			max_value_index = i;
			max_value = ball_location.x_place[i];
		}
	}
	int temp = ((max_value_index < (SIZE_OF_X_ARRAY/2)) ? TURN_LEFT : TURN_RIGHT);
	writeToLog(logFile,"FindTurnDirection: max_value_index= ",convertIntToString(max_value_index));
	writeToLog(logFile,"FindTurnDirection: max_value= ",convertIntToString(max_value));
	writeToLog(logFile,"FindTurnDirection: decision returned= ",convertIntToString(temp));
	printf("   FindTurnDirection: max_value_index=%d, max_value=%d, decision returned=%d",max_value_index,max_value,temp);
	return ((max_value_index < (SIZE_OF_X_ARRAY/2)) ? TURN_LEFT : TURN_RIGHT);
}

/*****************************************************************************************
* Method Name: play
* Description: main state machine
* Arguments: None
* Return Values: none
* Wrote: Roman & Gilad
*****************************************************************************************/
void play()
{
	int headPosCheck = 1;
	int counter_ball_not_found=0;
	E_HeadPosition HeadPosition = LUnknown;
	writeToLog(logFile,"HeadPosition = LUnknown");

	E_State State;
	State = Init;

	while (playing!='q')
	{
    	if (sem_wait(&ball_sem) != 0)
    		printf("Couldn't sem_wait\n");

    	S_BrainBall ball1 = BrainBall;

    	Robotstate = State;

    	if (sem_post(&ball_sem) != 0)
    		printf("Couldn't sem_post\n");

		//////////////////////INIT////////////////////////////////////

		if (State == Init)
		{
			writeToLog(logFile,"State == Init");
			Action::GetInstance()->Start(1);
			while(Action::GetInstance()->IsRunning()) usleep(8*1000);
			SetTilt(0,0);
			HeadPosition = LookingStraight;
			writeToLog(logFile,"HeadPosition = LookingStraight");
			State = LookForBall;
		}

		//////////////////////LOOK_FOR_BALL////////////////////////////////////

		else if (State == LookForBall)
		{
			Head::GetInstance()->m_Joint.SetEnableHeadOnly(true, true);
			writeToLog(logFile,"State == LookForBall");

	    	//sleep(1);qqq
	    	if (ball1.found)
	    	{
	    		State = GoToBall;
	    		writeToLog(logFile,"State == LookForBall - found");
	    		//headPosCheck = 1;
	    	}
	    	else if (headPosCheck == 1)
	    	{
	    		writeToLog(logFile,"State == LookForBall - LookingStraight");
	    		headPosCheck = 2;
	    		SetTilt(0,0);  //straight
				HeadPosition = LookingStraight;
				writeToLog(logFile,"HeadPosition = LookingStraight");
	    		sleep(2);
	    	}
	    	else if (headPosCheck == 2)
	    	{
	    		writeToLog(logFile,"State == LookForBall - LookingStraightDown");
	    		headPosCheck = 3;
	    		SetTilt(0,-25);  //back to center
				HeadPosition = LookingStraightDown;
				writeToLog(logFile,"HeadPosition = LookingStraightDown");
	    		sleep(2);
	    	}
	    	else if (headPosCheck == 3)
	    	{
	    		writeToLog(logFile,"State == LookForBall - LookingLeft");
	    		headPosCheck = 4;
	    		SetTilt(30,-15);  //left
				HeadPosition = LookingLeft;
			writeToLog(logFile,"State == LookForBall");

	    	//sleep(1);qqq
	    	if (ball1.found)
	    	{
	    		State = GoToBall;
	    		writeToLog(logFile,"State == LookForBall - found");
	    		//headPosCheck = 1;
	    	}
	    	else if (headPosCheck == 1)
	    	{
	    		writeToLog(logFile,"State == LookForBall - LookingStraight");
	    		headPosCheck = 2;
	    		SetTilt(0,0);  //straight
				HeadPosition = LookingStraight;
				writeToLog(logFile,"HeadPosition = LookingStraight");
	    		sleep(2);
	    	}
	    	else if (headPosCheck == 2)
	    	{
	    		writeToLog(logFile,"State == LookForBall - LookingStraightDown");
	    		headPosCheck = 3;
	    		SetTilt(0,-25);  //back to center
				HeadPosition = LookingStraightDown;
				writeToLog(logFile,"HeadPosition = LookingStraightDown");
	    		sleep(2);
	    	}
	    	else if (headPosCheck == 3)
	    	{
	    		writeToLog(logFile,"State == LookForBall - LookingLeft");
	    		headPosCheck = 4;
	    		SetTilt(30,-15);  //left
				HeadPosition = LookingLeft;
				writeToLog(logFile,"HeadPosition = LookingLeft");
	    		sleep(2);
	    	}
	    	else if (headPosCheck == 4)
	    	{
	    		writeToLog(logFile,"State == LookForBall - LookingRight");
	    		headPosCheck = 5;
	    		SetTilt(-30,-15);  //right
				HeadPosition = LookingRight;
				writeToLog(logFile,"HeadPosition = LookingRight");
	    		sleep(2);
	    	}
	    	else if (headPosCheck == 5)
	    	{
	    		writeToLog(logFile,"State == LookForBall - go to different spot");
	    		headPosCheck = 1;
	    		State =ChangeSpot; //and move forward
	    	}
		}

		//////////////////////CHANGE_SPOT////////////////////////////////////

		else if (State == ChangeSpot)
		{
			writeToLog(logFile,"State == ChangeSpot");

    		SetTilt(0,-25);  //back to center
			HeadPosition = LookingStraightDown;
			writeToLog(logFile,"HeadPosition = LookingStraightDown");

			int turn_direction = (FindTurnDirection()) ? TURN_HARD_RIGHT_DIR : TURN_HARD_LEFT_DIR;
			writeToLog(logFile,"turn_direction is", convertIntToString(turn_direction));
			printf("change spot: turn_direction %d",turn_direction);
			GoTo(turn_direction,StateList[State].stateString,ball1.x_ball_spot);

			for (int i=0; i<10; i++) // go forward 40 cm
			{
				if (sem_wait(&ball_sem) != 0)
				printf("Couldn't sem_wait\n");

				S_BrainBall ballinwalk = BrainBall;

				if (sem_post(&ball_sem) != 0)
					printf("Couldn't sem_post\n");

				if (ballinwalk.found)
				{
					State = GoToBall;
					writeToLog(logFile,"State == ChangeSpot - found");
					break;
				}

				sleep(1);
			}

			GoStop();

			State = LookForBall;
		}

		//////////////////////GO_TO_BALL////////////////////////////////////

		else if (State == GoToBall)
		{
			/*//for demo
			GoTo(0,StateList[State].stateString,ball1.x_ball_spot); //forward
			if(ball1.center.y > 360){
				State=GoForKick;
			}*/
			//till demo
			///*
			writeToLog(logFile,"State == GoToBall");

			int dir=ball1.x_ball_spot;

			if (ball1.found == false)
			{
				++counter_ball_not_found;
				writeToLog(logFile,"counter_ball_not_found value is",convertIntToString(counter_ball_not_found));
				if (counter_ball_not_found==3){
					State = LookForBall;
				}
			}
			else if (HeadPosition == LookingStraight)
			{
				counter_ball_not_found=0;
				if(ball1.x_ball_spot > 3 && ball1.x_ball_spot < 7)
				{
					GoTo(0,StateList[State].stateString,ball1.x_ball_spot); //forward
				}
				else if(ball1.x_ball_spot < 4)
				{
					GoTo(dir,StateList[State].stateString,ball1.x_ball_spot);  //left
				}
				else if(ball1.x_ball_spot > 6)
				{
					GoTo(-(dir % 5),StateList[State].stateString,ball1.x_ball_spot);  //Right
				}
				writeToLog(logFile,"GoToBall - LookingStraight");
			}
			else if (HeadPosition == LookingStraightDown)
			{
				counter_ball_not_found=0;
				if(ball1.center.y < 360)
				{
					if(ball1.x_ball_spot > 3 && ball1.x_ball_spot < 7)
					{
						GoTo(0,StateList[State].stateString,ball1.x_ball_spot); //forward
					}
					else if(ball1.x_ball_spot < 4)
					{
						GoTo(dir,StateList[State].stateString,ball1.x_ball_spot);  //left
					}
					else if(ball1.x_ball_spot > 6)
					{

						GoTo(-(dir % 5),StateList[State].stateString,ball1.x_ball_spot);  //right
					}
					writeToLog(logFile,"GoToBall - LookingStraightDown >360");
				}
				else
				{
					std::string g="Next State: GoForKick y=";
					stringstream p;
					p<<(int)ball1.center.y;
					g+=p.str();
					writeToLog(logFile,g);
					State=GoForKick;
				}
			}
			else if (HeadPosition == LookingRight)
			{
				counter_ball_not_found=0;
				if(ball1.x_ball_spot < 6)
				{
					GoTo(-(dir+1),StateList[State].stateString,ball1.x_ball_spot);
				}
				else if(ball1.x_ball_spot > 5)
				{
					GoTo(-dir,StateList[State].stateString,ball1.x_ball_spot);
				}

	    		SetTilt(0,-15);
				HeadPosition = LookingStraight;
				writeToLog(logFile,"HeadPosition = LookingStraight");
				sleep(1);
			}
			else if (HeadPosition == LookingLeft)
			{
				counter_ball_not_found=0;
				if(ball1.x_ball_spot < 6)
				{
					GoTo(8-dir,StateList[State].stateString,ball1.x_ball_spot);
				}
				else if(ball1.x_ball_spot>5) //check older versions
				{
					writeToLog(logFile,"HeHead::GetInstance()->m_Joint.SetEnableHeadOnly(true, true);_ball_spot > 5");
					GoTo(5-(dir%5),StateList[State].stateString,ball1.x_ball_spot);
				}

	    		SetTilt(0,-15);
				HeadPosition = LookingStraight;
				writeToLog(logFile,"HeadPosition = LookingStraight");
				sleep(1);
			}
			//*/
			sleep(1);
		}

		//////////////////////GO FOR KICK////////////////////////////////////

		else if (State == GoForKick)
		{
			if (1)//ball1.center.y>360
			{
				std::string g="State == GoForKick y=";
				stringstream p;
				p<<(int)ball1.center.y;
				g+=p.str();
				writeToLog(logFile,g);

				if(ball1.x_ball_spot > 3 && ball1.x_ball_spot < 7)
				{
					GoTo(0,StateList[State].stateString,ball1.x_ball_spot); //forward
					writeToLog(logFile,"GoForKick forward");
				}
				else if(ball1.x_ball_spot<4)
				{
					GoTo(6,StateList[State].stateString,ball1.x_ball_spot);  //right
					writeToLog(logFile,"GoForKick right");
				}
				else if(ball1.x_ball_spot>6)
				{
					GoTo(-6,StateList[State].stateString,ball1.x_ball_spot);  //left
					writeToLog(logFile,"GoForKick left");
				}

				sleep(4);

				GoTo(0,StateList[State].stateString,ball1.x_ball_spot);

//				if (ball1.center.y<420)
//					sleep(15);
//				sleep(11)
					sleep(20);


				GoStop();
				writeToLog(logFile,"Kick");

				Action::GetInstance()->Start(4);
				while(Action::GetInstance()->IsRunning()) usleep(8*1000);
			}

			State = GoToBall;
		}

		//////////////////////EXIT////////////////////////////////////

		else if (State == EXIT)
		{
			playing='q';
		}
	}


	playing='j';
	GoStop();
}
}





/*****************************************************************************************
* Method Name: GoTo
* Description: - right
* 			   + left
* Arguments: None
* Return Values: None
* Wrote: Gilad & Roman
*****************************************************************************************/
void GoTo(const double& dir,std::string state,int x_ball_spot)
{
	if(Walking::GetInstance()->IsRunning() == false )
	{
		if (dir==0)
			writeToLog(logFile,"Going Forward",state,dir,x_ball_spot);
		else if (dir>0)
			writeToLog(logFile,"Turning Left",state,dir,x_ball_spot);
		else
			writeToLog(logFile,"Turning Right",state,dir,x_ball_spot);


		Action::GetInstance()->Start(9);
		while(Action::GetInstance()->IsRunning()) usleep(8*1000);

		Walking::GetInstance()->m_Joint.SetEnableBodyWithoutHead(true,true);
		Walking::GetInstance()->X_MOVE_AMPLITUDE = 0;
		Walking::GetInstance()->Y_MOVE_AMPLITUDE = 0;
		Walking::GetInstance()->A_MOVE_AMPLITUDE = dir;
		Walking::GetInstance()->Start();
	}
	else if(Walking::GetInstance()->IsRunning() == true )
	{
		if (dir==0)
			writeToLog(logFile,"Going Forward",state,dir,x_ball_spot);
		else if (dir>0)
			writeToLog(logFile,"Turning Left",state,dir,x_ball_spot);
		else
			writeToLog(logFile,"Turning Right",state,dir,x_ball_spot);

		Walking::GetInstance()->A_MOVE_AMPLITUDE = dir;
	}
}


/*****************************************************************************************
* Method Name: GoToUpdateDirection
* Description: + right
* 			   - left
* Arguments: None
* Return Values: None
* Wrote: Gilad & Roman
*****************************************************************************************/
void GoToUpdateDirection(const double& dir)
{
	if(Walking::GetInstance()->IsRunning() == true )
	{
		if (dir>0)
			writeToLog(logFile,"Turning Right");
		else
			writeToLog(logFile,"Turning Left");

		Walking::GetInstance()->A_MOVE_AMPLITUDE = dir;

	}
}


/*****************************************************************************************
* Method Name: GoStop
* Description:
* Arguments: None
* Return Values: None
* Wrote: Gilad & Roman
*****************************************************************************************/
void GoStop()
{
	if(Walking::GetInstance()->IsRunning() == true )
	{
		fprintf(stderr, "STOPPING\n");
		Walking::GetInstance()->Stop();
		while(Walking::GetInstance()->IsRunning() == 1) usleep(8000);
		Walking::GetInstance()->m_Joint.SetEnableBody(false);
		Head::GetInstance()->m_Joint.SetEnableBody(false);
		Action::GetInstance()->m_Joint.SetEnableBody(true);
		MotionManager::GetInstance()->SetEnable(true);
	}
}



/*****************************************************************************************
* Method Name: freeAllEngines
* Description: free all engines for exiting
* Arguments: None
* Return Values: none
* Wrote: YanivS & AsafB
*****************************************************************************************/
void freeAllEngines(Robot::CM730 *cm730)
{
		char key;
		printf("Do you want to free all the engines? [y/n] \n");
		cin >> key ;
		if(key == 'y' || key == 'Y')
			cm730->WriteByte(CM730::ID_CM, MX28::P_TORQUE_ENABLE, 0, 0);

}

/*****************************************************************************************
* Method Name: convertIntToString
* Description: convert int to string (for counter to print in logfile)
* Arguments: None
* Return Values: none
* Wrote: YanivS & AsafB
*****************************************************************************************/
string convertIntToString(int number){
	ostringstream convert;
	convert << number;
	return convert.str();
}

string convertDoubleToString(double number){
	ostringstream convert;
	convert << number;
	return convert.str();
}




int compass_Gyro(CM730 *cm730, int id, double & current_angel)
{
	unsigned char table[128];
	int addr;
	int value_Gyro_X,value_Gyro_Y,value_Gyro_Z;//value_ACC_X,value_ACC_Y,value_ACC_Z;
	int gyroValue = 0;
	double integreted_rate=current_angel;
	writeToLog(logFile,"integreted_rate before: ",convertDoubleToString(integreted_rate));
    double angle = 0;
	double gyroRate = 0;
	//The conversion graph for Gyrorate DPS is: Gyrorate=MAX_DPS/OFFSET*GyroValue-MAX_DPS
	int const max_dps=1600;
	int const offset=512;


//	while(1)
//	{
//		if(id == CM730::ID_CM) // Sub board
//		{
//			if(cm730 -> ReadWord(id,CM730::P_GYRO_X_L,&value_Gyro_X,0) != CM730::SUCCESS)
//			{
//				printf(" Can not read table!\n");
//				return(-1);
//			}
//			if(cm730 -> ReadWord(id,CM730::P_GYRO_Y_L,&value_Gyro_Y,0) != CM730::SUCCESS)
//						{
//							printf(" Can not read table!\n");
//							return(-1);
//						}
//			if(cm730 -> ReadWord(id,CM730::P_GYRO_Z_L,&value_Gyro_Z,0) != CM730::SUCCESS)
//						{
//							printf(" Can not read table!\n");
//							return(-1);
//						}
//
//			printf( "\n" );
//			printf( " [EEPROM AREA]\n" );
//
//		    gyroValue = value_Gyro_Z;
//		    printf("gyroValueX: %d\n",gyroValue);
//		    printf("gyroValueY: %d\n",value_Gyro_Y);
//		    printf("gyroValueZ: %d\n",value_Gyro_Z);
//		    gyroRate = (float(gyroValue*max_dps)/offset - max_dps) / 3.2; // Sensitivity from data-sheet
//		    printf("gyroRate:  %f\n",gyroRate);
//		    integreted_rate = integreted_rate + (gyroRate / 80.0);
//		    if (integreted_rate < 0){
//		    	integreted_rate += 360;
//		    }
//		    int div = integreted_rate/360;
//			angle = integreted_rate - div*360;
//		    //angle = integreted_rate/360
//		    printf("angle:     %f\n",angle);
//		    printf("%5f\n",angle);
//		    printf("\n");
//
//		    sleep(0.001);
//		}
//	}
//	return(0);

	if (cm730->ReadWord(id, CM730::P_GYRO_X_L, &value_Gyro_X, 0)
			!= CM730::SUCCESS) {
		printf(" Can not read table!\n");
		return (-1);
	}
	if (cm730->ReadWord(id, CM730::P_GYRO_Y_L, &value_Gyro_Y, 0)
			!= CM730::SUCCESS) {
		printf(" Can not read table!\n");
		return (-1);
	}
	if (cm730->ReadWord(id, CM730::P_GYRO_Z_L, &value_Gyro_Z, 0)
			!= CM730::SUCCESS) {
		printf(" Can not read table!\n");
		return (-1);
	}

	printf("\n");
	printf(" [EEPROM AREA]\n");

	printf("gyroValueZ: %d\n", value_Gyro_Z);
	printf("gyroValueY: %d\n", value_Gyro_Y);
	printf("gyroValueX: %d\n", value_Gyro_X);

	//Summing Z-axis
	gyroValue = value_Gyro_Z;
	writeToLog(logFile,"gyroValueZ:      ",convertIntToString(gyroValue));
	gyroRate = (double(gyroValue * max_dps) / offset - max_dps) / 3.2; // Sensitivity from data-sheet
	integreted_rate = integreted_rate + gyroRate / 66.0 ;
	printf("angle:     %f\n", integreted_rate);
	writeToLog(logFile,"integreted_rate:  ",convertDoubleToString(integreted_rate));
	writeToLog(logFile,"\n");
	current_angel = integreted_rate;




	sleep(0.001);
	return 0;
}



