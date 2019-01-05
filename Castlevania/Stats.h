#pragma once
#include "Simon.h"
#include "Medusa.h"

#include <string>

class Stats :public CGameObject
{
	static Stats *__instance;
	int simon_health, boss_health, score, timer, stage, heart, rangedType, maximumThrows;
	DWORD tickTime;
public:
	static Stats *GetInstance(int ID_TEX_STATS, LPCWSTR STATS_TEXTURE_PATH, D3DCOLOR STATS_TEXTURE_BACKGROUND_COLOR);
	static Stats *GetInstance();

	Stats(int ID_TEX_STATS, LPCWSTR STATS_TEXTURE_PATH, D3DCOLOR STATS_TEXTURE_BACKGROUND_COLOR);
	void Update(int current_Stage);
	void SetTimer(int value) { timer = value; }
	int GetTimer() { return timer; }
	int GetHeart() { return heart; }
	void IncreaseHeart(int value) { heart += value; }
	void ReduceHeart() { heart--; }
	void IncreaseScore(int value) { score += value; }
	int GetScore() { return score; }

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) {};
	virtual void Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *Objects = NULL) {};
	virtual void Render();

	~Stats();
};