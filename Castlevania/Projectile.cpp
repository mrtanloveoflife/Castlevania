#include "Projectile.h"



Projectile::Projectile(float x, float y, int state)
{
	SetPosition(x, y);
	SetState(state);
	onScreen = true;

	AddAnimation(740);
	AddAnimation(741);
}

void Projectile::SetState(int state)
{
	this->state = state;
	switch (state)
	{
	case PROJECTILE_STATE_RIGHT:
		vx = PROJECTILE_SPEED_X;
		vy = 0;
		break;
	case PROJECTILE_STATE_LEFT:
		vx = -PROJECTILE_SPEED_X;
		vy = 0;
		break;
	}
}

void Projectile::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = left + PROJECTILE_BBOX_WIDTH;
	bottom = top + PROJECTILE_BBOX_HEIGHT;
}

void Projectile::Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *Objects)
{
	CGameObject::Update(dt, Objects);
	x += dx;
	y += dy;

	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);
	int SCREEN_WIDTH = camera->GetScreenWidth();

	if (pos.x < -PROJECTILE_BBOX_WIDTH || pos.x > SCREEN_WIDTH)
		onScreen = false;
}

void Projectile::Render()
{
	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);

	animations[state]->Render(pos.x, pos.y);
}
bool Projectile::DoesExist()
{
	return onScreen;
}

Projectile::~Projectile()
{
}
