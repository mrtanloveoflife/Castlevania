#include "Thorn.h"


void Thorn::LoadThornTextures(int ID_TEX_THORN, LPCWSTR THORN_TEXTURE_PATH, D3DCOLOR THORN_TEXTURE_BACKGROUND_COLOR)
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_THORN, THORN_TEXTURE_PATH, THORN_TEXTURE_BACKGROUND_COLOR);

	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texThorns = textures->Get(ID_TEX_THORN);
	LPANIMATION ani;
	ifstream in;
	in.open(THORN_SPRITES_PATH);
	int n, nState, id, left, top, right, bottom;

	in >> nState;
	for (int i = 0; i < nState; i++)
	{
		in >> n;
		ani = new CAnimation(100);
		for (int j = 0; j < n; j++)
		{
			in >> id >> left >> top >> right >> bottom;
			sprites->Add(id, left, top, right, bottom, texThorns);
			ani->Add(id);
		}

		animations->Add(ID_TEX_THORN + i, ani);
	}

	
}

Thorn::Thorn(float x, float y, int z)
{
	SetPosition(x, y);
	AddAnimation(1100 + z);
	this->z = z;
}
void Thorn::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	int type = animations[0]->GetCurrentFrame();
	if (z == 0)
	{
		if (type > 5) type = 10 - type;
	}
	else
	{
		if (type > 3) type = 7 - type;
	}
	switch (type)
	{
	case THORN_STATE_010:
		left = x;
		top = y;
		right = x + THORN_BBOX_WIDTH;
		bottom = y + THORN_BBOX_HEIGHT_010;
		break;
	case THORN_STATE_19:
		left = x;
		top = y;
		right = x + THORN_BBOX_WIDTH;
		bottom = y + THORN_BBOX_HEIGHT_19;
		break;
	case THORN_STATE_28:
		left = x;
		top = y;
		right = x + THORN_BBOX_WIDTH;
		bottom = y + THORN_BBOX_HEIGHT_28;
		break;
	case THORN_STATE_37:
		left = x;
		top = y;
		right = x + THORN_BBOX_WIDTH;
		bottom = y + THORN_BBOX_HEIGHT_37;
		break;
	case THORN_STATE_46:
		left = x;
		top = y;
		right = x + THORN_BBOX_WIDTH;
		bottom = y + THORN_BBOX_HEIGHT_46;
		break;
	case THORN_STATE_5:
		left = x;
		top = y;
		right = x + THORN_BBOX_WIDTH;
		bottom = y + THORN_BBOX_HEIGHT_5;
		break;

	}
}
void Thorn::Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *Objects)
{
	CSimon *simon = CSimon::GetInstance();
	float simon_x = simon->GetX();


	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);
	int SCREEN_WIDTH = camera->GetScreenWidth();
	int SCREEN_HEIGHT = camera->GetScreenHeight();

	if (pos.x < -THORN_BBOX_WIDTH || pos.x > SCREEN_WIDTH || pos.y > SCREEN_HEIGHT)
		onScreen = false;

}

void Thorn::Render()
{
	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);
	animations[0]->Render(pos.x, pos.y);
}
bool Thorn::DoesExist()
{
	return true;
}

