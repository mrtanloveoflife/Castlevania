#pragma once
#include "GameObject.h"
#include "Background.h"
#include "Brick.h"
#include "Enemy.h"

#include <vector>

#define THORN_STATE_010	0
#define THORN_STATE_19	1
#define THORN_STATE_28	2
#define THORN_STATE_37	3
#define THORN_STATE_46	4
#define THORN_STATE_5	5

#define THORN_BBOX_WIDTH  31
#define THORN_BBOX_HEIGHT_010 26
#define THORN_BBOX_HEIGHT_19 34
#define THORN_BBOX_HEIGHT_28 42
#define THORN_BBOX_HEIGHT_37 50
#define THORN_BBOX_HEIGHT_46 62
#define THORN_BBOX_HEIGHT_5 70

#define THORN_SPRITES_PATH	"Objects\\Thorn Sprites.txt"

class Thorn: public CGameObject, public Enemy
{
	int z;
public:
	static void LoadThornTextures(int ID_TEX_THORN, LPCWSTR THORN_TEXTURE_PATH, D3DCOLOR THORN_TEXTURE_BACKGROUND_COLOR);
	Thorn(float x, float y, int z);
	virtual void Render();
	virtual void Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual bool DoesExist();
};

