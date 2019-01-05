#pragma once
#include "GameObject.h"

class Brick:public CGameObject
{
	float left, top, right, bottom;

public:
	Brick(float left, float top, float right, float bottom);

	virtual void Render() {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects) {};
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};