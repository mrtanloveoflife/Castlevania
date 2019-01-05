#include "MeleeWeapon.h"

MeleeWeapon *MeleeWeapon::__instance = NULL;

MeleeWeapon *MeleeWeapon::GetInstance()
{
	return __instance;
}

MeleeWeapon *MeleeWeapon::GetInstance(int ID_TEX_MELEE_WEAPONS, LPCWSTR MELEE_WEAPONS_TEXTURE_PATH, D3DCOLOR MELEE_WEAPONS_TEXTURE_BACKGROUND_COLOR)
{
	__instance = new MeleeWeapon(ID_TEX_MELEE_WEAPONS, MELEE_WEAPONS_TEXTURE_PATH, MELEE_WEAPONS_TEXTURE_BACKGROUND_COLOR);
	return __instance;
}

MeleeWeapon::MeleeWeapon(int ID_TEX_MELEE_WEAPONS, LPCWSTR MELEE_WEAPONS_TEXTURE_PATH, D3DCOLOR MELEE_WEAPONS_TEXTURE_BACKGROUND_COLOR)
{
	ID_TEX = ID_TEX_MELEE_WEAPONS;
	SetSpeed(0, 0);
	type = 0;

	hitSound = new Sound();
	Mix_Chunk * mix = Mix_LoadWAV(SOUND_HIT);
	hitSound->SetSound(2, mix, 0);

	textures = CTextures::GetInstance();
	textures->Add(ID_TEX_MELEE_WEAPONS, MELEE_WEAPONS_TEXTURE_PATH, MELEE_WEAPONS_TEXTURE_BACKGROUND_COLOR);
	LoadWeaponTextures();
	LoadWeapon();
}

void MeleeWeapon::LoadWeaponTextures()
{
	CSprites * sprites = CSprites::GetInstance();
	LPDIRECT3DTEXTURE9 texWeapons = textures->Get(ID_TEX);
	CAnimations * animations = CAnimations::GetInstance();

	// Add Melee Weapons type 0 to 2
	for (int j = 0; j < 3; j++)
		for (int i = 0; i < 6; i++)
			if (i < 3)
				sprites->Add(40000 + ((j * 2 + 1) * 10) + (i + 1), 76 * i, j * 32, 76 * (i + 1), (j + 1) * 32, texWeapons);
			else sprites->Add(40000 + (j * 2 * 10) + (i - 2), 76 * i, j * 32, 76 * (i + 1), (j + 1) * 32, texWeapons);


	LPANIMATION ani;

	for (int j = 0; j < 3; j++)
	{
		// Right
		ani = new CAnimation(100);
		ani->Add(40001 + (j * 2 * 10));
		ani->Add(40002 + (j * 2 * 10));
		ani->Add(40003 + (j * 2 * 10));
		animations->Add(400 + j * 2, ani);

		// Left
		ani = new CAnimation(100);
		ani->Add(40011 + (j * 2 * 10));
		ani->Add(40012 + (j * 2 * 10));
		ani->Add(40013 + (j * 2 * 10));
		animations->Add(400 + j * 2 + 1, ani);
	}
}

void MeleeWeapon::LoadWeapon()
{
	LPANIMATION ani;

	if (animations.empty())
	{
		ani = CAnimations::GetInstance()->Get(400 + type * 2);
		this->animations.push_back(ani);
		ani = CAnimations::GetInstance()->Get(400 + type * 2 + 1);
		this->animations.push_back(ani);
		return;
	}

	ani = CAnimations::GetInstance()->Get(400 + type * 2);
	this->animations[0] = ani;
	ani = CAnimations::GetInstance()->Get(400 + type * 2 + 1);
	this->animations[1] = ani;
}

void MeleeWeapon::Render()
{
	Camera* camera = Camera::GetInstance();
	CSimon *simon = CSimon::GetInstance();

	int SCREEN_WIDTH = camera->GetScreenWidth();
	int SIMON_STATE = simon->GetState();
	float simon_x, simon_y;
	simon->GetPosition(simon_x, simon_y);

	SetState(SIMON_STATE);

	if (state % 2 == 0) 
		SetPosition(simon_x - 16.0f, simon_y);
	else 
		SetPosition(simon_x - 44.0f, simon_y);

	if (!simon->GetIsAttacking() || simon->GetIsThrowing())
	{
		// reset all animations when simon doesn't attack
		animations[0]->SetCurrentFrame(-1);
		animations[1]->SetCurrentFrame(-1);
		return;
	}

	if (state == SIMON_STATE_CROUCHING_ATTACK_RIGHT || state == SIMON_STATE_CROUCHING_ATTACK_LEFT) SetPosition(x, y + 7);

	int ani = state % 2;

	Point pos = camera->PositionOnCamera(x, y);
	animations[ani]->Render(pos.x, pos.y);
}

