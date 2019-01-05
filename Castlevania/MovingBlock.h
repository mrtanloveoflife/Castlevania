#pragma once
#include"GameObject.h"
#include "Textures.h"
#include "Camera.h"

#define MOVINGBLOCK_BBOX_WIDTH 32
#define MOVINGBLOCK_BBOX_HEIGHT 7

#define SPEED_MOVING_BLOCK 0.035f

class MovingBlock: public CGameObject
{
	float xmin, xmax;
public:
	MovingBlock(float y, float xmin, float xmax);
	void SetMaxMin(float xmin, float xmax) { this->xmin = xmin; this->xmax = xmax; }
	static void LoadMovingBlockTextures(int ID_TEX_MOVINGBLOCK, LPCWSTR MOVINGBLOCK_TEXTURE_PATH, D3DCOLOR MOVINGBLOCK_TEXTURE_BACKGROUND_COLOR);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL);
	virtual void Render();
};

