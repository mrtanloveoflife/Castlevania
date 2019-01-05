#pragma once
#include "Background.h"
#include "Brick.h"

#include <vector>

#define CANDLE_TYPE_BIG 0
#define CANDLE_TYPE_SMALL 1

#define CANDLE_BIG_BBOX_WIDTH  16
#define CANDLE_BIG_BBOX_HEIGHT 32
#define CANDLE_SMALL_BBOX_WIDTH 8
#define CANDLE_SMALL_BBOX_HEIGHT 16

#define CANDLE_SPRITES_PATH		"Objects\\Candle Sprites.txt"

class Candle : public CGameObject
{
	int type;
	int itemType;

public:
	static void LoadCandleTextures(int ID_TEX_CANDLES, LPCWSTR CANDLES_TEXTURE_PATH, D3DCOLOR CANDLES_TEXTURE_BACKGROUND_COLOR);
	Candle(float x, float y, int type, int itemType);
	int GetType() { return type; }
	int GetItemType() { return itemType; }
	void SetItemType(int itemType) { this->itemType = itemType; }

	virtual void Render();
	virtual void Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};