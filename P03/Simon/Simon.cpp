#include "Simon.h"

CSimon::CSimon() :CGameObject::CGameObject()
{
	lastState = 0;
	isHitting = 0;
	sit = 0;
}

void CSimon::Update(DWORD dt)
{
	CGameObject::Update(dt);

	// simple fall down
	vy += SIMON_GRAVITY;
	if (y > 100)
	{
		vy = 0; y = 100.0f;
	}

	// simple screen edge collision!!!
	if (vx > 0 && x > 290) x = 290;
	if (vx < 0 && x < 0) x = 0;
}

void CSimon::Render()
{
	int ani;
	if (isHitting)
	{
		if ((this->sit) == 0)
		{
			if (nx > 0) ani = SIMON_ANI_STAND_HIT_RIGHT;
			else ani = SIMON_ANI_STAND_HIT_LEFT;
		}
		else
		{
			if (nx > 0) ani = SIMON_ANI_SIT_HIT_RIGHT;
			else ani = SIMON_ANI_SIT_HIT_LEFT;
		}

	}
	else
	{
		if (vx == 0)
		{
			if (this->sit == 0) //nhân vật không ngồi
			{
				if (nx > 0) ani = SIMON_ANI_IDLE_RIGHT;
				else ani = SIMON_ANI_IDLE_LEFT;
			}
			else
			{
				if (nx > 0) ani = SIMON_ANI_SIT_RIGHT;
				else ani = SIMON_ANI_SIT_LEFT;
			}

		}
		else if (vx > 0)
			ani = SIMON_ANI_WALKING_RIGHT;
		else ani = SIMON_ANI_WALKING_LEFT;
	}

	animations[ani]->Render(x, y, isHitting);
}

void CSimon::SetState(int state)
{
	if (isHitting) return;
	CGameObject::SetState(state);
	lastState = state;
	sit = 0;
	//hit = 0;
	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_JUMP:
		if (y == 100)
			vy = -SIMON_JUMP_SPEED_Y;
		break;
	case SIMON_STATE_IDLE:
		vx = 0;
		break;
	case SIMON_STATE_STAND_HIT_RIGHT:
	case SIMON_STATE_STAND_HIT_LEFT:
		isHitting = 1;
		break;
	case SIMON_STATE_SIT_RIGHT:
	case SIMON_STATE_SIT_LEFT:
		sit = 1;
		vx = 0;
		break;
	case SIMON_STATE_SIT_HIT_RIGHT:
	case SIMON_STATE_SIT_HIT_LEFT:
		sit = 1;
		vx = 0;
		isHitting = 1;
		break;

	}
}

