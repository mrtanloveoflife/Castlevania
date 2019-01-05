#include "Panther.h"

Panther::Panther(float x, float y)
{
	this->SetPosition(x, y);

	health = 1;

	AddAnimation(720);
	AddAnimation(721);
	AddAnimation(722);
	AddAnimation(723);
	AddAnimation(724);
	AddAnimation(725);

	SetState(PANTHER_STATE_IDLE_LEFT);

	onScreen = true;
	isActive = false;
}

void Panther::SetState(int state)
{
	this->state = state;
	switch (state)
	{
	case PANTHER_STATE_IDLE_LEFT:
		SetSpeed(0, 0);
		break;
	case PANTHER_STATE_JUMP_RIGHT:
		SetSpeed(PANTHER_RUN_SPEED_X, -PANTHER_JUMP_SPEED_Y);
		break;
	case PANTHER_STATE_JUMP_LEFT:
		SetSpeed(-PANTHER_RUN_SPEED_X, -PANTHER_JUMP_SPEED_Y);
		break;
	case PANTHER_STATE_RUN_RIGHT:
		SetSpeed(PANTHER_RUN_SPEED_X, 0);
		break;
	case PANTHER_STATE_RUN_LEFT:
		SetSpeed(-PANTHER_RUN_SPEED_X, 0);
		break;
	}
}

void Panther::Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *Objects)
{
	if (GetTickCount() >= freezingTime)
		isFreezing = false;
	if (isFreezing) return;

	CSimon *simon = CSimon::GetInstance();
	float simon_x = simon->GetX();
	
	// If simon is near, jump toward him
	if (!isActive)
		if (abs(this->x - simon_x) <= 50)
		{
			if (this->x < simon_x)
				SetState(PANTHER_STATE_JUMP_RIGHT);
			else
				SetState(PANTHER_STATE_JUMP_LEFT);

			isActive = true;
		}

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
		// If panther lands the ground, run toward simon
		if (state == PANTHER_STATE_JUMP_RIGHT || state == PANTHER_STATE_JUMP_LEFT)
			if (this->x < simon_x) 
				SetState(PANTHER_STATE_RUN_RIGHT);
			else 
				SetState(PANTHER_STATE_RUN_LEFT);

		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.1f;		// nx*0.1f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.1f;

		if (nx != 0)
		{
			if (state == PANTHER_STATE_RUN_RIGHT)
				SetState(PANTHER_STATE_RUN_LEFT);
			else
				SetState(PANTHER_STATE_RUN_RIGHT);

			this->nx = -this->nx;
		}

		if (ny != 0) vy = 0;
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	if (isActive)
	{
		Camera *camera = Camera::GetInstance();
		Point pos = camera->PositionOnCamera(x, y);
		int SCREEN_WIDTH = camera->GetScreenWidth();

		if (pos.x < -PANTHER_BBOX_WIDTH || pos.x > SCREEN_WIDTH)
			onScreen = false;
	}
}

void Panther::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x + 1;
	top = y;
	right = x + PANTHER_BBOX_WIDTH;
	bottom = y + PANTHER_BBOX_HEIGHT;
}

void Panther::Render()
{
	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);
	
	animations[state]->Render(pos.x, pos.y);
}

bool Panther::DoesExist()
{
	return (!isActive || onScreen) && health > 0;
}