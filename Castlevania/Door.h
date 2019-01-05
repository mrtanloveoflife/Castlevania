#pragma once
#include "GameObject.h"
#include "Camera.h"

#define DOOR_BBOX_WIDTH		8
#define DOOR_BBOX_HEIGHT	32

#define DOOR_STATE_CLOSED	0
#define DOOR_STATE_OPENING	1
#define DOOR_STATE_OPENED	2
#define DOOR_STATE_CLOSING	3

#define DOOR_SPRITES_PATH		"Objects\\Door Sprites.txt"

class Door :public CGameObject
{
	bool isUsed;
public:
	static void LoadDoorTextures(int ID_TEX_DOORS, LPCWSTR DOORS_TEXTURE_PATH, D3DCOLOR DOORS_TEXTURE_BACKGROUND_COLOR);

	Door(float x, float y, int nx);
	bool GetIsUsed() { return isUsed; }
	void SetIsUsed(bool value) { isUsed = value; }

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL) {};
	virtual void Render();

	~Door() {};
};