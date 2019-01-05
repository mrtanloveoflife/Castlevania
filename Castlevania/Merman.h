#pragma once
#include "GameObject.h"
#include "Enemy.h"
#include "Simon.h"
#include "Projectile.h"

#define MERMAN_WALK_SPEED_X	0.05f
#define MERMAN_JUMP_SPPED_Y 0.5f

#define MERMAN_STATE_JUMP_RIGHT		0
#define MERMAN_STATE_JUMP_LEFT		1
#define MERMAN_STATE_WALK_RIGHT		2
#define MERMAN_STATE_WALK_LEFT		3
#define MERMAN_STATE_ATTACK_RIGHT	4
#define MERMAN_STATE_ATTACK_LEFT	5

#define MERMAN_WALKING_BBOX_WIDTH		16
#define MERMAN_WALKING_BBOX_HEIGHT		32
#define MERMAN_ATTACKING_BBOX_WIDTH		16
#define MERMAN_ATTACKING_BBOX_HEIGHT	30

class Merman : public CGameObject, public Enemy
{
	static DWORD spawTime;
	
	bool isAttacking;
	DWORD startAttackingTime;

public:
	static int SpawnPos();

	Merman();
	void SetState(int state);
	bool IsOnScreen() { return onScreen; }
	void ChoosingAttackingTime();

	void Brickcollision(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL);
	virtual void Render();
	virtual bool DoesExist();

	~Merman();
};