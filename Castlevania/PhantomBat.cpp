#include "PhantomBat.h"

PhantomBat *PhantomBat::__instance = NULL;

PhantomBat *PhantomBat::GetInstance()
{
	return __instance;
}

PhantomBat *PhantomBat::GetInstance(float x, float y)
{
	__instance = new PhantomBat(x, y);
	return __instance;
}

PhantomBat::PhantomBat()
{
}

PhantomBat::PhantomBat(float x, float y)
{
	SetPosition(x, y);
	SetState(PHANTOMBAT_STATE_IDLE);
	movePhase = 0;
	desX = x;
	desY = y;
	health = 16;

	AddAnimation(901);
	AddAnimation(902);
}

void PhantomBat::SetState(int state)
{
	this->state = state;
}

void PhantomBat::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = left + PHANTOMBAT_BBOX_WIDTH;
	bottom = top + PHANTOMBAT_BBOX_HEIGHT;
}

void PhantomBat::Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *Objects)
{
	if (state == PHANTOMBAT_STATE_IDLE) return;

	if (GetTickCount() >= freezingTime)
		isFreezing = false;
	if (isFreezing) return;

	Camera *camera = Camera::GetInstance();
	float camera_x = camera->GetX();
	int SCREEN_WIDTH = camera->GetScreenWidth();
	CSimon *simon = CSimon::GetInstance();
	float simon_x, simon_y;
	simon->GetPosition(simon_x, simon_y);

	/*4 phases: 
	phase 0 : move to top of simon
	phase 1 : move to left top of simon
	phase 2 : move to simon
	phase 3 : move to right top of simon
	*/
	// if the bat reach desX, desY set new destination
	switch (movePhase)
	{
	case 0:
		if ((abs(x - desX) <= 10) && abs(y - desY) <= 10)
		{
			movePhase++;
			desX = simon_x;
			desY = simon_y - 50;
			SetSpeed((desX - x) / 1000, (desY - y) / 1000);
		}
	case 1:
		if ((abs(x - desX) <= 10) && abs(y - desY) <= 10)
		{
			movePhase++;
			desX = max(simon_x - 50, camera_x - SCREEN_WIDTH / 2 + 50);
			desY = simon_y - 50;
			SetSpeed((desX - x) / 2000, (desY - y) / 2000);
		}
	case 2: 
		if ((abs(x - desX) <= 10) && abs(y - desY) <= 10)
		{
			movePhase++;
			desX = simon_x;
			desY = simon_y;
			SetSpeed((desX - x) / 1000, (desY - y) / 1000);
		}
	case 3:
		if ((abs(x - desX) <= 10) && abs(y - desY) <= 10)
		{
			movePhase = 0;
			desX = min(simon_x + 50, camera_x + SCREEN_WIDTH / 2 - 50);
			desY = simon_y - 50;
			SetSpeed((desX - x) / 2000, (desY - y) / 2000);
		}
	}

	x += vx * dt;
	y += vy * dt;
}

void PhantomBat::Render()
{
	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);

	animations[state]->Render(pos.x, pos.y);
}

bool PhantomBat::DoesExist()
{
	return health > 0;
}

PhantomBat::~PhantomBat()
{
}
