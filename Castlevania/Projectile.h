#pragma once
#include "GameObject.h"
#include "Enemy.h"
#include "Camera.h"

#define PROJECTILE_SPEED_X 0.1f

#define PROJECTILE_BBOX_WIDTH	7
#define PROJECTILE_BBOX_HEIGHT	6

#define  PROJECTILE_STATE_RIGHT	0
#define  PROJECTILE_STATE_LEFT	1

class Projectile: public CGameObject, public Enemy
{
public:
	Projectile(float x, float y, int nx);

	void SetState(int state);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL);
	virtual void Render();
	virtual bool DoesExist();

	~Projectile();
};

