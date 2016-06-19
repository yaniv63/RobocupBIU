#include "../State.h"

class LookForBall : public State
{
	public:
	LookForBall();
	virtual ~LookForBall();

	virtual void Run();
	void CenterBall();

	private:
	LookDirection m_lookDirection;
};
