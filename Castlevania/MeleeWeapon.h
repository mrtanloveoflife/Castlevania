#pragma once
#include "Textures.h"
#include "Candle.h"
#include "Simon.h"
#include "Item.h"
#include "Zombie.h"
#include "Bat.h"
#include "Panther.h"
#include "Effect.h"
#include "Sound.h"

#define MELEE_WEAPON_TYPE_0_BBOX_WIDTH  23
#define MELEE_WEAPON_TYPE_0_BBOX_HEIGHT 8
#define MELEE_WEAPON_TYPE_1_BBOX_WIDTH  23
#define MELEE_WEAPON_TYPE_1_BBOX_HEIGHT 6
#define MELEE_WEAPON_TYPE_2_BBOX_WIDTH  44
#define MELEE_WEAPON_TYPE_2_BBOX_HEIGHT 6

#define SOUND_HIT "Sounds\\sound\\hit.wav"

class MeleeWeapon : public CGameObject
{
	static MeleeWeapon * __instance;
	CTextures * textures;
	int ID_TEX;
	int type;
	DWORD attackingTime;
	DWORD startAttackTime;
	Sound *hitSound;

public:
	static MeleeWeapon * GetInstance();
	static MeleeWeapon * GetInstance(int ID_TEX_MELEE_WEAPONS, LPCWSTR MELEE_WEAPONS_TEXTURE_PATH, D3DCOLOR MELEE_WEAPONS_TEXTURE_BACKGROUND_COLOR);

	MeleeWeapon(int ID_TEX_MELEE_WEAPONS, LPCWSTR MELEE_WEAPONS_TEXTURE_PATH, D3DCOLOR MELEE_WEAPONS_TEXTURE_BACKGROUND_COLOR);
	void LoadWeaponTextures();
	void LoadWeapon();
	void Upgrade();
	int GetType() { return type; }
	void ResetStartAttackingTime() { startAttackTime = GetTickCount(); }

	virtual void Render();
	virtual void Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *colliable_objects = NULL);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};