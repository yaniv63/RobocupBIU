#include "../State.h"


class GoToBall : public State
{
	public:
	GoToBall();
	virtual ~GoToBall();

	virtual void Run();

	private:
	void turnToBall(float angleToBall);
	const float factor = 1481.0;
};

