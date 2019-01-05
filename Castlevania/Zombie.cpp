#include "Zombie.h"

DWORD Zombie::spawTime = GetTickCount() + (rand() % 5 + 1) * 1000;

Zombie::Zombie(float x, float y, int nx)
{
	this->SetPosition(x, y);

	health = 1;

	AddAnimation(700);
	AddAnimation(701);

	this->nx = nx;

	if (nx == 1)
		SetSpeed(ZOMBIE_SPEED, 0);
	else
		SetSpeed(-ZOMBIE_SPEED, 0);
	onScreen = true;
}

int Zombie::SpawnPos()
{
	if (GetTickCount() < spawTime) return -1;

	CSimon *simon = CSimon::GetInstance();
	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(simon->GetX(), simon->GetY());
	int SCREEN_WIDTH = camera->GetScreenWidth();

	spawTime = GetTickCount() + (rand() % 5 + 1) * 1000;

	if (pos.x < 50) return 2;
	if (pos.x > SCREEN_WIDTH - 50) return 1;

	return rand() % 2 + 1;
}

void Zombie::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + ZOMBIE_BBOX_WIDTH;
	bottom = y + ZOMBIE_BBOX_HEIGHT;
}

void Zombie::Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *Objects)
{
	if (GetTickCount() >= freezingTime)
		isFreezing = false;
	if (isFreezing) return;

	CGameObject::Update(dt);
	vy += GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vector<LPGAMEOBJECT> coObjects;

	for (int i = max(0, int(x) / 256 - 1); i <= int(x) / 256 + 1; i++)
		for (UINT j = 0; j < Objects->at(i).size(); j++)
			if (dynamic_cast<Brick *>(Objects->at(i).at(j)) || dynamic_cast<BreakableBrick *>(Objects->at(i).at(j)))
				coObjects.push_back(Objects->at(i).at(j));

	CalcPotentialCollisions(&coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.1f;		// nx*0.1f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.1f;

		if (nx != 0)
		{
			this->vx = -vx;
			this->nx = -this->nx;
		}

		if (ny != 0) vy = 0;
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);
	int SCREEN_WIDTH = camera->GetScreenWidth();

	if (pos.x < -ZOMBIE_BBOX_WIDTH || pos.x > SCREEN_WIDTH)
		onScreen = false;
}

void Zombie::Render()
{
	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);

	if (vx > 0)
		animations[0]->Render(pos.x, pos.y);
	else
		animations[1]->Render(pos.x, pos.y);
}

bool Zombie::DoesExist()
{
	return onScreen && health > 0;
}