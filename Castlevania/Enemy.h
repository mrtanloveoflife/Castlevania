#pragma once
#include <Windows.h>
#include <fstream>

#include "Textures.h"
#include "Sprites.h"

#define ENEMY_SPRITES_PATH	"Enemies\\Enemy Sprites.txt"
#define BOSS_SPRITES_PATH	"Bosses\\Boss Sprites.txt"

class Enemy
{
protected:
	int health;
	DWORD startAttackingTime;
	DWORD startThrowingTime;
	bool isFreezing;
	DWORD freezingTime;
	bool onScreen;
	static bool isFreezingAll;
	static DWORD freezingTimeAll;

public:
	static void LoadEnemyTextures(int ID_TEX_ENEMIES, LPCWSTR ENEMIES_TEXTURE_PATH, D3DCOLOR ENEMIES_TEXTURE_BACKGROUND_COLOR);
	static void LoadBossTextures(int ID_TEX_BOSSES, LPCWSTR BOSSES_TEXTURE_PATH, D3DCOLOR BOSSES_TEXTURE_BACKGROUND_COLOR);
	static void StartFreezingAll(int time);

	Enemy();

	void ReduceHealth(int value, DWORD startAttackingTime);
	void ReduceHealth(DWORD startThrowingTime);
	int GetHealth() { return health; }
	void StartFreezing(int time);

	virtual bool DoesExist() = 0;
	~Enemy();
};

typedef Enemy * LPENEMY;