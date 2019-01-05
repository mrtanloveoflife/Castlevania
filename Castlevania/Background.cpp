#include "Background.h"

Background::Background(int ID_TEX_BACKGROUND, D3DCOLOR BACKGROUND_TEXTURE_BACKGROUND_COLOR, int stage)
{
	CTextures * textures = CTextures::GetInstance();

	LPCWSTR BACKGROUND_TEXTURE_PATH = L"";
	LPCWSTR BACKGROUND_INFO_PATH = L"";

	switch (stage)
	{
	case 0:
		BACKGROUND_TEXTURE_PATH = BACKGROUND_STAGE_0_TEXTURE_PATH;
		break;
	case 1:
		BACKGROUND_TEXTURE_PATH = BACKGROUND_STAGE_1_TEXTURE_PATH;
		BACKGROUND_INFO_PATH = BACKGROUND_STAGE_1_INFO_PATH;
		break;
	case 2:
		BACKGROUND_TEXTURE_PATH = BACKGROUND_STAGE_2_TEXTURE_PATH;
		BACKGROUND_INFO_PATH = BACKGROUND_STAGE_2_INFO_PATH;
		break;
	case 3:
		BACKGROUND_TEXTURE_PATH = BACKGROUND_STAGE_3_TEXTURE_PATH;
		BACKGROUND_INFO_PATH = BACKGROUND_STAGE_3_INFO_PATH;
		break;
	case 4:
		BACKGROUND_TEXTURE_PATH = BACKGROUND_STAGE_4_TEXTURE_PATH;
		BACKGROUND_INFO_PATH = BACKGROUND_STAGE_4_INFO_PATH;
		break;
	case 5:
		BACKGROUND_TEXTURE_PATH = BACKGROUND_STAGE_5_TEXTURE_PATH;
		BACKGROUND_INFO_PATH = BACKGROUND_STAGE_5_INFO_PATH;
		break;
	case 6:
		BACKGROUND_TEXTURE_PATH = BACKGROUND_STAGE_6_TEXTURE_PATH;
		BACKGROUND_INFO_PATH = BACKGROUND_STAGE_6_INFO_PATH;
		break;
	case 7:
		BACKGROUND_TEXTURE_PATH = BACKGROUND_STAGE_7_TEXTURE_PATH;
		BACKGROUND_INFO_PATH = BACKGROUND_STAGE_7_INFO_PATH;
		break;
	case 8:
		BACKGROUND_TEXTURE_PATH = BACKGROUND_TITLE_SCENE_TEXTURE_PATH;
		break;
	case 9:
		BACKGROUND_TEXTURE_PATH = BACKGROUND_DEATH_SCENE_TEXTURE_PATH;
		break;
	default:
		break;
	}

	textures->Add(ID_TEX_BACKGROUND, BACKGROUND_TEXTURE_PATH, BACKGROUND_TEXTURE_BACKGROUND_COLOR);

	CSprites * sprites = CSprites::GetInstance();

	LPDIRECT3DTEXTURE9 texBackground = textures->Get(ID_TEX_BACKGROUND);

	this->stage = stage;

	if (stage > 0 && stage < 8)
	{
		ifstream in;
		int elementNumber, columnNumber;
		in.open(BACKGROUND_SOURCE_INFO_PATH);

		// ignore other number, just get the number we need
		for (int i = 1; i < stage; i++)
			in >> elementNumber >> columnNumber;
		in >> elementNumber >> columnNumber;

		in.close();
		
		// Read tiled
		for (int i = 0; i < elementNumber; i++)
			sprites->Add(20000 + i + stage * 1000, i % columnNumber * 16, i / columnNumber * 16, (i % columnNumber + 1) * 16, (i / columnNumber + 1) * 16, texBackground);

		int n_tiled_per_array, element;

		in.open(BACKGROUND_INFO_PATH);

		in >> n_arrays;

		for (int i = 0; i < n_arrays; i++)
		{
			vector<int> tiled_array;
			in >> n_tiled_per_array;
			for (int j = 0; j < n_tiled_per_array; j++)
			{
				in >> element;
				tiled_array.push_back(element);
			}
			this->tiled_arrays.push_back(tiled_array);
		}
		in.close();
	}
	else
		sprites->Add(20000 + stage * 1000, 0, 0, 270, 270, texBackground);
}

void Background::Render()
{
	CSprites * sprites = CSprites::GetInstance();
	int sprites_id;

	if (stage == 0 || stage == 8 || stage == 9)
	{
		sprites_id = 20000 + stage * 1000;
		sprites->Get(sprites_id)->Draw(0, 0);
		return;
	}

	Camera *camera = Camera::GetInstance();
	int screen_width = camera->GetScreenWidth();
	float camera_x = camera->GetX();

	Point pos;
	for (int i = 0; i < n_arrays; i++)
		if (i * 16 * 16 - (camera_x - screen_width / 2) <= screen_width && i * 16 * 16 + 16 * 16 - (camera_x - screen_width / 2) >= 0)
			for (UINT j = 0; j < tiled_arrays.at(i).size(); j++)
			{
				pos = camera->PositionOnCamera(i * 16 * 16 + j % 16 * 16 * 1.0f, j / 16 * 16 * 1.0f);
				sprites_id = 20000 + stage * 1000 + tiled_arrays.at(i).at(j) - 1;

				if (pos.x >= -16 && pos.x <= screen_width)
					sprites->Get(sprites_id)->Draw(pos.x, pos.y);
			}
}

int Background::ChangeStage(int currentStage)
{
	CSimon *simon = CSimon::GetInstance();
	bool isStairs = simon->GetIsOnStairs();
	switch (currentStage)
	{
	case 1:
		if (simon->GetX() >= 680) return 2;
		break;
	case 2:
	{
		if ((simon->GetY() + SIMON_WALKING_BBOX_HEIGHT > 175) && isStairs) return 3;
		if (simon->GetDidGetCrystal())
		{
			simon->SetDidGetCrystal(false);
			return 4;
		}
		break;
	}
	case 3:
		if (simon->GetY() < -10 && isStairs) return 2;
		break;
	case 4:
		if (simon->GetY() < -10 && isStairs) return 5;
		break;
	case 5:
		if (simon->GetY() > 145 && simon->GetX() > 1140 && simon->GetX() < 1150 && isStairs) return 4;
		if (simon->GetY() < -10 && isStairs) return 6;
		break;
	case 6:
		if (simon->GetY() > 140 && simon->GetX() > 825 && simon->GetX() < 835 && isStairs) return 5;
		if (simon->GetY() < -10 && isStairs) return 7;
		break;
	case 7:
		if (simon->GetY() > 143 && simon->GetX() > 665 && simon->GetX() < 675 && isStairs) return 6;
		if (simon->GetDidGetCrystal())
		{
			simon->SetDidGetCrystal(false);
			return 8;
		}
		break;
	}
	return currentStage;
}