#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include <fstream>

#include "Sprites.h"
#include "Textures.h"
#include "Camera.h"
#include "PhantomBat.h"

#define BACKGROUND_STAGE_0_TEXTURE_PATH		L"Backgrounds\\Intro Scene.png"
#define BACKGROUND_STAGE_1_TEXTURE_PATH		L"Backgrounds\\Stage 1 Source.png"
#define BACKGROUND_STAGE_1_INFO_PATH		L"Backgrounds\\Stage 1 Background Info.txt"
#define BACKGROUND_STAGE_2_TEXTURE_PATH		L"Backgrounds\\Stage 2 Source.png"
#define BACKGROUND_STAGE_2_INFO_PATH		L"Backgrounds\\Stage 2 Background Info.txt"
#define BACKGROUND_STAGE_3_TEXTURE_PATH		L"Backgrounds\\Stage 3 Source.png"
#define BACKGROUND_STAGE_3_INFO_PATH		L"Backgrounds\\Stage 3 Background Info.txt"
#define BACKGROUND_STAGE_4_TEXTURE_PATH		L"Backgrounds\\Stage 4 Source.png"
#define BACKGROUND_STAGE_4_INFO_PATH		L"Backgrounds\\Stage 4 Background Info.txt"
#define BACKGROUND_STAGE_5_TEXTURE_PATH		L"Backgrounds\\Stage 5 Source.png"
#define BACKGROUND_STAGE_5_INFO_PATH		L"Backgrounds\\Stage 5 Background Info.txt"
#define BACKGROUND_STAGE_6_TEXTURE_PATH		L"Backgrounds\\Stage 6 Source.png"
#define BACKGROUND_STAGE_6_INFO_PATH		L"Backgrounds\\Stage 6 Background Info.txt"
#define BACKGROUND_STAGE_7_TEXTURE_PATH		L"Backgrounds\\Stage 7 Soucre.png"
#define BACKGROUND_STAGE_7_INFO_PATH		L"Backgrounds\\Stage 7 Background Info.txt"
#define BACKGROUND_TITLE_SCENE_TEXTURE_PATH	L"Backgrounds\\Title Scene.png"
#define BACKGROUND_DEATH_SCENE_TEXTURE_PATH	L"Backgrounds\\Death Scene.png"

#define BACKGROUND_SOURCE_INFO_PATH "Backgrounds\\Background Source Info.txt"


class Background
{
protected:
	int stage;
	int n_arrays;
	vector<vector<int>> tiled_arrays;
public:

	Background(int ID_TEX_BACKGROUND, D3DCOLOR BACKGROUND_TEXTURE_BACKGROUND_COLOR, int stage);
	void Render();
	static int ChangeStage(int currentStage);
};

typedef Background* LPBACKGROUND;