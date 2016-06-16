/*

 * axis_angle.cpp

 *
 *  Created on: Apr 14, 2016
 *      Author: root
 */

#include "axis_angle.h"
#include <iomanip>
#include <iostream>

//TODO:inlined!!
axis_angle::axis_angle(string _name,CM730* cm,int memory_adress,float delta_t):max_dps(1600)
																			  ,offset(512)
																			  ,angle(0)
																			  ,gyro_sensitivity(3.2)
																			  ,dt(delta_t)
																			  ,mem_adress(memory_adress)
{
	cm730 = cm;
	current_mem_value = 0;
	name = _name;
}

void   axis_angle::update_angle(){
	read_gyro_value();
	angle += gyro_value_to_dps()/dt ;
}

void  axis_angle::read_gyro_value(){
	if(cm730->ReadWord(CM730::ID_CM, mem_adress, &current_mem_value, 0) != CM730::SUCCESS)
		cout << "couldn't read gyro value from controller" << endl;
}

double  axis_angle::gyro_value_to_dps(){
	return  (double(current_mem_value * max_dps) / offset - max_dps) / gyro_sensitivity; // Sensitivity from data-sheet
}





