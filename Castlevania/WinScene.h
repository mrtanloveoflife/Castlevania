#pragma once
#include "GameObject.h"
#include "Textures.h"

#include <string>

#define WINSCENE_SPRITES_PATH "Backgrounds\\Win Scene Sprites.txt"

class WinScene : public CGameObject 
{
	static WinScene *__instance;

	int score;
public:
	static WinScene *GetInstance(int ID_TEX_WIN_SCENE, LPCWSTR WIN_SCENE_TEXTURE_PATH, D3DCOLOR WIN_SCENE_TEXTURE_BACKGROUND_COLOR);
	static WinScene *GetInstance();

	WinScene(int ID_TEX_WIN_SCENE, LPCWSTR WIN_SCENE_TEXTURE_PATH, D3DCOLOR WIN_SCENE_TEXTURE_BACKGROUND_COLOR);
	void SetScore(int score) { this->score = score; }

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) {};
	virtual void Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL) {};
	virtual void Render();
	~WinScene();
};

