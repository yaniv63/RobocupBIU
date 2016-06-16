#include "../State.h"

class LookForBall : public State
{
	public:
	LookForBall();
	virtual ~LookForBall();

	virtual void Run();
	void CenterBall();

	float angleToBall;

	private:
	LookDirection m_lookDirection;
};
