#include "../State.h"


class GoToBall : public State
{
	public:
	GoToBall(float);
	virtual ~GoToBall();

	virtual void Run();

	private:
	float m_angleToBall;
};

