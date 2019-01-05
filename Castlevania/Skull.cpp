#include "Skull.h"

Skull::Skull(float x, float y)
{
	CSimon *simon = CSimon::GetInstance();
	float simon_x = simon->GetX();
	float simon_y = simon->GetY();
	this->SetPosition(x, y);
	this->SetSpeed(0, 0);

	health = 1;

	AddAnimation(790);
	AddAnimation(791);

	onScreen = true;
}


void Skull::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + SKULL_BBOX_WIDTH;
	bottom = y + SKULL_BBOX_HEIGHT;
}

void Skull::Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *Objects)
{
	if (GetTickCount() >= freezingTime)
		isFreezing = false;
	if (isFreezing) return;
	CGameObject::Update(dt);

	CSimon *simon = CSimon::GetInstance();
	float simon_x = simon->GetX();
	float simon_y = simon->GetY();

	if (abs(this->x - simon_x) <= 100 && abs(this->y - simon_y) <= 50)
	{
		if (y == simon_y)
			vy = 0;
		else if (x < simon_x && y < simon_y)
		{
			vx = SKULL_SPEED_X;
			vy = SKULL_SPEED_Y;
		}
		else if (x < simon_x && y > simon_y)
		{
			vx = SKULL_SPEED_X;
			vy = -SKULL_SPEED_Y;
		}
		else if (x > simon_x && y > simon_y)
		{
			vx = -SKULL_SPEED_X;
			vy = -SKULL_SPEED_Y;
		}
		else if (x > simon_x && y < simon_y)
		{
			vx = -SKULL_SPEED_X;
			vy = SKULL_SPEED_Y;
		}
	}

	x += dx;
	y += dy;

	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);
	int SCREEN_WIDTH = camera->GetScreenWidth();

	if (pos.x < -ZOMBIE_BBOX_WIDTH || pos.x > SCREEN_WIDTH)
		onScreen = false;
	else onScreen = true;

}

void Skull::Render()
{
	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);

	if (vx > 0)
		animations[0]->Render(pos.x, pos.y);
	else if (vx < 0)
		animations[1]->Render(pos.x, pos.y);
}

bool Skull::DoesExist()
{
	if (vx != 0)
		return onScreen && health > 0;

	return health > 0;
}