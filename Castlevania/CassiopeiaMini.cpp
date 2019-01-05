#include "CassiopeiaMini.h"

CassiopeiaMini::CassiopeiaMini(float x, float y, int state)
{
	SetPosition(x, y);
	SetState(state);
	AddAnimation(793);
	AddAnimation(794);
	onScreen = true;
}
void CassiopeiaMini::SetState(int state)
{
	this->state = state;
	switch (state)
	{
	case CASSIOPEIAMINI_STATE_RIGHT:
		vx = CASSIOPEIAMINI_SPEED_X;
		vy = 0;
		break;
	case CASSIOPEIAMINI_STATE_LEFT:
		vx = -CASSIOPEIAMINI_SPEED_X;
		vy = 0;
		break;
	}
}
void CassiopeiaMini::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = left + CASSIOPEIAMINI_BBOX_WIDTH;
	bottom = top + CASSIOPEIAMINI_BBOX_HEIGHT;
}
void CassiopeiaMini::Brickcollision(DWORD dt, vector<vector<LPGAMEOBJECT>> *Objects)
{
	CSimon *simon = CSimon::GetInstance();
	float simon_x = simon->GetX();

	CGameObject::Update(dt);

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
			this->vx = 0;
		}
		if (ny != 0)
		{
			this->vy = 0;
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void CassiopeiaMini::Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *Objects)
{
	CGameObject::Update(dt, Objects);
	vy += GRAVITY * dt;
	if (vx != 0)
		Brickcollision(dt, Objects);
	else
	{
		x += dx;
		y += dy;
	}
	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);
	int SCREEN_WIDTH = camera->GetScreenWidth();

	if (pos.x < -CASSIOPEIAMINI_BBOX_WIDTH || pos.x > SCREEN_WIDTH)
		onScreen = false;
}
void CassiopeiaMini::Render()
{
	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);

	animations[state]->Render(pos.x, pos.y);
}
bool CassiopeiaMini::DoesExist()
{
	return onScreen;
}
CassiopeiaMini::~CassiopeiaMini()
{
}
