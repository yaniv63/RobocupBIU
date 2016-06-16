/*
 * axis_angle.h
 *
 *  Created on: Apr 14, 2016
 *      Author: root
 */

#include "../BrainAndMotion.h"


#ifndef SRC_AXIS_ANGLE_H_
#define SRC_AXIS_ANGLE_H_

class axis_angle {
public:
	axis_angle(string _name,CM730* cm,int memory_adress,float delta_t);
	double get_angle(){return angle;}
	void  update_angle();
private:
	string name;
	double angle;
	const int mem_adress;
	CM730* cm730 ;
	int current_mem_value;


	void  read_gyro_value();
	double   gyro_value_to_dps();

	const int max_dps;
	const int offset;
	const float  gyro_sensitivity;
	const float dt;

};




#endif /* SRC_AXIS_ANGLE_H_ */
