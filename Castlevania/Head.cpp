#include "Head.h"

DWORD Head::spawTime = GetTickCount() + (rand() % 5 + 2) * 1000;

Head::Head(float x, float y, int nx)
{
	this->SetPosition(x, y);

	health = 3;

	AddAnimation(770);
	AddAnimation(771);

	this->nx = nx;

	if (nx == 1)
		SetSpeed(HEAD_SPEED_X, HEAD_SPEED_Y);
	else
		SetSpeed(-HEAD_SPEED_X, HEAD_SPEED_Y);
	onScreen = true;
	changeVyTime = GetTickCount() + 1000;
}

int Head::SpawnPos()
{
	if (GetTickCount() < spawTime) return -1;

	CSimon *simon = CSimon::GetInstance();
	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(simon->GetX(), simon->GetY());
	int SCREEN_WIDTH = camera->GetScreenWidth();

	spawTime = GetTickCount() + (rand() % 5 + 2) * 1000;

	if (pos.x < 50) return 2;
	if (pos.x > SCREEN_WIDTH - 50) return 1;

	return rand() % 2 + 1;
}

void Head::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + HEAD_BBOX_WIDTH;
	bottom = y + HEAD_BBOX_HEIGHT;
}

void Head::Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *Objects)
{
	if (GetTickCount() >= freezingTime)
		isFreezing = false;
	if (isFreezing) return;

	CGameObject::Update(dt);
	if (GetTickCount() >= changeVyTime)
	{
		this->vy = -this->vy;
		changeVyTime = GetTickCount() + 1000;
	}

	x += dx;
	y += dy;

	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);
	int SCREEN_WIDTH = camera->GetScreenWidth();

	if (pos.x < -HEAD_BBOX_WIDTH || pos.x > SCREEN_WIDTH)
		onScreen = false;
}

void Head::Render()
{
	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);

	if (vx > 0)
		animations[0]->Render(pos.x, pos.y);
	else
		animations[1]->Render(pos.x, pos.y);
}

bool Head::DoesExist()
{
	return onScreen && health > 0;
}