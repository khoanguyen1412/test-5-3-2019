#pragma once
#include "GameObject.h"

#define SIMON_WALKING_SPEED		0.1f
#define SIMON_JUMP_SPEED_Y		0.7f
#define SIMON_GRAVITY			0.07f

#define SIMON_STATE_IDLE			0
#define SIMON_STATE_WALKING_RIGHT	1
#define SIMON_STATE_WALKING_LEFT	2
#define SIMON_STATE_JUMP			3
#define SIMON_STATE_SIT_RIGHT       4
#define SIMON_STATE_SIT_LEFT		5
#define SIMON_STATE_STAND_HIT_RIGHT 6
#define SIMON_STATE_STAND_HIT_LEFT	7
#define SIMON_STATE_SIT_HIT_RIGHT	8
#define SIMON_STATE_SIT_HIT_LEFT	9

#define SIMON_ANI_IDLE_RIGHT		0
#define SIMON_ANI_IDLE_LEFT			1
#define SIMON_ANI_WALKING_RIGHT		2
#define SIMON_ANI_WALKING_LEFT		3
#define SIMON_ANI_SIT_RIGHT			4
#define SIMON_ANI_SIT_LEFT			5
#define SIMON_ANI_STAND_HIT_RIGHT	6
#define SIMON_ANI_STAND_HIT_LEFT	7
#define SIMON_ANI_SIT_HIT_RIGHT		8
#define SIMON_ANI_SIT_HIT_LEFT		9

class CSimon :public CGameObject
{

protected:
	int sit;

public:

	CSimon();
	int isHitting;
	int lastState;
	void Update(DWORD dt);
	void Render();
	void SetState(int state);
};