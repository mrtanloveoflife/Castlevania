#pragma once

#include "GameObject.h"
#include "Simon.h"
#include "Enemy.h"

#define BAT2_FLY_SPEED_X 0.075f
#define BAT2_FLY_SPEED_Y 0.05f

#define BAT2_BBOX_WIDTH 16
#define BAT2_BBOX_HEIGHT 22

#define BAT2_STATE_IDLE			0
#define BAT2_STATE_FLY_RIGHT	1
#define BAT2_STATE_FLY_LEFT		2

class Bat2 : public CGameObject, public Enemy
{
	bool isActive;

public:

	Bat2(float x, float y);
	void SetState(int state);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL);
	virtual void Render();
	virtual bool DoesExist();
};