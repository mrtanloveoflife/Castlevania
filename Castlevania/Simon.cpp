#include "Simon.h"

CSimon *CSimon::__instance = NULL;

CSimon *CSimon::GetInstance()
{
	return __instance;
}

CSimon *CSimon::GetInstance(int ID_TEX_SIMON, LPCWSTR SIMON_TEXTURE_PATH, D3DCOLOR SIMON_TEXTURE_BACKGROUND_COLOR)
{
	__instance = new CSimon(ID_TEX_SIMON, SIMON_TEXTURE_PATH, SIMON_TEXTURE_BACKGROUND_COLOR);
	return __instance;
}

void CSimon::ResetState()
{
	isAttacking = false;
	isThrowing = false;
	isOnStairs = false;
	isUntouchable = false;
	firstHurtJump = false;
	isChangingScene = false;
	isJumpingForward = false;
	isDead = false;
	didGetCrystal = false;
}

CSimon::CSimon(int ID_TEX_SIMON, LPCWSTR SIMON_TEXTURE_PATH, D3DCOLOR SIMON_TEXTURE_BACKGROUND_COLOR)
{
	ResetState();
	FlickeringTime = GetTickCount();
	health = 16;
	maximumThrows = 1;
	numberOfThrows = 0;
	rangedType = ID_RANGED_WEAPON_NONE;

	collectSound = new Sound();
	Mix_Chunk * mix = Mix_LoadWAV(SOUND_COLLECT);
	collectSound->SetSound(4, mix, 0);

	mix = Mix_LoadWAV(SOUND_COLLECT);
	collectSound->SetSound(4, NULL, 0);
	collectSound->SetSound(4, mix, 0);

	collectWeaponSound = new Sound();
	mix = Mix_LoadWAV(SOUND_COLLECT_WEAPON);
	collectWeaponSound->SetSound(5, mix, 0);

	collectCrossSound = new Sound();
	mix = Mix_LoadWAV(SOUND_COLLECT_CROSS);
	collectCrossSound->SetSound(6, mix, 0);

	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_SIMON, SIMON_TEXTURE_PATH, SIMON_TEXTURE_BACKGROUND_COLOR);

	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);

	// readline => id, left, top, right , bottom

	ifstream in;
	in.open(SIMON_SPRITES_PATH);
	int n, nState, id, left, top, right, bottom;
	LPANIMATION ani;

	in >> nState;
	for (int i = 0; i < nState; i++)
	{
		in >> n;
		for (int j = 0; j < n; j++)
		{
			in >> id >> left >> top >> right >> bottom;
			sprites->Add(id, left, top, right, bottom, texSimon);
		}
		// If walking on stairs, do the animation a bit slower
		if (i >= 20 && i <= 23)
			ani = new CAnimation(150);
		else
			ani = new CAnimation(100);
		for (int j = n - 1; j >= 0; j--)
			ani->Add(id - j);
		animations->Add(ID_TEX_SIMON + i, ani);

		AddAnimation(ID_TEX_SIMON + i);
	}

	in.close();
}

void CSimon::SetUntouchable(int time)
{
	isUntouchable = true;
	untouchableTime = GetTickCount() + time;
}

