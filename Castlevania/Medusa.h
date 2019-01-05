#pragma once
#include "GameObject.h"
#include "Enemy.h"
#include "Camera.h"
#include "CassiopeiaMini.h"

#define MEDUSA_STATE_IDLE	0
#define MEDUSA_STATE_ACTIVE	1

#define MEDUSA_BBOX_WIDTH	32
#define MEDUSA_BBOX_HEIGHT	32

#define MEDUSA_SPEED_X		0.03f
#define MEDUSA_SPEED_Y		0.03f


class Medusa : public CGameObject, public Enemy
{
	static Medusa *__instance;

	int movePhase;
	float desX, desY;
	DWORD changeVyTime;
	DWORD changePhase;
	DWORD startAttackingTime;


public:
	static Medusa *GetInstance();
	static Medusa *GetInstance(float x, float y);
	bool direction;
	Medusa(float x, float y);

	void SetState(int state);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL);
	virtual void Render();
	virtual bool DoesExist();

	~Medusa();
};