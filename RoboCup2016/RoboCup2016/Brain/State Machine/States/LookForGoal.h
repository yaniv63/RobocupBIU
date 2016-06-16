#include "../State.h"

class LookForGoal : public State
{
	public:
	LookForGoal();
	virtual ~LookForGoal();

	virtual void Run();

	private:
	LookDirection m_lookDirection;
};
