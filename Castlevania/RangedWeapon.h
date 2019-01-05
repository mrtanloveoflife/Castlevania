#pragma once
#include "Simon.h"
#include "Textures.h"
#include "Sound.h"

#define DAGGER_BBOX_WIDTH					17
#define DAGGER_BBOX_HEIGHT					9
#define HOLY_WATER_IN_POTION_BBOX_WIDTH		10
#define HOLY_WATER_IN_POTION_BBOX_HEIGHT	10
#define HOLY_WATER_ON_FIRE_BBOX_WIDTH		16
#define HOLY_WATER_ON_FIRE_BBOX_HEIGHT		13
#define AXE_BBOX_WIDTH						15
#define AXE_BBOX_HEIGHT						14
#define BOOMERANG_BBOX_WIDTH				15
#define BOOMERANG_BBOX_HEIGHT				14

#define THROWING_AXE_SPEED_X	0.15f
#define THROWING_AXE_SPEED_Y	0.3f
#define THROWING_SPEED_X		0.2f
#define THROWING_SPEED_Y		0.2f
#define TURNING_BACK_SPEED_X	0.0008f

#define ID_RANGED_WEAPON_NONE					0
#define ID_RANGED_WEAPON_DAGGER					1
#define ID_RANGED_WEAPON_HOLY_WATER_IN_POTION	2
#define ID_RANGED_WEAPON_HOLY_WATER_ON_FIRE		3
#define ID_RANGED_WEAPON_AXE					4
#define ID_RANGED_WEAPON_BOOMERANG				5

#define ID_RANGED_WEAPON_STOPWATCH				6 // Put this at the end of the list so we don't need to add sprites for it

#define SOUND_HOLY_WATER	"Sounds\\sound\\holywater.wav"
#define RANGED_WEAPON_SPRITES_PATH "Objects\\RangedWeapon Sprites.txt"

class RangedWeapon : public CGameObject
{
	int type;
	DWORD FireTime;
	DWORD startThrowingTime;
	bool didChangeDirection;
	bool didResetTime;
	Sound *hitSound, *holyWaterSound;
	bool doesExist;

public:
	static void LoadRangedWeaponTexture(int ID_TEX_RANGED_WEAPONS, LPCWSTR RANGED_WEAPONS_TEXTURE_PATH, D3DCOLOR RANGED_WEAPONS_TEXTURE_BACKGROUND_COLOR);

	RangedWeapon(int type);
	void LoadWeaponSpeed();
	void SetType(int type) { this->type = type; LoadWeaponSpeed(); }
	int GetType() { return type; }
	bool GetDoesExist() { return doesExist; }

	void BricksCollision(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL);
	void DestroyObjectsCollision(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL);
	bool SimonCollision(DWORD dt);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL);
	virtual void Render();
};