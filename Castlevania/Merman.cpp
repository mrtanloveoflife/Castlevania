#include "Merman.h"

DWORD Merman::spawTime = GetTickCount() + (rand() % 5 + 1) * 1000;

int Merman::SpawnPos()
{
	if (GetTickCount() < spawTime) return -1;

	spawTime = GetTickCount() + (rand() % 5 + 1) * 1000;

	return 1;
}

Merman::Merman()
{
	Camera *camera = Camera::GetInstance();
	int xmin = int(camera->GetX() - camera->GetScreenWidth() / 2);
	int xmax = int(camera->GetX() + camera->GetScreenWidth() / 2);

	this->SetPosition(rand() % xmax + xmin * 1.0f, camera->GetScreenHeight() - 100 * 1.0f);

	health = 1;

	AddAnimation(730);
	AddAnimation(731);
	AddAnimation(732);
	AddAnimation(733);
	AddAnimation(734);
	AddAnimation(735);

	onScreen = true;
	isAttacking = false;

	CSimon *simon = CSimon::GetInstance();
	if (this->x == simon->GetX()) this->x += 50;
	if (x < simon->GetX())
		SetState(MERMAN_STATE_JUMP_RIGHT);
	else
		SetState(MERMAN_STATE_JUMP_LEFT);
}

void Merman::SetState(int state)
{
	this->state = state;
	switch (state)
	{
	case MERMAN_STATE_JUMP_RIGHT:
		vx = 0;
		vy = -MERMAN_JUMP_SPPED_Y;
		break;
	case MERMAN_STATE_JUMP_LEFT :
		vx = 0;
		vy = -MERMAN_JUMP_SPPED_Y;
		break;
	case MERMAN_STATE_WALK_RIGHT:
		vx = MERMAN_WALK_SPEED_X;
		vy = 0;
		break;
	case MERMAN_STATE_WALK_LEFT:
		vx = -MERMAN_WALK_SPEED_X;
		vy = 0;
		break;
	case MERMAN_STATE_ATTACK_RIGHT:
		vx = 0;
		vy = 0;
		break;
	case MERMAN_STATE_ATTACK_LEFT:
		vx = 0;
		vy = 0;
		break;
	}
}

void Merman::ChoosingAttackingTime()
{
	startAttackingTime = GetTickCount() + rand() % 5 + 1000;
}

void Merman::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == MERMAN_STATE_ATTACK_RIGHT || state == MERMAN_STATE_ATTACK_LEFT)
	{
		left = x;
		top = y + 2;
		right = left + MERMAN_ATTACKING_BBOX_WIDTH;
		bottom = top + MERMAN_ATTACKING_BBOX_HEIGHT;
	}
	else
	{
		left = x;
		top = y;
		right = left + MERMAN_WALKING_BBOX_WIDTH;
		bottom = top + MERMAN_WALKING_BBOX_HEIGHT;
	}
}

void Merman::Brickcollision(DWORD dt, vector<vector<LPGAMEOBJECT>> *Objects)
{
	CSimon *simon = CSimon::GetInstance();
	float simon_x = simon->GetX();

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

	if (coEvents.size() == 0 || vy < 0)
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
			if (state == MERMAN_STATE_WALK_RIGHT)
				SetState(MERMAN_STATE_WALK_LEFT);
			else
				SetState(MERMAN_STATE_WALK_RIGHT);
			this->nx = -this->nx;
		}

		// If merman lands the ground, walk toward simon
		if (ny == -1)
			if (state == MERMAN_STATE_JUMP_RIGHT || state == MERMAN_STATE_JUMP_LEFT)
			{
				if (this->x < simon_x)
					SetState(MERMAN_STATE_WALK_RIGHT);
				else
					SetState(MERMAN_STATE_WALK_LEFT);
				ChoosingAttackingTime();
			}

		if (ny != 0) vy = 0;
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Merman::Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *Objects)
{
	if (GetTickCount() >= freezingTime)
		isFreezing = false;
	if (isFreezing) return;

	CSimon *simon = CSimon::GetInstance();
	float simon_x = simon->GetX();

	if (state == MERMAN_STATE_WALK_RIGHT || state == MERMAN_STATE_WALK_LEFT)
		if (GetTickCount() >= startAttackingTime && GetTickCount() < startAttackingTime + 1000 && !isAttacking)
		{
			Projectile * projectile;
			if (this->x < simon_x)
			{
				SetState(MERMAN_STATE_ATTACK_RIGHT);
				projectile = new Projectile(this->x + 16, this->y + 2, PROJECTILE_STATE_RIGHT);
			}
			else
			{
				SetState(MERMAN_STATE_ATTACK_LEFT);
				projectile = new Projectile(this->x, this->y +  2, PROJECTILE_STATE_LEFT);
			}
			Objects->at(int(projectile->GetX() / 256)).push_back(projectile);

			isAttacking = true;
		}
	if ((state == MERMAN_STATE_ATTACK_RIGHT || state == MERMAN_STATE_ATTACK_LEFT) && GetTickCount() >= startAttackingTime + 1000)
	{
		if (this->x < simon_x)
			SetState(MERMAN_STATE_WALK_RIGHT);
		else
			SetState(MERMAN_STATE_WALK_LEFT);
		ChoosingAttackingTime();

		isAttacking = false;
	}

	Brickcollision(dt, Objects);
	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);
	int SCREEN_WIDTH = camera->GetScreenWidth();
	int SCREEN_HEIGHT = camera->GetScreenHeight();

	if (pos.x < -MERMAN_WALKING_BBOX_WIDTH || pos.x > SCREEN_WIDTH || pos.y > SCREEN_HEIGHT)
		onScreen = false;
}

void Merman::Render()
{
	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);

	animations[state]->Render(pos.x, pos.y);
}

bool Merman::DoesExist()
{
	return onScreen && health > 0;
}

Merman::~Merman()
{
}
