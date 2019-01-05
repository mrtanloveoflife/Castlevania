#include "Candle.h"

void Candle::LoadCandleTextures(int ID_TEX_CANDLES, LPCWSTR CANDLES_TEXTURE_PATH, D3DCOLOR CANDLES_TEXTURE_BACKGROUND_COLOR)
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_CANDLES, CANDLES_TEXTURE_PATH, CANDLES_TEXTURE_BACKGROUND_COLOR);

	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texCandles = textures->Get(ID_TEX_CANDLES);
	LPANIMATION ani;
	ifstream in;
	in.open(CANDLE_SPRITES_PATH);
	int n, nState, id, left, top, right, bottom;

	in >> nState;
	for (int i = 0; i < nState; i++)
	{
		in >> n;
		ani = new CAnimation(100);
		for (int j = 0; j < n; j++)
		{
			in >> id >> left >> top >> right >> bottom;
			sprites->Add(id, left, top, right, bottom, texCandles);
			ani->Add(id);
		}

		animations->Add(ID_TEX_CANDLES + i, ani);
	}
	
}

Candle::Candle(float x, float y, int type, int itemType)
{
	SetPosition(x, y);
	this->type = type;
	this->itemType = itemType;
	if (type==CANDLE_TYPE_BIG)
		AddAnimation(300);
	else 
		AddAnimation(301);
}

void Candle::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	switch (type)
	{
	case CANDLE_TYPE_BIG:
		left = x;
		top = y;
		right = left + CANDLE_BIG_BBOX_WIDTH;
		bottom = top + CANDLE_BIG_BBOX_HEIGHT;
		break;
	case CANDLE_TYPE_SMALL:
		left = x;
		top = y;
		right = left + CANDLE_SMALL_BBOX_WIDTH;
		bottom = top + CANDLE_SMALL_BBOX_HEIGHT;
		break;
	}
}

void Candle::Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *Objects)
{

}

void Candle::Render()
{
	Camera *camera = Camera::GetInstance();
	int SCREEN_WIDTH = camera->GetScreenWidth();

	Point pos = camera->PositionOnCamera(x, y);

	if (pos.x >= -16 && pos.x <= SCREEN_WIDTH)
		animations[0]->Render(pos.x, pos.y);
}