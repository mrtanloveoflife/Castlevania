#include "BreakableBrick.h"

void BreakableBrick::LoadBreakableBrickTextures(int ID_TEX_BREAKABLE_BRICKS, LPCWSTR BREAKABLE_BRICKS_TEXTURE_PATH, D3DCOLOR BREAKABLE_BRICKS_TEXTURE_BACKGROUND_COLOR)
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_BREAKABLE_BRICKS, BREAKABLE_BRICKS_TEXTURE_PATH, BREAKABLE_BRICKS_TEXTURE_BACKGROUND_COLOR);

	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texBricks = textures->Get(ID_TEX_BREAKABLE_BRICKS);

	LPANIMATION ani;
	ifstream in;
	in.open(BREAKABLE_SPRITES_PATH);
	int n, nState, id, left, top, right, bottom;

	in >> nState;
	for (int i = 0; i < nState; i++)
	{
		in >> n;
		ani = new CAnimation(100);
		for (int j = 0; j < n; j++)
		{
			in >> id >> left >> top >> right >> bottom;
			sprites->Add(id, left, top, right, bottom, texBricks);
			ani->Add(id);
		}

		animations->Add(ID_TEX_BREAKABLE_BRICKS + i, ani);
	}
	
}

BreakableBrick::BreakableBrick(float x, float y, int type, int itemType)
{
	SetPosition(x, y);
	AddAnimation(1200 + type);
	this->itemType = itemType;
}

void BreakableBrick::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = left + BREAKABLE_BRICK_BBOX_WIDTH;
	bottom = top + BREAKABLE_BRICK_BBOX_HEIGHT;
}

void BreakableBrick::Render()
{
	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);
	int SCREEN_WIDTH = camera->GetScreenWidth();
	if (pos.x >= -BREAKABLE_BRICK_BBOX_WIDTH && pos.x <= SCREEN_WIDTH)
		animations[0]->Render(pos.x, pos.y);
}

BreakableBrick::~BreakableBrick()
{
}
