#include "Medusa.h"
Medusa *Medusa::__instance = NULL;

Medusa *Medusa::GetInstance()
{
	return __instance;
}

Medusa *Medusa::GetInstance(float x, float y)
{
	__instance = new Medusa(x, y);
	return __instance;
}
Medusa::Medusa(float x, float y)
{
	SetPosition(x, y);
	SetState(MEDUSA_STATE_IDLE);
	movePhase = 0;
	desX = x;
	desY = y;
	health = 16;
	changeVyTime = GetTickCount() + 500;
	changePhase = GetTickCount() + 1000;
	startAttackingTime = GetTickCount() + 1100;
	AddAnimation(903);
	AddAnimation(904);
}

void Medusa::SetState(int state)
{
	this->state = state;
}

void Medusa::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = left + MEDUSA_BBOX_WIDTH;
	bottom = top + MEDUSA_BBOX_HEIGHT;
}

void Medusa::Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *Objects)
{
	if (state == MEDUSA_STATE_IDLE) return;

	if (GetTickCount() >= freezingTime)
		isFreezing = false;
	if (isFreezing) return;

	CSimon *simon = CSimon::GetInstance();
	float simon_x, simon_y;
	simon->GetPosition(simon_x, simon_y);

	if (movePhase == 0 && GetTickCount() >= changeVyTime)
	{
		this->vy = -this->vy;
		changeVyTime = GetTickCount() + 500;
	}
	//
	switch (movePhase)
	{
	case 0:							// Get Position
		
		if (GetTickCount() > changePhase)
		{
			movePhase++;
			changePhase = GetTickCount() + 2000;
			SetSpeed(0, (simon_y - y) / 2000);
		}
		break;
	
	case 1:
		CassiopeiaMini *cassiopeiaMini;
		if (GetTickCount() > startAttackingTime)
		{
			if (this->x < simon_x)
			{
				SetState(MEDUSA_STATE_ACTIVE);
				cassiopeiaMini = new CassiopeiaMini(this->x, this->y, CASSIOPEIAMINI_STATE_RIGHT);
			}
			else
			{
				SetState(MEDUSA_STATE_ACTIVE);
				cassiopeiaMini = new CassiopeiaMini(this->x, this->y, CASSIOPEIAMINI_STATE_LEFT);
			}
			Objects->at(int(cassiopeiaMini->GetX() / 256)).push_back(cassiopeiaMini);
			startAttackingTime = GetTickCount() + 1100;
		}
		if (GetTickCount() > changePhase)
		{
			if (x > simon_x)
			{
				SetSpeed(-MEDUSA_SPEED_X, MEDUSA_SPEED_Y);
			}
			if (x < simon_x)
			{
				SetSpeed(MEDUSA_SPEED_X, MEDUSA_SPEED_Y);
			}
			movePhase = 0;
			changePhase = GetTickCount() + 3000;
		}
		break;
	}


	x += vx * dt;
	y += vy * dt;
}

void Medusa::Render()
{
	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);

	animations[state]->Render(pos.x, pos.y);
}

bool Medusa::DoesExist()
{
	return health > 0;
}

Medusa::~Medusa()
{
}
