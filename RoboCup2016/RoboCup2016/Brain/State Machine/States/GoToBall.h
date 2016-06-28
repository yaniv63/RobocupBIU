#include "../State.h"


class GoToBall : public State
{
	public:
	GoToBall();
	virtual ~GoToBall();

	virtual void Run();

	const double DISTANCE_WHEN_LOSING_BALL = 21; //cm

	private:
	void turnToBall(float angleToBall);
	const float factor = 1388.89;
};

