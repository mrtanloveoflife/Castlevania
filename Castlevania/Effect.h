#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "Camera.h"

#define EFFECT_SPEED_X_TYPE_0 0.05f
#define EFFECT_SPEED_Y_TYPE_0 -0.25f

#define EFFECT_SPEED_X_TYPE_1 0.05f
#define EFFECT_SPEED_Y_TYPE_1 -0.2f

#define EFFECT_SPEED_X_TYPE_2 -0.05f
#define EFFECT_SPEED_Y_TYPE_2 -0.25f

#define EFFECT_SPEED_X_TYPE_3 -0.05f
#define EFFECT_SPEED_Y_TYPE_3 -0.2f

#define EFFECT_SPEED_X_TYPE_4 0.0f
#define EFFECT_SPEED_Y_TYPE_4 0.0f

#define EFFECT_WATER_BBOX_WIDTH		7
#define EFFECT_WATER_BBOX_HEIGHT	14
#define EFFECT_BRICK_BBOX_WIDTH		7
#define EFFECT_BRICK_BBOX_HEIGHT	8

#define EFFECT_TYPE_WATER		0
#define EFFECT_TYPE_BRICK		1
#define EFFECT_TYPE_DESTROYED	2

#define EFFECT_SPRITES_PATH		"Objects\\Effect Sprites.txt"

class Effect: public CGameObject
{
	int type;
	DWORD existTime;
public:
	
	static void LoadEffectTextures(int ID_TEX_EFFECTS, LPCWSTR EFFECTS_TEXTURE_PATH, D3DCOLOR EFFECTS_TEXTURE_BACKGROUND_COLOR);

	Effect(float x, float y, int type, int vType);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) {};
	virtual void Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL);
	virtual void Render();

	bool DoesExist();

	~Effect();
};

