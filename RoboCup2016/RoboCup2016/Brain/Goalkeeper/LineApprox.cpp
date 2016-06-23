/*
 * LineApprox.cpp
 *
 *  Created on: Jun 20, 2016
 *      Author: root
 */

#include "LineApprox.h"

LineApprox::LineApprox(float C, float D) {
	m_C = C;
	m_D = D;
}

LineApprox::~LineApprox() {
	// TODO Auto-generated destructor stub
}

float LineApprox::GetFit(float t)
{
	return m_C + m_D * t;
}
