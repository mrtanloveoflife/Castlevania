#pragma once

#include "GameObject.h"
#include "Simon.h"
#include "Enemy.h"

#define HEAD_SPEED_X 0.05f
#define HEAD_SPEED_Y 0.085f

#define HEAD_BBOX_WIDTH 15
#define HEAD_BBOX_HEIGHT 16

class Head : public CGameObject, public Enemy
{
	static DWORD spawTime;
	DWORD changeVyTime;

public:
	static int SpawnPos();

	Head(float x, float y, int nx);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL);
	virtual void Render();
	virtual bool DoesExist();
};