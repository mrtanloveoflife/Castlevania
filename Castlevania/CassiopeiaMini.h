#pragma once
#include "GameObject.h"
#include "Enemy.h"
#include "Camera.h"

#define CASSIOPEIAMINI_SPEED_X 0.1f

#define CASSIOPEIAMINI_BBOX_WIDTH	18
#define CASSIOPEIAMINI_BBOX_HEIGHT	11

#define  CASSIOPEIAMINI_STATE_RIGHT	1
#define  CASSIOPEIAMINI_STATE_LEFT	0

class CassiopeiaMini : public CGameObject, public Enemy
{
public:
	CassiopeiaMini(float x, float y, int state);

	void SetState(int state);
	void Brickcollision(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL);
	virtual void Render();
	virtual bool DoesExist();

	~CassiopeiaMini();
};

