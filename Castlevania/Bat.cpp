#include "Bat.h"

DWORD Bat::spawTime = GetTickCount() + (rand() % 10 + 3) * 1000;

Bat::Bat(float x, int ymin, int ymax, int nx)
{
	this->SetPosition(x, rand() % ymax + ymin * 1.0f);

	health = 1;

	AddAnimation(710);
	AddAnimation(711);

	this->nx = nx;

	if (nx == 1)
		SetSpeed(BAT_SPEED_X, BAT_SPEED_Y);
	else
		SetSpeed(-BAT_SPEED_X, BAT_SPEED_Y);
	onScreen = true;
	changeVyTime = GetTickCount() + 500;
}

int Bat::SpawnPos()
{
	if (GetTickCount() < spawTime) return -1;

	CSimon *simon = CSimon::GetInstance();
	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(simon->GetX(), simon->GetY());
	int SCREEN_WIDTH = camera->GetScreenWidth();

	spawTime = GetTickCount() + (rand() % 10 + 3) * 1000;

	if (pos.x < 50) return 2;
	if (pos.x > SCREEN_WIDTH - 50) return 1;

	return rand() % 2 + 1;
}

void Bat::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + BAT_BBOX_WIDTH;
	bottom = y + BAT_BBOX_HEIGHT;
}

void Bat::Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *Objects)
{
	if (GetTickCount() >= freezingTime)
		isFreezing = false;
	if (isFreezing) return;

	CGameObject::Update(dt);
	if (GetTickCount() >= changeVyTime)
	{
		this->vy = -this->vy;
		changeVyTime = GetTickCount() + 500;
	}

	x += dx;
	y += dy;

	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);
	int SCREEN_WIDTH = camera->GetScreenWidth();

	if (pos.x < -BAT_BBOX_WIDTH || pos.x > SCREEN_WIDTH)
		onScreen = false;
}

void Bat::Render()
{
	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);

	if (vx > 0)
		animations[0]->Render(pos.x, pos.y);
	else
		animations[1]->Render(pos.x, pos.y);
}

bool Bat::DoesExist()
{
	return onScreen && health > 0;
}