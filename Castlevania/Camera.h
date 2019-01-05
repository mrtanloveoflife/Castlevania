#pragma once
#include <Windows.h>

#include "Simon.h"
#include "Point.h"
#include "GameObject.h"

#define CAMERA_CHANGING_SCENE_SPEED 0.1f

class Camera
{
	static Camera *__instance;

	float x, y;
	int screen_Width;
	int screen_Height;
	vector<LPGAMEOBJECT> doors;

public:
	static Camera *GetInstance();

	float GetX() { return x; }
	void SetX(float x) { this->x = x; }
	int GetScreenWidth() { return screen_Width; }
	int GetScreenHeight() { return screen_Height; }
	Point PositionOnCamera(float x, float y);
	void SetUpCamera(int SCREEN_WIDTH, int SCREEN_HEIGHT, float background_Width, vector<vector<LPGAMEOBJECT>> *Objects = NULL);
	void Update();
};