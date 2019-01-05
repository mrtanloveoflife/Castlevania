#pragma once
#include "GameObject.h"
#include "Simon.h"
#include "Enemy.h"

#define SOLDIER_SPEED_X 0.05f

#define SOLDIER_BBOX_WIDTH 16
#define SOLDIER_BBOX_HEIGHT 32

class Soldier: public CGameObject, public Enemy
{
	float xmin, xmax;
public:

	Soldier(float x, float y,float xmin, float xmax);
	void SetMaxMin(float xmin, float xmax);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL);
	virtual void Render();
	virtual bool DoesExist();
};

