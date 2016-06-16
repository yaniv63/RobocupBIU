/*
 * Gyro.h
 *
 *  Created on: Apr 14, 2016
 *      Author: root
 */

#ifndef LOCALIZATION_GYRO_GYRO_H_
#define LOCALIZATION_GYRO_GYRO_H_

#include <thread>
#include "../BrainAndMotion.h"
#include "axis_angle.h"

class Gyro {
public:
	~Gyro(){}
	Gyro(CM730* cm);
	//void RunThread();
	void stop(){run =false;gyro_thread.join();}
	double get_yaw(){return yaw_angle.get_angle(); }
	double get_roll(){return roll_angle.get_angle(); }
	double get_pitch(){return pitch_angle.get_angle(); }

private:
	void update_angles();
	const float integration_delta_t;
	bool run;
	std::thread gyro_thread;
	axis_angle roll_angle; //http://machinedesign.com/engineering-essentials/what-s-difference-between-pitch-roll-and-yaw
	axis_angle pitch_angle;
	axis_angle yaw_angle;

	/*int inline update_angle(int selector, int value_gyro);
	void inline read_gyro_value(int selector, int& value_Gyro);
	double inline gyro_value_to_dps(double const & value);
	CM730* cm730 ;
	int const max_dps;
	int const offset;
	float const gyro_sensitivity;
	double angle_z;	//integrated angle of z till now
	double angle_y;
	double angle_x;

	enum axis{
		X,Y,Z
	};*/

};

#endif /* LOCALIZATION_GYRO_GYRO_H_ */
