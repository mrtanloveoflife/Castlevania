#pragma once
#include "GameObject.h"
#include "Enemy.h"
#include "Simon.h"
#include "Projectile.h"

#define SNAKEHEAD_BBOX_WIDTH		16
#define SNAKEHEAD_BBOX_HEIGHT		32

#define SNAKEHEAD_RIGHT				0
#define SNAKEHEAD_LEFT				1

class SnakeHead : public CGameObject, public Enemy
{
	DWORD startAttackingTime;

public:
	SnakeHead(float x, float y);
	void ChoosingAttackingTime();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL);
	virtual void Render();
	virtual bool DoesExist();

	~SnakeHead();
};