#include "SnakeHead.h"

SnakeHead::SnakeHead(float x, float y)
{
	this->SetPosition(x, y);
	health = 10;

	startAttackingTime = GetTickCount();
	AddAnimation(780);
	AddAnimation(781);
	onScreen = true;
}

void SnakeHead::ChoosingAttackingTime()
{
	startAttackingTime = GetTickCount() + (rand() % 5 ) * 1000;
}

void SnakeHead::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
		left = x;
		top = y + 2;
		right = left + SNAKEHEAD_BBOX_WIDTH;
		bottom = top + SNAKEHEAD_BBOX_HEIGHT;
}

void SnakeHead::Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *Objects)
{
	if (GetTickCount() >= freezingTime)
		isFreezing = false;
	if (isFreezing) return;

	CSimon *simon = CSimon::GetInstance();
	float simon_x = simon->GetX();
	float simon_y = simon->GetY();

	if (abs(simon_x - this->x) < 100 && abs(simon_y - this->y) < 100)
		if (GetTickCount() >= startAttackingTime)
		{
			Projectile * projectile;
			if (this->x < simon_x)
			{
				projectile = new Projectile(this->x + 12, this->y + 2, PROJECTILE_STATE_RIGHT);
			}
			else
			{
				projectile = new Projectile(this->x + 3, this->y + 2, PROJECTILE_STATE_LEFT);
			}
			Objects->at(int(projectile->GetX() / 256)).push_back(projectile);
			ChoosingAttackingTime();
		}



	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);
	int SCREEN_WIDTH = camera->GetScreenWidth();
	int SCREEN_HEIGHT = camera->GetScreenHeight();

	if (pos.x < -SNAKEHEAD_BBOX_WIDTH || pos.x > SCREEN_WIDTH || pos.y > SCREEN_HEIGHT)
		onScreen = false;
}

void SnakeHead::Render()
{
	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);
	CSimon *simon = CSimon::GetInstance();
	float simon_x = simon->GetX();
	
	if (simon_x > this->x)
		animations[0]->Render(pos.x, pos.y);
	if (simon_x < this->x)
		animations[1]->Render(pos.x, pos.y);
}

bool SnakeHead::DoesExist()
{
	return health > 0;
}

SnakeHead::~SnakeHead()
{
}
