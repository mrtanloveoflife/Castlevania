#include "Effect.h"

void Effect::LoadEffectTextures(int ID_TEX_EFFECTS, LPCWSTR EFFECTS_TEXTURE_PATH, D3DCOLOR EFFECTS_TEXTURE_BACKGROUND_COLOR)
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_EFFECTS, EFFECTS_TEXTURE_PATH, EFFECTS_TEXTURE_BACKGROUND_COLOR);

	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texEffects = textures->Get(ID_TEX_EFFECTS);
	LPANIMATION ani;
	ifstream in;
	in.open(EFFECT_SPRITES_PATH);
	int n, nState, id, left, top, right, bottom;

	in >> nState;
	for (int i = 0; i < nState; i++)
	{
		in >> n;
		ani = new CAnimation(100);
		for (int j = 0; j < n; j++)
		{
			in >> id >> left >> top >> right >> bottom;
			sprites->Add(id, left, top, right, bottom, texEffects);
			ani->Add(id);
		}

		animations->Add(ID_TEX_EFFECTS + i, ani);
	}
}

Effect::Effect(float x, float y, int type, int vType)
{
	SetPosition(x, y);
	this->type = type;
	AddAnimation(1300 + type);
	existTime = GetTickCount() + 400;
	switch (vType)
	{
	case 0: 
		SetSpeed(EFFECT_SPEED_X_TYPE_0, EFFECT_SPEED_Y_TYPE_0);
		break;
	case 1:
		SetSpeed(EFFECT_SPEED_X_TYPE_1, EFFECT_SPEED_Y_TYPE_1);
		break;
	case 2:
		SetSpeed(EFFECT_SPEED_X_TYPE_2, EFFECT_SPEED_Y_TYPE_2);
		break;
	case 3:
		SetSpeed(EFFECT_SPEED_X_TYPE_3, EFFECT_SPEED_Y_TYPE_3);
		break;
	case 4:
		SetSpeed(EFFECT_SPEED_X_TYPE_4, EFFECT_SPEED_Y_TYPE_4);
		break;
	}
}

void Effect::Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *Objects)
{
	if (type < 2)
	{
		CGameObject::Update(dt);
		vy += GRAVITY * dt;

		x += dx;
		y += dy;
	}
}

void Effect::Render()
{
	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);

	animations[0]->Render(pos.x, pos.y);
}

bool Effect::DoesExist()
{
	if (type < 2)
	{
		Camera *camera = Camera::GetInstance();
		int SCREEN_WIDTH = camera->GetScreenWidth();
		int SCREEN_HEIGHT = camera->GetScreenHeight();
		Point pos = camera->PositionOnCamera(x, y);
		if (pos.x < -7 || pos.x > SCREEN_WIDTH || pos.y > SCREEN_HEIGHT)
			return false;
	}
	else if (GetTickCount() >= existTime)
	{
		animations[0]->SetCurrentFrame(-1);
		return false;
	}
	return true;
}

Effect::~Effect()
{
}
