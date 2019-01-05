#include "Camera.h"

Camera *Camera::__instance = NULL;

Camera *Camera::GetInstance()
{
	if (__instance == NULL) __instance = new Camera();
	return __instance;
}

void Camera::Update()
{
	CSimon *simon = CSimon::GetInstance();
	float simon_x = simon->GetX();

	// Finding simon's posision between doors
	for (UINT i = 0; i < doors.size() - 1; i++)
		if (simon_x >= doors[i]->GetX() && simon_x <= doors[i + 1]->GetX())
		{
			if (doors[i]->nx == -1)
			{
				if (simon_x + 8 < doors[i]->GetX() + screen_Width / 2 + 16)
				{
					x = doors[i]->GetX() + screen_Width / 2 + 16;
					return;
				}
			}
			else
			{
				if (simon_x + 8 < doors[i]->GetX() + screen_Width / 2 + 8)
				{
					x = doors[i]->GetX() + screen_Width / 2 + 8;
					return;
				}
			}
			if (doors[i + 1]->nx == -1)
			{
				x = min(simon_x + 8, doors[i + 1]->GetX() + 31 - screen_Width / 2);
			}
			else x = min(simon_x + 8, doors[i + 1]->GetX() + 47 - screen_Width / 2 - 24);
			return;
		}
}

void Camera::SetUpCamera(int SCREEN_WIDTH, int SCREEN_HEIGHT, float background_Width, vector<vector<LPGAMEOBJECT>> *Objects)
{
	this->screen_Width = SCREEN_WIDTH;
	this->screen_Height = SCREEN_HEIGHT;

	doors.clear();

	doors.push_back(new Door(-16.0f, 0.0f, -1));

	for (UINT i = 0; i < Objects->size(); i++)
		for (UINT j = 0; j < Objects->at(i).size(); j++)
			if (dynamic_cast<Door *>(Objects->at(i).at(j)))
				doors.push_back(Objects->at(i).at(j));

	doors.push_back(new Door(background_Width - 9, 0.0f, 1));
}

Point Camera::PositionOnCamera(float x, float y)
{
	Point point;
	point.x = x - (this->x - screen_Width / 2);
	point.y = y + 50;
	return point;
}