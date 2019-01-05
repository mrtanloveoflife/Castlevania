#include "RangedWeapon.h"

void RangedWeapon::LoadRangedWeaponTexture(int ID_TEX_RANGED_WEAPONS, LPCWSTR RANGED_WEAPONS_TEXTURE_PATH, D3DCOLOR RANGED_WEAPONS_TEXTURE_BACKGROUND_COLOR)
{

	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_RANGED_WEAPONS, RANGED_WEAPONS_TEXTURE_PATH, RANGED_WEAPONS_TEXTURE_BACKGROUND_COLOR);

	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texWeapons = textures->Get(ID_TEX_RANGED_WEAPONS);

	ifstream in;
	in.open(RANGED_WEAPON_SPRITES_PATH);
	int n, nState, id, left, top, right, bottom;
	LPANIMATION ani;

	in >> nState;
	for (int i = 0; i < nState; i++)
	{
		in >> n;
		for (int j = 0; j < n; j++)
		{
			in >> id >> left >> top >> right >> bottom;
			sprites->Add(id, left, top, right, bottom, texWeapons);
		}
		if (i >= 6 && i <= 9)
			ani = new CAnimation(50);
		else
			ani = new CAnimation(100);
		for (int j = n - 1; j >= 0; j--)
			ani->Add(id - j);
		animations->Add(ID_TEX_RANGED_WEAPONS + i, ani);
	}

	in.close();
}

RangedWeapon::RangedWeapon(int type)
{
	doesExist = true;

	CSimon *simon = CSimon::GetInstance();

	SetState(simon->GetState() % 2);

	SetType(type);

	if (state == 0) CGameObject::SetPosition(simon->GetX(), simon->GetY() + 8);
	else CGameObject::SetPosition(simon->GetX() + 10, simon->GetY() + 8);

	if (simon->GetState() == SIMON_STATE_CROUCHING_THROW_RIGHT || simon->GetState() == SIMON_STATE_CROUCHING_THROW_LEFT) 
		CGameObject::SetPosition(x, y + 7);
	
	hitSound = new Sound();
	Mix_Chunk * mix = Mix_LoadWAV(SOUND_HIT);
	hitSound->SetSound(3, mix, 0);

	if (type == ID_RANGED_WEAPON_HOLY_WATER_IN_POTION)
	{
		holyWaterSound = new Sound();
		mix = Mix_LoadWAV(SOUND_HOLY_WATER);
		holyWaterSound->SetSound(6, mix, 0);
	}

	if (type == ID_RANGED_WEAPON_BOOMERANG)
	{
		didChangeDirection = false;
		didResetTime = false;
	}

	if (type != ID_RANGED_WEAPON_STOPWATCH)
	{
		AddAnimation(600 + (type - 1) * 2);
		AddAnimation(600 + (type - 1) * 2 + 1);
	}

	startThrowingTime = GetTickCount();
}

void RangedWeapon::LoadWeaponSpeed()
{
	if (state == 0)
		vx = THROWING_SPEED_X;
	else
		vx = -THROWING_SPEED_X;

	switch (type)
	{
	case ID_RANGED_WEAPON_DAGGER:
		vy = 0;
		break;
	case ID_RANGED_WEAPON_HOLY_WATER_IN_POTION:
		vy = -THROWING_SPEED_Y;
		break;
	case ID_RANGED_WEAPON_HOLY_WATER_ON_FIRE:
		vx = 0;
		vy = 0;
		animations.clear();
		AddAnimation(600 + (type - 1) * 2);
		AddAnimation(600 + (type - 1) * 2 + 1);
		break;
	case ID_RANGED_WEAPON_AXE:
		if (state == 0)
			vx = THROWING_AXE_SPEED_X;
		else vx = -THROWING_AXE_SPEED_X;
		vy = -THROWING_AXE_SPEED_Y;

		break;
	case ID_RANGED_WEAPON_BOOMERANG:
		vy = 0;
		break;
	}
}

