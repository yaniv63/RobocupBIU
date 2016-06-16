/*
 * Gyro.cpp
 *
 *  Created on: Apr 14, 2016
 *      Author: root
 */
#include "Gyro.h"




Gyro::Gyro(CM730* cm):gyro_thread(std::thread(&Gyro::update_angles,this))
					 ,integration_delta_t(75.0)
					 ,roll_angle("roll_angle",cm,CM730::P_GYRO_X_L,integration_delta_t)
					 ,pitch_angle("pitch_angle",cm,CM730::P_GYRO_Y_L,integration_delta_t)
					 ,yaw_angle("yaw_angle",cm,CM730::P_GYRO_Z_L,integration_delta_t)
{
	run = true;
}

void Gyro::update_angles(){
	sleep(15);
	int i=0;
	while(run){
		i++;
		roll_angle.update_angle();
		pitch_angle.update_angle();
		yaw_angle.update_angle();
		if (i==70){
			cout<<"roll: "<<roll_angle.get_angle()<<".  pitch: "<<pitch_angle.get_angle()<<".  yaw: "<<yaw_angle.get_angle()<<endl;
			i=0;
		}
		sleep(0.01);

	}
}




/*void Gyro::RunThread()
{
	pthread_t gyroThread;

		if (1 == pthread_create(&gyroThread, NULL,&Gyro::run_update_angles,(void *) NULL)) {
			fprintf(stderr, "Couldn't create gyro thread\n");
			exit(1);
		}

}*/

/*
Gyro::Gyro(CM730* cm):max_dps(1600),offset(512),angle_z(0),angle_x(0),angle_y(0),gyro_sensitivity(3.2) {
	cm730 = cm;
}
*/

/*void Gyro::update_angles(){

	int value_gyro_x,value_gyro_y,value_gyro_z;//value_ACC_X,value_ACC_Y,value_ACC_Z;
	double gyro_rate_z = 0;
	double gyro_rate_y = 0;
	double gyro_rate_x = 0;

	angle_x += update_angle(Gyro::X, value_gyro_x);
	angle_y += update_angle(Gyro::Y, value_gyro_y);
	angle_z += update_angle(Gyro::Z, value_gyro_z);
	cout << "angle X:" << angle_x << endl << "angle Y:" << angle_y << endl << "angle Z:" << angle_z << endl;

}

int inline Gyro::update_angle(int selector, int value_gyro){
	read_gyro_value(selector, value_gyro);
	int gyro_rate = gyro_value_to_dps(value_gyro);
	return (gyro_rate / 66.0);
}


void inline Gyro::read_gyro_value(int selector, int& value_Gyro){
	int ret;
	switch (selector){
		case Gyro::X:
			ret=cm730->ReadWord(CM730::ID_CM, CM730::P_GYRO_X_L, &value_Gyro, 0);
			break;
		case Gyro::Y:
			ret=cm730->ReadWord(CM730::ID_CM, CM730::P_GYRO_Y_L, &value_Gyro, 0);
			break;
		case Gyro::Z:
			ret=cm730->ReadWord(CM730::ID_CM, CM730::P_GYRO_Z_L, &value_Gyro, 0);
			break;
	}

	if (ret!=CM730::SUCCESS){
		cout << "couldn't read gyro value from controller" << endl;
	}
}

double inline Gyro::gyro_value_to_dps(double const & value){
	return  (double(value * max_dps) / offset - max_dps) / gyro_sensitivity; // Sensitivity from data-sheet
}

void* GyroActionAsync(void*)
{

}*/

