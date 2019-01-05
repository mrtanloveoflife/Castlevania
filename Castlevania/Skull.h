#pragma once

#include "GameObject.h"
#include "Simon.h"
#include "Enemy.h"

#define SKULL_SPEED_X 0.04f
#define SKULL_SPEED_Y 0.007f

#define SKULL_BBOX_WIDTH 16
#define SKULL_BBOX_HEIGHT 16

class Skull : public CGameObject, public Enemy
{

public:

	Skull(float x, float y);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL);
	virtual void Render();
	virtual bool DoesExist();
};