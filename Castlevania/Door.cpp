#include "Door.h"

void Door::LoadDoorTextures(int ID_TEX_DOORS, LPCWSTR DOORS_TEXTURE_PATH, D3DCOLOR DOORS_TEXTURE_BACKGROUND_COLOR)
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_DOORS, DOORS_TEXTURE_PATH, DOORS_TEXTURE_BACKGROUND_COLOR);

	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texDoors = textures->Get(ID_TEX_DOORS);

	LPANIMATION ani;
	ifstream in;
	in.open(DOOR_SPRITES_PATH);
	int n, nState, id, left, top, right, bottom;

	in >> nState;
	for (int i = 0; i < nState; i++)
	{
		in >> n;
		ani = new CAnimation(100);
		for (int j = 0; j < n; j++)
		{
			in >> id >> left >> top >> right >> bottom;
			sprites->Add(id, left, top, right, bottom, texDoors);
			ani->Add(id);
		}

		animations->Add(ID_TEX_DOORS + i, ani);
	}
}

Door::Door(float x, float y, int nx)
{
	SetState(DOOR_STATE_CLOSED);
	SetPosition(x, y);
	this->nx = nx;
	AddAnimation(800);
	AddAnimation(801);
	AddAnimation(802);
	AddAnimation(803);
	isUsed = false;
}

void Door::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x + 8;
	top = y;
	right = left + DOOR_BBOX_WIDTH;
	bottom = top + DOOR_BBOX_HEIGHT;
}

void Door::Render()
{
	Camera *camera = Camera::GetInstance();
	int SCREEN_WIDTH = camera->GetScreenWidth();

	Point pos = camera->PositionOnCamera(x, y);

	if (pos.x >= -16 && pos.x <= SCREEN_WIDTH)
	{
		// if the door has done its opening or closing state, reset animation (to avoid errors) and change its state
		if (animations[state]->GetCurrentFrame() == 2)
		{
			animations[state]->SetCurrentFrame(-1);
			if (state == DOOR_STATE_OPENING)
				SetState(DOOR_STATE_OPENED);
			else
				SetState(DOOR_STATE_CLOSED);
		}

		animations[state]->Render(pos.x, pos.y);
	}
}