void RangedWeapon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	switch (type)
	{
	case ID_RANGED_WEAPON_DAGGER:
		left = x;
		top = y;
		right = left + DAGGER_BBOX_WIDTH;
		bottom = top + DAGGER_BBOX_HEIGHT;
		break;
	case ID_RANGED_WEAPON_HOLY_WATER_IN_POTION:
		if (state == 0)
			left = x;
		else
			left = x + 6;
		top = y + 3;
		right = left + HOLY_WATER_IN_POTION_BBOX_WIDTH;
		bottom = top + HOLY_WATER_IN_POTION_BBOX_HEIGHT;
		break;
	case ID_RANGED_WEAPON_HOLY_WATER_ON_FIRE:
		left = x;
		top = y;
		right = left + HOLY_WATER_ON_FIRE_BBOX_WIDTH;
		bottom = top + HOLY_WATER_ON_FIRE_BBOX_HEIGHT;
		break;
	case ID_RANGED_WEAPON_AXE:
		left = x;
		top = y;
		right = left + AXE_BBOX_WIDTH;
		bottom = top + AXE_BBOX_HEIGHT;
		break;
	case ID_RANGED_WEAPON_BOOMERANG:
		left = x;
		top = y;
		right = left + BOOMERANG_BBOX_WIDTH;
		bottom = top + BOOMERANG_BBOX_HEIGHT;
	}
}

