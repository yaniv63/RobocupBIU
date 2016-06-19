#define TEAM_NUMBER 2
#define PLAYER_NUMBER 1
#define TEAM_COLOUR 0

#include "RoboCupGameControlData.h"

struct PlayerInfo
{
	const uint8_t teamNumber;
	const uint8_t teamColour;
	const uint8_t playerNumber;
	uint8_t penalty;              //penalty state of the player
	PlayerInfo():teamNumber(TEAM_NUMBER),teamColour(TEAM_COLOUR),playerNumber(PLAYER_NUMBER),penalty(PENALTY_NONE)
	{

	};
};
