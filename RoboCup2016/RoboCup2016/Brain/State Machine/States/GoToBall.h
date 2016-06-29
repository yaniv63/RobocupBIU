#include "../State.h"


class GoToBall : public State
{
	public:
	GoToBall();
	virtual ~GoToBall();

	virtual void Run();

	const double DISTANCE_WHEN_LOSING_BALL = 21; //cm
};

