#include "Bat2.h"

Bat2::Bat2(float x, float y)
{
	this->SetPosition(x, y);

	health = 3;

	AddAnimation(760);
	AddAnimation(761);
	AddAnimation(762);

	SetState(BAT2_STATE_IDLE);

	onScreen = true;
	isActive = false;
}

void Bat2::SetState(int state)
{
	this->state = state;
	switch (state)
	{
	case BAT2_STATE_IDLE:
		SetSpeed(0, 0);
		break;
	case BAT2_STATE_FLY_RIGHT:
		SetSpeed(BAT2_FLY_SPEED_X, BAT2_FLY_SPEED_Y);
		break;
	case BAT2_STATE_FLY_LEFT:
		SetSpeed(-BAT2_FLY_SPEED_X, BAT2_FLY_SPEED_Y);
		break;
	}
}

void Bat2::Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *Objects)
{
	if (GetTickCount() >= freezingTime)
		isFreezing = false;
	if (isFreezing) return;

	CSimon *simon = CSimon::GetInstance();
	float simon_x = simon->GetX();
	float simon_y = simon->GetY();
	if (!isActive)
		if (abs(this->x - simon_x) <= 100 && abs(this->y - simon_y) <= 70)
		{
			if (this->x < simon_x)
				SetState(BAT2_STATE_FLY_RIGHT);
			else
				SetState(BAT2_STATE_FLY_LEFT);

			isActive = true;
		}

	CGameObject::Update(dt);

	if (this->y >= simon->GetY() ) vy = 0;
	x += dx;
	y += dy;

	if (isActive)
	{
		Camera *camera = Camera::GetInstance();
		Point pos = camera->PositionOnCamera(x, y);
		int SCREEN_WIDTH = camera->GetScreenWidth();

		if (pos.x < -BAT2_BBOX_WIDTH || pos.x > SCREEN_WIDTH)
			onScreen = false;
	}
}

void Bat2::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x + 1;
	top = y;
	right = x + BAT2_BBOX_WIDTH;
	bottom = y + BAT2_BBOX_HEIGHT;
}

void Bat2::Render()
{
	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);

	animations[state]->Render(pos.x, pos.y);
}

bool Bat2::DoesExist()
{
	return (!isActive || onScreen) && health > 0;
}