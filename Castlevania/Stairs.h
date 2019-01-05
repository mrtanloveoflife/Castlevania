#pragma once
#include "GameObject.h"

#define STAIRS_STATE_UP_RIGHT	0
#define STAIRS_STATE_UP_LEFT	1

class Stairs :public CGameObject
{
	float uleft, utop, uright, ubottom; // bbox of the go-up stairs
	float dleft, dtop, dright, dbottom; // bbox of the go-down stairs
public:
	bool isUp;
	Stairs(int state, float uleft, float utop, float uright, float ubottom,
		float dleft, float dtop, float dright, float dbottom);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL) {};
	virtual void Render() {};
};