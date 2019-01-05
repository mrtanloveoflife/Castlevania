#pragma once
#include <cstdlib>
#include <ctime>

#include "Brick.h"
#include "Textures.h"
#include "Camera.h"
#include "Enemy.h"

#define ZOMBIE_SPEED 0.07f

#define ZOMBIE_BBOX_WIDTH 16
#define ZOMBIE_BBOX_HEIGHT 32

class Zombie :public CGameObject, public Enemy
{
	static DWORD spawTime;

public:
	static int SpawnPos();

	Zombie(float x, float y, int nx);
	bool IsOnScreen() { return onScreen; }

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL);
	virtual void Render();
	virtual bool DoesExist();
};