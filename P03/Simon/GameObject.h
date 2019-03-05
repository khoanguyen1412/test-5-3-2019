#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include "Sprites.h"



using namespace std;

class CGameObject
{
protected:
	//Tọa độ object
	float x;
	float y;

	//vận tốc
	float vx;
	float vy;

	//hướng object: >0: (phải) else (trái)
	int nx;


	int state;

	static vector<LPANIMATION> animations;

public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }

	void SetState(int state) { this->state = state; }
	int GetState() { return this->state; }
	int GetDirection() { return nx; }


	static void AddAnimation(int aniId);

	CGameObject();

	void Update(DWORD dt);
	void Render();
	~CGameObject();
};