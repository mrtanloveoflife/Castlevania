#include "MovingBlock.h"
#include "Textures.h"

MovingBlock::MovingBlock(float y, float xmin, float xmax)
{	
	SetMaxMin(xmin, xmax);
	SetPosition(xmin, y);
	AddAnimation(1000);
	SetSpeed(SPEED_MOVING_BLOCK, 0);
	this->nx = 1;
}
void MovingBlock::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + MOVINGBLOCK_BBOX_WIDTH;
	bottom = y + MOVINGBLOCK_BBOX_HEIGHT;
}

void MovingBlock::LoadMovingBlockTextures(int ID_TEX_MOVINGBLOCK, LPCWSTR MOVINGBLOCK_TEXTURE_PATH, D3DCOLOR MOVINGBLOCK_TEXTURE_BACKGROUND_COLOR)
{
	CTextures * textures = CTextures::GetInstance();
	textures->Add(ID_TEX_MOVINGBLOCK, MOVINGBLOCK_TEXTURE_PATH, MOVINGBLOCK_TEXTURE_BACKGROUND_COLOR);
	CSprites * sprites = CSprites::GetInstance();
	LPDIRECT3DTEXTURE9 texMovingBlock = textures->Get(ID_TEX_MOVINGBLOCK);
	CAnimations * animations = CAnimations::GetInstance();

	sprites->Add(90001, 0, 0, 32, 7, texMovingBlock);
	LPANIMATION ani;
	ani = new CAnimation(100);
	ani->Add(90001);
	animations->Add(1000, ani);
}
void MovingBlock::Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects)
{
	if ((this->x > xmax - 32 && nx == 1) || (this->x < xmin && nx == -1))
	{
		this->nx = -this->nx;
		this->vx = -vx;
	}
	CGameObject::Update(dt);
	x += dx;
	y += dy;
}
void MovingBlock::Render()
{
	Camera *camera = Camera::GetInstance();
	Point pos = camera->PositionOnCamera(x, y);

	animations[state]->Render(pos.x, pos.y);
}
