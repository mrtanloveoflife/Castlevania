#pragma once
#include "GameObject.h"
#include "Enemy.h"
#include "Camera.h"

#define PHANTOMBAT_STATE_IDLE	0
#define PHANTOMBAT_STATE_ACTIVE	1

#define PHANTOMBAT_BBOX_WIDTH	48
#define PHANTOMBAT_BBOX_HEIGHT	23

class PhantomBat: public CGameObject, public Enemy
{
	static PhantomBat *__instance;

	int movePhase;
	float desX, desY;

public:
	static PhantomBat *GetInstance();
	static PhantomBat *GetInstance(float x, float y);

	PhantomBat();
	PhantomBat(float x, float y);

	void SetState(int state);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL);
	virtual void Render();
	virtual bool DoesExist();

	~PhantomBat();
};