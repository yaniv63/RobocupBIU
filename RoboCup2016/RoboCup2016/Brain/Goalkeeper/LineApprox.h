/*
 * LineApprox.h
 *
 *  Created on: Jun 20, 2016
 *      Author: root
 */

#ifndef BRAIN_GOALKEEPER_LINEAPPROX_H_
#define BRAIN_GOALKEEPER_LINEAPPROX_H_

class LineApprox {
public:
	LineApprox(float C, float D);
	virtual ~LineApprox();

	float GetFit(float t);

private:
	float m_C;
	float m_D;
};

#endif /* BRAIN_GOALKEEPER_LINEAPPROX_H_ */
