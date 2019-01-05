#include "Soldier.h"

Soldier::Soldier(float x, float y, float xmin, float xmax)
{
	SetMaxMin(xmin, xmax);
	this->SetPosition(x, y);
	health = 3;

	AddAnimation(750);
	AddAnimation(751);

	this->nx = 1;
	if (nx == 1)
		SetSpeed(SOLDIER_SPEED_X, 0);
	else
		SetSpeed(-SOLDIER_SPEED_X, 0);
	onScreen = true;
}

void Soldier::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + SOLDIER_BBOX_WIDTH;
	bottom = y + SOLDIER_BBOX_HEIGHT;
}
void Soldier::SetMaxMin(float xmin, float xmax)
{
	this->xmax = xmax;
	this->xmin = xmin;
}

void Soldier::Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *Objects)
{
	if (GetTickCount() >= freezingTime)
		isFreezing = false;
	if (isFreezing) return;

	CGameObject::Update(dt);
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


	if (this->vx > 0 && this->x > this->xmax)
			SetSpeed(-SOLDIER_SPEED_X, 0);
	if(this->x < this->xmin && this->vx < 0)
			SetSpeed(SOLDIER_SPEED_X, 0);

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

	if (pos.x < -SOLDIER_BBOX_WIDTH || pos.x > SCREEN_WIDTH)
		onScreen = false;
	
}

void Soldier::Render()
{
	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);

	if (vx > 0)
		animations[0]->Render(pos.x, pos.y);
	else
		animations[1]->Render(pos.x, pos.y);
}

bool Soldier::DoesExist()
{
	return health > 0;
}
