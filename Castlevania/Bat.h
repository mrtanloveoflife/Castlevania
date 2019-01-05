#pragma once

#include "GameObject.h"
#include "Simon.h"
#include "Enemy.h"

#define BAT_SPEED_X 0.05f
#define BAT_SPEED_Y 0.03f

#define BAT_BBOX_WIDTH 16
#define BAT_BBOX_HEIGHT 22

class Bat: public CGameObject, public Enemy
{
	static DWORD spawTime;
	DWORD changeVyTime;

public:
	static int SpawnPos();
	
	Bat(float x, int ymin, int ymax, int nx);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL);
	virtual void Render();
	virtual bool DoesExist();
};