void MeleeWeapon::Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *Objects)
{
	CSimon *simon = CSimon::GetInstance();

	if (!simon->GetIsAttacking() || simon->GetIsThrowing()) return;
	if (animations[state % 2]->GetCurrentFrame() != 2) return;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vector<LPGAMEOBJECT> coObjects;

	for (int i = max(0, int(x) / 256 - 1); i <= int(x) / 256 + 1; i++)
		for (UINT j = 0; j < Objects->at(i).size(); j++)
			if (dynamic_cast<Candle *>(Objects->at(i).at(j)) || dynamic_cast<LPENEMY>(Objects->at(i).at(j)) || dynamic_cast<BreakableBrick *>(Objects->at(i).at(j)))
				coObjects.push_back(Objects->at(i).at(j));

	CalcPotentialCollisions(&coObjects, coEvents);

	if (coEvents.size() != 0)
	{
		hitSound->playSound();

		float min_tx, min_ty, nx = 0, ny;
		Effect *effect;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// Collision logic with candles
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Candle *>(e->obj))
			{
				Candle *candle = dynamic_cast<Candle *>(e->obj);

				if (candle->GetItemType() == ITEM_TYPE_WHIP_UPGRADE && type == 2) candle->SetItemType(ITEM_TYPE_HEART_BIG);

				Item * item = new Item(candle->GetX(), candle->GetY(), candle->GetItemType());
				Objects->at(int(item->GetX() / 256)).push_back(item);

				effect = new Effect(e->obj->GetX(), e->obj->GetY(), EFFECT_TYPE_DESTROYED, 4);
				Objects->at(int(effect->GetX() / 256)).push_back(effect);

				CGameObject::DeleteElement(e->obj, Objects);
			}
			else if (dynamic_cast<LPENEMY>(e->obj))
			{
				LPENEMY enemy = dynamic_cast<LPENEMY>(e->obj);
				enemy->ReduceHealth(type + 1, startAttackTime);
				enemy->StartFreezing(200);
			}
			else
			{
				BreakableBrick *breakableBrick = dynamic_cast<BreakableBrick *>(e->obj);

				if (breakableBrick->GetItemType() != -1)
				{
					Item *item = new Item(breakableBrick->GetX(), breakableBrick->GetY(), breakableBrick->GetItemType());
					Objects->at(int(item->GetX() / 256)).push_back(item);
				}
				for (int i = 0; i < 4; i++)
				{
					effect = new Effect(e->obj->GetX(), e->obj->GetY(), 1, i);
					Objects->at(int(effect->GetX() / 256)).push_back(effect);
				}
				CGameObject::DeleteElement(e->obj, Objects);
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void MeleeWeapon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	CSimon *simon = CSimon::GetInstance();

	if (!simon->GetIsAttacking() || simon->GetIsThrowing()) return;

	int ani = state % 2;

	if (ani == 0)
		left = x + 16 + 21;

	switch (type)
	{
	case 0:
		if (ani == 1)
			left = x + 16;
		top = y + 7;
		right = left + MELEE_WEAPON_TYPE_0_BBOX_WIDTH;
		bottom = top + MELEE_WEAPON_TYPE_0_BBOX_HEIGHT;
		break;
	case 1:
		if (ani == 1)
			left = x + 16;
		top = y + 10;
		right = left + MELEE_WEAPON_TYPE_1_BBOX_WIDTH;
		bottom = top + MELEE_WEAPON_TYPE_1_BBOX_HEIGHT;
		break;
	case 2:
		if (ani == 1)
			left = x;
		top = y + 10;
		right = left + MELEE_WEAPON_TYPE_2_BBOX_WIDTH;
		bottom = top + MELEE_WEAPON_TYPE_2_BBOX_HEIGHT;
		break;
	}
}

void MeleeWeapon::Upgrade()
{
	if (type > 2) 
		return; 
	type++;
	LoadWeapon(); 
}