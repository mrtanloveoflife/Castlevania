#pragma once
#include "GameObject.h"
#include "Simon.h"

#define PANTHER_RUN_SPEED_X 0.2f
#define PANTHER_JUMP_SPEED_Y 0.22f

#define PANTHER_BBOX_WIDTH	28
#define PANTHER_BBOX_HEIGHT 16

#define PANTHER_STATE_IDLE_LEFT		0
#define PANTHER_STATE_JUMP_RIGHT	1
#define PANTHER_STATE_JUMP_LEFT		2
#define PANTHER_STATE_RUN_RIGHT		3
#define PANTHER_STATE_RUN_LEFT		4

class Panther :public CGameObject, public Enemy
{
	bool isActive;

public:

	Panther(float x, float y);
	void SetState(int state);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL);
	virtual void Render();
	virtual bool DoesExist();
};