void CSimon::ItemsCollision(vector<vector<LPGAMEOBJECT>> *Objects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vector<LPGAMEOBJECT> coObjects;

	for (int i = max(0, int(x) / 256 - 1); i <= int(x) / 256 + 1; i++)
		for (UINT j = 0; j < Objects->at(i).size(); j++)
			if (dynamic_cast<Item *>(Objects->at(i).at(j)))
				coObjects.push_back(Objects->at(i).at(j));

	CalcPotentialCollisions(&coObjects, coEvents);

	float min_tx, min_ty, nx = 0, ny;

	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

	// Collision logic with items
	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];
		Item *item = dynamic_cast<Item *>(e->obj);
		if (e->nx != 0 || e->ny != 0)
		{
			MeleeWeapon *meleeWeapon = MeleeWeapon::GetInstance();
			Stats *stats = Stats::GetInstance();
			switch (item->GetType())
			{
			case ITEM_TYPE_WHIP_UPGRADE:
				meleeWeapon->Upgrade();
				collectWeaponSound->playSound();
				break;
			case ITEM_TYPE_DAGGER:
				rangedType = ID_RANGED_WEAPON_DAGGER;
				collectWeaponSound->playSound();
				break;
			case ITEM_TYPE_HEART_BIG:
				stats->IncreaseHeart(2);
				collectSound->playSound();
				break;
			case ITEM_TYPE_HEART_SMALL:
				stats->IncreaseHeart(1);
				collectSound->playSound();
				break;
			case ITEM_TYPE_MONEY_BAG_400:
				stats->IncreaseScore(400);
				collectSound->playSound();
				break;
			case ITEM_TYPE_MONEY_BAG_700:
				stats->IncreaseScore(700);
				collectSound->playSound();
				break;
			case ITEM_TYPE_HOLY_WATER:
				rangedType = ID_RANGED_WEAPON_HOLY_WATER_IN_POTION;
				collectWeaponSound->playSound();
				break;
			case ITEM_TYPE_CROSS:
			{
				UINT j;
				for (UINT i = 0; i < Objects->size(); i++)
				{
					j = 0;
					while (j < Objects->at(i).size())
					{
						if (dynamic_cast<LPENEMY>(Objects->at(i).at(j)))
							Objects->at(i).erase(Objects->at(i).begin() + j);
						else
							j++;
					}
				}
				FlickeringTime = GetTickCount() + 500;
				collectCrossSound->playSound();
				break;
			}
			case ITEM_TYPE_STOPWATCH:
				rangedType = ID_RANGED_WEAPON_STOPWATCH;
				collectWeaponSound->playSound();
				break;
			case ITEM_TYPE_INVINCIBILITY_POTION:
				SetUntouchable(5000);
				collectSound->playSound();
				break;
			case ITEM_TYPE_AXE:
				rangedType = ID_RANGED_WEAPON_AXE;
				collectWeaponSound->playSound();
				break;
			case ITEM_TYPE_POT_ROAST:
				health += 6;
				collectSound->playSound();
				break;
			case ITEM_TYPE_CRYSTAL:
				health = 16;
				didGetCrystal = true;
				stats->IncreaseScore(1000);
				collectSound->playSound();
				break;
			case ITEM_TYPE_BOOMMERANG:
				rangedType = ID_RANGED_WEAPON_BOOMERANG;
				collectWeaponSound->playSound();
				break;
			case ITEM_TYPE_DOUBLE_THROW:
				maximumThrows = 2;
				collectSound->playSound();
				break;
			default:
				break;
			}
			CGameObject::DeleteElement(item, Objects);
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

bool CSimon::MovingBlockCollision(vector<vector<LPGAMEOBJECT>> *Objects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vector<LPGAMEOBJECT> coObjects;

	for (int i = max(0, int(x) / 256 - 1); i <= int(x) / 256 + 1; i++)
		for (UINT j = 0; j < Objects->at(i).size(); j++)
			if (dynamic_cast<MovingBlock *>(Objects->at(i).at(j)))
				coObjects.push_back(Objects->at(i).at(j));

	CalcPotentialCollisions(&coObjects, coEvents);

	if (coEvents.size() > 0)
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

	 vx = 0;
		if (ny <= 0)
		{
			x += min_tx * dx + nx * 0.1f;		// nx*0.1f : need to push out a bit to avoid overlapping next frame
			y += min_ty * dy + ny * 0.1f;
			if (ny < 0)
			{
				isFalling = false;
				isJumpingForward = false;
				firstHurtJump = false;

				vy = 0;
				{
					vx += coEventsResult[0]->obj->Getvx();
					x += (vx*2) * dt;
				}
			}
			return true;
		}
		else return false;
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	return false;
}

bool CSimon::BricksCollision(vector<vector<LPGAMEOBJECT>> *Objects)
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
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		if (nx != 0) vx = 0;
		if (ny <= 0)
		{
			x += min_tx * dx + nx * 0.1f;		// nx*0.1f : need to push out a bit to avoid overlapping next frame
			y += min_ty * dy + ny * 0.1f;

			if (ny < 0)
			{
				vy = 0;
				isFalling = false;
				isJumpingForward = false;
				firstHurtJump = false;
			}
			return true;
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	return false;
}

void CSimon::EnemiesCollision(vector<vector<LPGAMEOBJECT>> *Objects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vector<LPGAMEOBJECT> coObjects;

	for (int i = max(0, int(x) / 256 - 1); i <= int(x) / 256 + 1; i++)
		for (UINT j = 0; j < Objects->at(i).size(); j++)
			if (dynamic_cast<LPENEMY>(Objects->at(i).at(j)))
				coObjects.push_back(Objects->at(i).at(j));

	CalcPotentialCollisions(&coObjects, coEvents);

	if (coEvents.size() > 0)
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		SetUntouchable(3000);

		firstHurtJump = true;
		isJumpingForward = true;

		// Disable all other states when simon is hurt
		isOnStairs = false;
		isAttacking = false;
		isThrowing = false;
		isFalling = false;

		this->nx = coEventsResult[0]->obj->nx;
		if (this->nx == 1)
			SetState(SIMON_STATE_JUMPING_RIGHT);
		else
			SetState(SIMON_STATE_JUMPING_LEFT);
		health -= 2;
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

int CSimon::CheckOnStairs(vector<vector<LPGAMEOBJECT>> *Objects, bool isUp)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vector<LPGAMEOBJECT> coObjects;

	for (int i = max(0, int(x) / 256 - 1); i <= int(x) / 256 + 1; i++)
		for (UINT j = 0; j < Objects->at(i).size(); j++)
			if (dynamic_cast<Stairs *>(Objects->at(i).at(j)))
			{
				Stairs *stairs = dynamic_cast<Stairs*>(Objects->at(i).at(j));
				stairs->isUp = isUp;
				coObjects.push_back(stairs);
			}

	CalcPotentialCollisions(&coObjects, coEvents);

	if (coEvents.size() > 0)
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		if (nx != 0 || ny != 0)
		{
			isOnStairs = true;
			currentStairs = dynamic_cast<Stairs *>(coEventsResult[0]->obj);

			float sleft, stop, sright, sbottom;
			currentStairs->GetBoundingBox(sleft, stop, sright, sbottom);
			this->SetPosition((sleft + sright) / 2, y);
			if (currentStairs->GetState() == 1) this->SetPosition(x - 16, y);

			return coEventsResult[0]->obj->GetState();
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	return -1;
}

bool CSimon::CheckOutStairs(bool isUp)
{
	if (isUp)
	{
		float sleft, stop, sright, sbottom;
		float sileft, sitop, siright, sibottom;
		currentStairs->isUp = false;

		currentStairs->GetBoundingBox(sleft, stop, sright, sbottom);
		this->GetBoundingBox(sileft, sitop, siright, sibottom);

		if (sibottom <= sbottom)
		{
			isOnStairs = false;
			vx = 0;
			vy = 0;
			this->SetPosition(x, sbottom - SIMON_WALKING_BBOX_HEIGHT - 0.2f);
			return true;
		}
	}
	else
	{
		float sleft, stop, sright, sbottom;
		float sileft, sitop, siright, sibottom;
		currentStairs->isUp = true;

		currentStairs->GetBoundingBox(sleft, stop, sright, sbottom);
		this->GetBoundingBox(sileft, sitop, siright, sibottom);

		if (sibottom >= stop)
		{
			isOnStairs = false;
			vx = 0;
			vy = 0;
			this->SetPosition(x, stop - SIMON_WALKING_BBOX_HEIGHT - 0.2f);
			return true;
		}
	}

	return false;
}

void CSimon::DoorsCollision(vector<vector<LPGAMEOBJECT>> *Objects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vector<LPGAMEOBJECT> coObjects;

	for (int i = max(0, int(x) / 256 - 1); i <= int(x) / 256 + 1; i++)
		for (UINT j = 0; j < Objects->at(i).size(); j++)
			if (dynamic_cast<Door *>(Objects->at(i).at(j)))
				coObjects.push_back(Objects->at(i).at(j));

	CalcPotentialCollisions(&coObjects, coEvents);

	if (coEvents.size() > 0)
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		Door *door = dynamic_cast<Door *>(coEventsResult[0]->obj);

		x += min_tx * dx + nx * 0.1f;		// nx*0.1f : need to push out a bit to avoid overlapping next frame

		vx = 0;
		x -= dx;

		isJumpingForward = false;
		isAttacking = false;

		// if simon interact the door the right way
		if (nx == door->nx)
		{
			isChangingScene = true;
			if (!isFalling)
				if (state % 2 == 0)
					SetState(SIMON_STATE_IDLE_RIGHT);
				else
					SetState(SIMON_STATE_IDLE_LEFT);
			door->SetIsUsed(true);

			// delete all enemies when changing scene
			UINT j;
			for (UINT i = 0; i < Objects->size(); i++)
			{
				j = 0;
				while (j < Objects->at(i).size())
				{
					if (dynamic_cast<LPENEMY>(Objects->at(i).at(j)) && 
						!dynamic_cast<Thorn *>(Objects->at(i).at(j)) && !dynamic_cast<SnakeHead *>(Objects->at(i).at(j)) && !dynamic_cast<Soldier *>(Objects->at(i).at(j)) &&
						!dynamic_cast<Panther *>(Objects->at(i).at(j)) && !dynamic_cast<Skull *>(Objects->at(i).at(j)) && !dynamic_cast<Bat2 *>(Objects->at(i).at(j)))
					{
						delete Objects->at(i).at(j);
						Objects->at(i).erase(Objects->at(i).begin() + j);
					}
					else j++;
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CSimon::Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *Objects)
{
	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);
	Stats *stats = Stats::GetInstance();
	if ((health <= 0 && (state == SIMON_STATE_IDLE_RIGHT || state == SIMON_STATE_IDLE_LEFT))
		|| pos.y > camera->GetScreenHeight() + 50 || stats->GetTimer() <= 0)
	{
		SetState(SIMON_STATE_DIE_RIGHT);
		isDead = true;
		return;
	}


	// Calculate dx, dy 
	CGameObject::Update(dt);

	if (isUntouchable && GetTickCount() >= untouchableTime)
		isUntouchable = false;

	if (x < 0) x = 0;

	// check attacking time
	DWORD now = GetTickCount();
	if (now - SIMON_ATTACKING_TIME > attackingTime || !isAttacking)
	{
		isAttacking = false;
		// reset current frame of all attacking animations
		animations[SIMON_STATE_ATTACK_LEFT]->SetCurrentFrame(-1);
		animations[SIMON_STATE_ATTACK_RIGHT]->SetCurrentFrame(-1);
		animations[SIMON_STATE_CROUCHING_ATTACK_LEFT]->SetCurrentFrame(-1);
		animations[SIMON_STATE_CROUCHING_ATTACK_RIGHT]->SetCurrentFrame(-1);
		animations[SIMON_STATE_THROW_LEFT]->SetCurrentFrame(-1);
		animations[SIMON_STATE_THROW_RIGHT]->SetCurrentFrame(-1);
		animations[SIMON_STATE_CROUCHING_THROW_LEFT]->SetCurrentFrame(-1);
		animations[SIMON_STATE_CROUCHING_THROW_RIGHT]->SetCurrentFrame(-1);
		animations[SIMON_STATE_ATTACKING_UPSTAIRS_RIGHT]->SetCurrentFrame(-1);
		animations[SIMON_STATE_ATTACKING_UPSTAIRS_LEFT]->SetCurrentFrame(-1);
		animations[SIMON_STATE_ATTACKING_DOWNSTAIRS_RIGHT]->SetCurrentFrame(-1);
		animations[SIMON_STATE_ATTACKING_DOWNSTAIRS_LEFT]->SetCurrentFrame(-1);
		animations[SIMON_STATE_THROWING_UPSTAIRS_RIGHT]->SetCurrentFrame(-1);
		animations[SIMON_STATE_THROWING_UPSTAIRS_LEFT]->SetCurrentFrame(-1);
		animations[SIMON_STATE_THROWING_DOWNSTAIRS_RIGHT]->SetCurrentFrame(-1);
		animations[SIMON_STATE_THROWING_DOWNSTAIRS_LEFT]->SetCurrentFrame(-1);

		if (isThrowing)
		{
			Stats *stats = Stats::GetInstance();
			if (stats->GetHeart() > 0 && CanThrow() && rangedType != ID_RANGED_WEAPON_NONE)
			{
				RangedWeapon *rangedWeapon = new RangedWeapon(rangedType);
				stats->ReduceHeart();
				Objects->at(int(rangedWeapon->GetX() / 256)).push_back(rangedWeapon);
				numberOfThrows++;
			}
			isThrowing = false;
		}
	}
	// Handling Doors
	if (!isChangingScene)
		DoorsCollision(Objects);

	// simple fall down
	if (!isOnStairs)
		vy += GRAVITY * dt;

	if (isOnStairs || (!BricksCollision(Objects) && (!MovingBlockCollision(Objects))))
	{
		x += dx;
		y += dy;
	}


	// Handling Item
	ItemsCollision(Objects);

	// Handling Enemies
	if (!isUntouchable)
		EnemiesCollision(Objects);

	if (!isFalling) vx = 0;
	health = min(16, health);
}

void CSimon::Render()
{
	int ani = state;

	if (isFalling && !isAttacking) ani = SIMON_STATE_JUMPING_RIGHT;

	if ((ani == SIMON_STATE_JUMPING_RIGHT) || (ani == SIMON_STATE_JUMPING_LEFT))
	{
		if (nx > 0)
		{
			ani = SIMON_STATE_JUMPING_RIGHT;
			if (isJumpingForward) vx = SIMON_WALKING_SPEED;
			if (firstHurtJump) ani = SIMON_STATE_HURTING_RIGHT;
			;
		}
		else
		{
			ani = SIMON_STATE_JUMPING_LEFT;
			if (isJumpingForward) vx = -SIMON_WALKING_SPEED;
			if (firstHurtJump) ani = SIMON_STATE_HURTING_LEFT;
		}
	}

	if ((ani == SIMON_STATE_CROUCH_RIGHT) || (ani == SIMON_STATE_CROUCH_LEFT))
	{
		if (nx > 0) ani = SIMON_STATE_CROUCH_RIGHT;
		else ani = SIMON_STATE_CROUCH_LEFT;
	}

	if ((ani == SIMON_STATE_IDLE_RIGHT) || (ani == SIMON_STATE_IDLE_LEFT))
	{
		if (nx > 0) ani = SIMON_STATE_IDLE_RIGHT;
		else ani = SIMON_STATE_IDLE_LEFT;
	}

	if ((ani == SIMON_STATE_ATTACK_RIGHT) || (ani == SIMON_STATE_ATTACK_LEFT))
	{
		if (nx > 0) ani = SIMON_STATE_ATTACK_RIGHT;
		else ani = SIMON_STATE_ATTACK_LEFT;
	}

	if ((ani == SIMON_STATE_CROUCHING_ATTACK_RIGHT) || (ani == SIMON_STATE_CROUCHING_ATTACK_LEFT))
	{
		if (nx > 0) ani = SIMON_STATE_CROUCHING_ATTACK_RIGHT;
		else ani = SIMON_STATE_CROUCHING_ATTACK_LEFT;
	}

	if ((ani == SIMON_STATE_THROW_RIGHT) || (ani == SIMON_STATE_THROW_LEFT))
	{
		if (nx > 0) ani = SIMON_STATE_THROW_RIGHT;
		else ani = SIMON_STATE_THROW_LEFT;
	}

	if ((ani == SIMON_STATE_CROUCHING_THROW_RIGHT) || (ani == SIMON_STATE_CROUCHING_THROW_LEFT))
	{
		if (nx > 0) ani = SIMON_STATE_CROUCHING_THROW_RIGHT;
		else ani = SIMON_STATE_CROUCHING_THROW_LEFT;
	}

	if ((ani == SIMON_STATE_ATTACKING_UPSTAIRS_RIGHT) || (ani == SIMON_STATE_ATTACKING_UPSTAIRS_LEFT))
	{
		if (nx > 0) ani = SIMON_STATE_ATTACKING_UPSTAIRS_RIGHT;
		else ani = SIMON_STATE_ATTACKING_UPSTAIRS_LEFT;
	}

	if ((ani == SIMON_STATE_ATTACKING_DOWNSTAIRS_RIGHT) || (ani == SIMON_STATE_ATTACKING_DOWNSTAIRS_LEFT))
	{
		if (nx > 0) ani = SIMON_STATE_ATTACKING_DOWNSTAIRS_RIGHT;
		else ani = SIMON_STATE_ATTACKING_DOWNSTAIRS_LEFT;
	}

	if ((ani == SIMON_STATE_THROWING_UPSTAIRS_RIGHT) || (ani == SIMON_STATE_THROWING_UPSTAIRS_LEFT))
	{
		if (nx > 0) ani = SIMON_STATE_THROWING_UPSTAIRS_RIGHT;
		else ani = SIMON_STATE_THROWING_UPSTAIRS_LEFT;
	}

	if ((ani == SIMON_STATE_THROWING_DOWNSTAIRS_RIGHT) || (ani == SIMON_STATE_THROWING_DOWNSTAIRS_LEFT))
	{
		if (nx > 0) ani = SIMON_STATE_THROWING_DOWNSTAIRS_RIGHT;
		else ani = SIMON_STATE_THROWING_DOWNSTAIRS_LEFT;
	}

	if ((ani == SIMON_STATE_DIE_RIGHT) || (ani == SIMON_STATE_DIE_LEFT))
	{
		if (nx > 0) ani = SIMON_STATE_DIE_LEFT;
		else ani = SIMON_STATE_DIE_RIGHT;
	}

	state = ani;

	// Rendering depends on screen
	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);

	if ((isUntouchable && GetTickCount() % 5) || !isUntouchable)
		if (!isAttacking)
			animations[ani]->Render(pos.x, pos.y);
		else if (ani % 2 == 0) animations[ani]->Render(pos.x - 8, pos.y);
		else animations[ani]->Render(pos.x - 6, pos.y);
}

void CSimon::SetState(int state)
{
	CGameObject::SetState(state);

	MeleeWeapon *meleeWeapon = MeleeWeapon::GetInstance();
	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_JUMPING_LEFT:
		if (!isFalling)
		{
			isFalling = true;
			vy = -SIMON_JUMP_SPEED_Y;
		}
		break;
	case SIMON_STATE_JUMPING_RIGHT:
		if (!isFalling)
		{
			isFalling = true;
			vy = -SIMON_JUMP_SPEED_Y;
		}
		break;
	case SIMON_STATE_IDLE_RIGHT:
		vx = 0;
		break;
	case SIMON_STATE_IDLE_LEFT:
		vx = 0;
		break;
	case SIMON_STATE_CROUCH_RIGHT:
		vx = 0;
		break;
	case SIMON_STATE_CROUCH_LEFT:
		vx = 0;
		break;
	case SIMON_STATE_ATTACK_RIGHT:
		if (!isJumpingForward) vx = 0;
		isAttacking = true;
		attackingTime = GetTickCount();
		meleeWeapon->ResetStartAttackingTime();
		break;
	case SIMON_STATE_ATTACK_LEFT:
		if (!isJumpingForward) vx = 0;
		isAttacking = true;
		attackingTime = GetTickCount();
		meleeWeapon->ResetStartAttackingTime();
		break;
	case SIMON_STATE_CROUCHING_ATTACK_RIGHT:
		vx = 0;
		isAttacking = true;
		attackingTime = GetTickCount();
		meleeWeapon->ResetStartAttackingTime();
		break;
	case SIMON_STATE_CROUCHING_ATTACK_LEFT:
		vx = 0;
		isAttacking = true;
		attackingTime = GetTickCount();
		meleeWeapon->ResetStartAttackingTime();
		break;
	case SIMON_STATE_THROW_RIGHT:
		if (!isJumpingForward) vx = 0;
		isAttacking = true;
		isThrowing = true;
		attackingTime = GetTickCount();
		break;
	case SIMON_STATE_THROW_LEFT:
		if (!isJumpingForward) vx = 0;
		isAttacking = true;
		isThrowing = true;
		attackingTime = GetTickCount();
		break;
	case SIMON_STATE_CROUCHING_THROW_RIGHT:
		vx = 0;
		isAttacking = true;
		isThrowing = true;
		attackingTime = GetTickCount();
		break;
	case SIMON_STATE_CROUCHING_THROW_LEFT:
		vx = 0;
		isAttacking = true;
		isThrowing = true;
		attackingTime = GetTickCount();
		break;
	case SIMON_STATE_IDLE_UPSTAIRS_RIGHT:
		vx = 0;
		vy = 0;
		break;
	case SIMON_STATE_IDLE_UPSTAIRS_LEFT:
		vx = 0;
		vy = 0;
		break;
	case SIMON_STATE_IDLE_DOWNSTAIRS_RIGHT:
		vx = 0;
		vy = 0;
		break;
	case SIMON_STATE_IDLE_DOWNSTAIRS_LEFT:
		vx = 0;
		vy = 0;
		break;
	case SIMON_STATE_WALKING_UPSTAIRS_RIGHT:
		vx = SIMON_WALKING_STAIRS_SPEED;
		vy = -SIMON_WALKING_STAIRS_SPEED;
		isOnStairs = true;
		nx = 1;
		ny = -1;
		break;
	case SIMON_STATE_WALKING_UPSTAIRS_LEFT:
		vx = -SIMON_WALKING_STAIRS_SPEED;
		vy = -SIMON_WALKING_STAIRS_SPEED;
		isOnStairs = true;
		nx = -1;
		ny = -1;
		break;
	case SIMON_STATE_WALKING_DOWNSTAIRS_RIGHT:
		vx = SIMON_WALKING_STAIRS_SPEED;
		vy = SIMON_WALKING_STAIRS_SPEED;
		isOnStairs = true;
		nx = 1;
		ny = 1;
		break;
	case SIMON_STATE_WALKING_DOWNSTAIRS_LEFT:
		vx = -SIMON_WALKING_STAIRS_SPEED;
		vy = SIMON_WALKING_STAIRS_SPEED;
		isOnStairs = true;
		nx = -1;
		ny = 1;
		break;
	case SIMON_STATE_ATTACKING_UPSTAIRS_RIGHT:
		vx = 0;
		vy = 0;
		isAttacking = true;
		attackingTime = GetTickCount();
		meleeWeapon->ResetStartAttackingTime();
		break;
	case SIMON_STATE_ATTACKING_UPSTAIRS_LEFT:
		vx = 0;
		vy = 0;
		isAttacking = true;
		attackingTime = GetTickCount();
		meleeWeapon->ResetStartAttackingTime();
		break;
	case SIMON_STATE_ATTACKING_DOWNSTAIRS_RIGHT:
		vx = 0;
		vy = 0;
		isAttacking = true;
		attackingTime = GetTickCount();
		meleeWeapon->ResetStartAttackingTime();
		break;
	case SIMON_STATE_ATTACKING_DOWNSTAIRS_LEFT:
		vx = 0;
		vy = 0;
		isAttacking = true;
		attackingTime = GetTickCount();
		meleeWeapon->ResetStartAttackingTime();
		break;
	case SIMON_STATE_THROWING_UPSTAIRS_RIGHT:
		vx = 0;
		vy = 0;
		isAttacking = true;
		isThrowing = true;
		attackingTime = GetTickCount();
		break;
	case SIMON_STATE_THROWING_UPSTAIRS_LEFT:
		vx = 0;
		vy = 0;
		isAttacking = true;
		isThrowing = true;
		attackingTime = GetTickCount();
		break;
	case SIMON_STATE_THROWING_DOWNSTAIRS_RIGHT:
		vx = 0;
		vy = 0;
		isAttacking = true;
		isThrowing = true;
		attackingTime = GetTickCount();
		break;
	case SIMON_STATE_THROWING_DOWNSTAIRS_LEFT:
		vx = 0;
		vy = 0;
		isAttacking = true;
		isThrowing = true;
		attackingTime = GetTickCount();
		break;
	case SIMON_STATE_DIE_RIGHT:
		vx = 0;
		vy = 0;
		ResetState();
		break;
	case SIMON_STATE_DIE_LEFT:
		vx = 0;
		vy = 0;
		ResetState();
		break;
	case SIMON_STATE_TURN_BACK:
		vx = 0;
		vy = 0;
		nx = 1;
		ResetState();
		break;
	}

}

void CSimon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == SIMON_STATE_CROUCH_LEFT || state == SIMON_STATE_CROUCH_RIGHT || state == SIMON_STATE_CROUCHING_ATTACK_RIGHT || state == SIMON_STATE_CROUCHING_ATTACK_LEFT ||
		state == SIMON_STATE_CROUCHING_THROW_RIGHT || state == SIMON_STATE_CROUCHING_THROW_LEFT || isFalling)
	{
		left = x;
		top = y + 8;
		right = left + SIMON_CROUCHING_BBOX_WIDTH;
		bottom = top + SIMON_CROUCHING_BBOX_HEIGHT;
	}
	else
	{
		left = x;
		top = y;
		right = left + SIMON_WALKING_BBOX_WIDTH;
		bottom = top + SIMON_WALKING_BBOX_HEIGHT;
	}
}

bool CSimon::IsUpstairs()
{
	switch (state)
	{
	case SIMON_STATE_IDLE_UPSTAIRS_RIGHT:
		return true;
	case SIMON_STATE_IDLE_UPSTAIRS_LEFT:
		return true;
	case SIMON_STATE_WALKING_UPSTAIRS_RIGHT:
		return true;
	case SIMON_STATE_WALKING_UPSTAIRS_LEFT:
		return true;
	case SIMON_STATE_ATTACKING_UPSTAIRS_RIGHT:
		return true;
	case SIMON_STATE_ATTACKING_UPSTAIRS_LEFT:
		return true;
	case SIMON_STATE_THROWING_UPSTAIRS_RIGHT:
		return true;
	case SIMON_STATE_THROWING_UPSTAIRS_LEFT:
		return true;
	}
	return false;
}