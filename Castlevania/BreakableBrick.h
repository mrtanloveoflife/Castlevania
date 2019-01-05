#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "Camera.h"

#define BREAKABLE_BRICK_BBOX_WIDTH	16
#define BREAKABLE_BRICK_BBOX_HEIGHT	16

#define  BREAKABLE_SPRITES_PATH		"Objects\\BreakableBrick Sprites.txt"

class BreakableBrick : public CGameObject
{
	int itemType;
public:
	static void LoadBreakableBrickTextures(int ID_TEX_BREAKABLE_BRICKS, LPCWSTR BREAKABLE_BRICKS_TEXTURE_PATH, D3DCOLOR BREAKABLE_BRICKS_TEXTURE_BACKGROUND_COLOR);

	BreakableBrick(float x, float y, int type, int itemType);
	int GetItemType() { return itemType; }

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL) {};
	virtual void Render();

	~BreakableBrick();
};