void RangedWeapon::BricksCollision(DWORD dt, vector<vector<LPGAMEOBJECT>> *Objects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vector<LPGAMEOBJECT> coObjects;

	for (int i = max(0, int(x) / 256 - 1); i <= int(x) / 256 + 1; i++)
		for (UINT j = 0; j < Objects->at(i).size(); j++)
			if (dynamic_cast<Brick *>(Objects->at(i).at(j)) || dynamic_cast<BreakableBrick *>(Objects->at(i).at(j)))
				coObjects.push_back(Objects->at(i).at(j));

	CalcPotentialCollisions(&coObjects, coEvents);

	if (coEvents.size() > 0)
	{
		holyWaterSound->playSound();

		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.1f;		// nx*0.1f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.1f;

		vx = 0;
		vy = 0;

		SetType(ID_RANGED_WEAPON_HOLY_WATER_ON_FIRE);
		FireTime = GetTickCount() + 2000;

		float left, top, right, bottom;
		coEventsResult[0]->obj->GetBoundingBox(left, top, right, bottom);
		
		if (ny == -1) y = top - HOLY_WATER_ON_FIRE_BBOX_HEIGHT;
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void RangedWeapon::DestroyObjectsCollision(DWORD dt, vector<vector<LPGAMEOBJECT>> *Objects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vector<LPGAMEOBJECT> coObjects;
	for (int i = max(0, int(x) / 256 - 1); i <= int(x) / 256 + 1; i++)
		for (UINT j = 0; j < Objects->at(i).size(); j++)
			if (dynamic_cast<Candle *>(Objects->at(i).at(j)) || dynamic_cast<LPENEMY>(Objects->at(i).at(j)))
				coObjects.push_back(Objects->at(i).at(j));

	CalcPotentialCollisions(&coObjects, coEvents);

	if (coEvents.size() > 0)
	{
		hitSound->playSound();

		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// Collision logic with objects
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Candle *>(e->obj))
			{
				Candle *candle = dynamic_cast<Candle *>(e->obj);
				
				if (candle->GetItemType() == ITEM_TYPE_WHIP_UPGRADE && type == 2) candle->SetItemType(ITEM_TYPE_HEART_BIG);

				Item * item = new Item(candle->GetX(), candle->GetY(), candle->GetItemType());

				Objects->at(int(item->GetX() / 256)).push_back(item);

				Effect *effect = new Effect(e->obj->GetX(), e->obj->GetY(), EFFECT_TYPE_DESTROYED, 4);
				Objects->at(int(effect->GetX() / 256)).push_back(effect);

				CGameObject::DeleteElement(e->obj, Objects);
			}
			else
			{
				LPENEMY enemy = dynamic_cast<LPENEMY>(e->obj);
				enemy->ReduceHealth(startThrowingTime);
				enemy->StartFreezing(200);
			}

			if (type == ID_RANGED_WEAPON_DAGGER)
				doesExist = false;
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

bool RangedWeapon::SimonCollision(DWORD dt)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vector<LPGAMEOBJECT> coObjects;

	coObjects.push_back(CSimon::GetInstance());

	CalcPotentialCollisions(&coObjects, coEvents);

	if (coEvents.size() > 0)
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		vx = 0;
		vy = 0;

		if (nx != 0 || ny != 0) 
			return true;
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	return false;
}

void RangedWeapon::Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *Objects)
{
	CGameObject::Update(dt);

	if (type == ID_RANGED_WEAPON_STOPWATCH)
	{
		for (int i = max(0, int(x) / 256 - 1); i <= int(x) / 256 + 1; i++)
			for (UINT j = 0; j < Objects->at(i).size(); j++)
				if (dynamic_cast<LPENEMY>(Objects->at(i).at(j)))
					dynamic_cast<LPENEMY>(Objects->at(i).at(j))->StartFreezing(3000);
		Enemy::StartFreezingAll(3000);
		doesExist = false;
		return;
	}

	if (type == ID_RANGED_WEAPON_HOLY_WATER_IN_POTION || type == ID_RANGED_WEAPON_AXE)
		vy += GRAVITY * dt;

	if (type == ID_RANGED_WEAPON_HOLY_WATER_IN_POTION)
		BricksCollision(dt, Objects);

	if (type == ID_RANGED_WEAPON_HOLY_WATER_ON_FIRE && GetTickCount()>= FireTime)
	{
		SetType(ID_RANGED_WEAPON_HOLY_WATER_IN_POTION);
		doesExist = false;
		return;
	}

	if (type == ID_RANGED_WEAPON_BOOMERANG)
	{
		// Boomerang would have been thrown for 400ms before turning back
		if (GetTickCount() >= startThrowingTime + 400 && !didChangeDirection)
			didChangeDirection = true;

		if (didChangeDirection)
		{
			if (state == 0)
			{
				vx = max(-THROWING_SPEED_X, vx - TURNING_BACK_SPEED_X * dt);
				if (vx <= 0 && !didResetTime)
				{
					didResetTime = true;
					startThrowingTime = GetTickCount(); // Reset throwing time to calculate damage to enemy
				}
			}
			else
			{
				vx = min(THROWING_SPEED_X, vx + TURNING_BACK_SPEED_X * dt);
				if (vx >= 0 && !didResetTime)
				{
					didResetTime = true;
					startThrowingTime = GetTickCount(); // Reset throwing time to calculate damage to enemy
				}
			}
			// If it turning back and meet simon, set it disapear
			if (SimonCollision(dt))
			{
				doesExist = false;
				return;
			}
		}
	}

	DestroyObjectsCollision(dt, Objects);

	// If it is out of screen, set it disapear
	float left, top, right, bottom;
	GetBoundingBox(left, top, right, bottom);

	Camera *camera = Camera::GetInstance();
	int SCREEN_WIDTH = camera->GetScreenWidth();
	int SCREEN_HEIGHT = camera->GetScreenHeight();
	Point pos = camera->PositionOnCamera(right, bottom);

	if (pos.x < 0 || pos.x > SCREEN_WIDTH || pos.y < 0 || pos.y > SCREEN_HEIGHT)
		doesExist = false;

	// No collision occured, proceed normally
	if (doesExist && type != ID_RANGED_WEAPON_HOLY_WATER_ON_FIRE)
	{
		x += dx;
		y += dy;
	}
}

void RangedWeapon::Render()
{
	if (type == ID_RANGED_WEAPON_STOPWATCH) return;
	Camera *camera = Camera::GetInstance();
	int SCREEN_WIDTH = camera->GetScreenWidth();
	Point pos = camera->PositionOnCamera(x, y);

	animations[state]->Render(pos.x, pos.y);
}