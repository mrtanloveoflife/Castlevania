#include "Item.h"

void Item::LoadItemTextures(int ID_TEX_ITEMS, LPCWSTR ITEMS_TEXTURE_PATH, D3DCOLOR ITEMS_TEXTURE_BACKGROUND_COLOR)
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_ITEMS, ITEMS_TEXTURE_PATH, ITEMS_TEXTURE_BACKGROUND_COLOR);

	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texItems = textures->Get(ID_TEX_ITEMS);

	ifstream in;
	in.open(ITEM_SPRITES_PATH);
	int n, nState, id, left, top, right, bottom;
	LPANIMATION ani;

	in >> nState;
	for (int i = 0; i < nState; i++)
	{
		in >> n;
		for (int j = 0; j < n; j++)
		{
			in >> id >> left >> top >> right >> bottom;
			sprites->Add(id, left, top, right, bottom, texItems);
		}
		ani = new CAnimation(50);
		for (int j = n - 1; j >= 0; j--)
			ani->Add(id - j);
		animations->Add(ID_TEX_ITEMS + i, ani);
	}

	in.close();
}

Item::Item(float x, float y, int type)
{
	SetPosition(x, y);
	this->type = type;
	CAnimations * animations = CAnimations::GetInstance();
	AddAnimation(500 + type);
}

void Item::Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *Objects)
{
	vy += GRAVITY * dt;

	CGameObject::Update(dt, Objects);

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

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Item::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	switch (type)
	{
	case ITEM_TYPE_WHIP_UPGRADE:
		right = left + ITEM_TYPE_WHIP_UPGRADE_BBOX_WIDTH;
		bottom = top + ITEM_TYPE_WHIP_UPGRADE_BBOX_HEIGHT;
		break;
	case ITEM_TYPE_DAGGER:
		right = left + ITEM_TYPE_DAGGER_BBOX_WIDTH;
		bottom = top + ITEM_TYPE_DAGGER_BBOX_HEIGHT;
		break;
	case ITEM_TYPE_HEART_BIG:
		right = left + ITEM_TYPE_HEART_BIG_BBOX_WIDTH;
		bottom = top + ITEM_TYPE_HEART_BIG_BBOX_HEIGHT;
		break;
	case ITEM_TYPE_HEART_SMALL:
		right = left + ITEM_TYPE_HEART_SMALL_BBOX_WIDTH;
		bottom = top + ITEM_TYPE_HEART_SMALL_BBOX_HEIGHT;
		break;
	case ITEM_TYPE_MONEY_BAG_400:
		right = left + ITEM_TYPE_MONEY_BAG_BBOX_WIDTH;
		bottom = top + ITEM_TYPE_MONEY_BAG_BBOX_HEIGHT;
		break;
	case ITEM_TYPE_MONEY_BAG_700:
		right = left + ITEM_TYPE_MONEY_BAG_BBOX_WIDTH;
		bottom = top + ITEM_TYPE_MONEY_BAG_BBOX_HEIGHT;
		break;
	case ITEM_TYPE_HOLY_WATER:
		right = left + ITEM_TYPE_HOLY_WATER_BBOX_WIDTH;
		bottom = top + ITEM_TYPE_HOLY_WATER_BBOX_HEIGHT;
		break;
	case ITEM_TYPE_CROSS:
		right = left + ITEM_TYPE_CROSS_BBOX_WIDTH;
		bottom = top + ITEM_TYPE_CROSS_BBOX_HEIGHT;
		break;
	case ITEM_TYPE_STOPWATCH:
		right = left + ITEM_TYPE_STOPWATCH_BBOX_WIDTH;
		bottom = top + ITEM_TYPE_STOPWATCH_BBOX_HEIGHT;
		break;
	case ITEM_TYPE_INVINCIBILITY_POTION:
		right = left + ITEM_TYPE_INVINCIBILITY_POTION_BBOX_WIDTH;
		bottom = top + ITEM_TYPE_INVINCIBILITY_POTION_BBOX_HEIGHT;
		break;
	case ITEM_TYPE_AXE:
		right = left + ITEM_TYPE_AXE_BBOX_WIDTH;
		bottom = top + ITEM_TYPE_AXE_BBOX_HEIGHT;
		break;
	case ITEM_TYPE_POT_ROAST:
		right = left + ITEM_TYPE_POT_ROAST_BBOX_WIDTH;
		bottom = top + ITEM_TYPE_POT_ROAST_BBOX_HEIGHT;
		break;
	case ITEM_TYPE_CRYSTAL:
		right = left + ITEM_TYPE_CRYSTAL_BBOX_WIDTH;
		bottom = top + ITEM_TYPE_CRYSTAL_BBOX_HEIGHT;
		break;
	case ITEM_TYPE_BOOMMERANG:
		right = left + ITEM_TYPE_BOOMERANG_BBOX_WIDTH;
		bottom = top + ITEM_TYPE_BOOMERANG_BBOX_HEIGHT;
		break;
	case ITEM_TYPE_DOUBLE_THROW:
		right = left + ITEM_TYPE_DOUBLE_THROW_BBOX_WIDTH;
		bottom = top + ITEM_TYPE_DOUBLE_THROW_BBOX_HEIGHT;
		break;
	default:
		right = left + 10;
		bottom = top + 10;
		break;
	}
}

void Item::Render()
{
	Camera *camera = Camera::GetInstance();
	int SCREEN_WIDTH = camera->GetScreenWidth();

	Point pos = camera->PositionOnCamera(x, y);

	if (pos.x >= -20 && pos.y <= SCREEN_WIDTH)
		animations[0]->Render(pos.x, pos.y);
}