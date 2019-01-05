#include "Enemy.h"

bool Enemy::isFreezingAll = false;
DWORD Enemy::freezingTimeAll = GetTickCount();

void Enemy::LoadEnemyTextures(int ID_TEX_ENEMIES, LPCWSTR ENEMIES_TEXTURE_PATH, D3DCOLOR ENEMIES_TEXTURE_BACKGROUND_COLOR)
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_ENEMIES, ENEMIES_TEXTURE_PATH, ENEMIES_TEXTURE_BACKGROUND_COLOR);

	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texEnemies = textures->Get(ID_TEX_ENEMIES);

	LPANIMATION ani;
	ifstream in;
	in.open(ENEMY_SPRITES_PATH);
	int n, nState, id, left, top, right, bottom;
	int flag = 0;
	int count = -1;
	in >> nState;
	for (int i = 0; i < nState; i++)
	{
		in >> n;
		ani = new CAnimation(100);
		for (int j = 0; j < n; j++)
		{
			in >> id >> left >> top >> right >> bottom;
			sprites->Add(id, left, top, right, bottom, texEnemies);
			ani->Add(id);
		}
		int temp = (id / 100 % 10);
		if (flag == temp)
		{
			count++;
			flag = temp;
		}
		else
		{
			count = 0;
			flag = temp;
		}
		animations->Add(ID_TEX_ENEMIES + temp * 10 + count, ani);


	}
}


void Enemy::LoadBossTextures(int ID_TEX_BOSSES, LPCWSTR BOSSES_TEXTURE_PATH, D3DCOLOR BOSSES_TEXTURE_BACKGROUND_COLOR)
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_BOSSES, BOSSES_TEXTURE_PATH, BOSSES_TEXTURE_BACKGROUND_COLOR);

	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texBosses = textures->Get(ID_TEX_BOSSES);
	LPANIMATION ani;
	ifstream in;
	in.open(BOSS_SPRITES_PATH);
	int n, nState, id, left, top, right, bottom;
	int temp = 793;
	in >> nState;
	for (int i = 0; i < nState; i++)
	{
		in >> n;
		ani = new CAnimation(100);
		for (int j = 0; j < n; j++)
		{
			in >> id >> left >> top >> right >> bottom;
			sprites->Add(id, left, top, right, bottom, texBosses);
			ani->Add(id);
		}

		if (id / 10000 == 8)
			animations->Add(ID_TEX_BOSSES + 1 + i, ani);
		else
		{
			animations->Add(temp, ani);
			temp++;
		}
	}


}

Enemy::Enemy()
{
	isFreezing = isFreezingAll;
	freezingTime = freezingTimeAll;
}

void Enemy::ReduceHealth(int value, DWORD startAttackingTime)
{
	if (this->startAttackingTime != startAttackingTime)
	{
		health -= value;
		this->startAttackingTime = startAttackingTime;
	}
}

void Enemy::ReduceHealth(DWORD startThrowingTime)
{
	if (this->startThrowingTime != startThrowingTime)
	{
		health -= 2;
		this->startThrowingTime = startThrowingTime;
	}
}

void Enemy::StartFreezing(int time)
{
	isFreezing = true;
	freezingTime = GetTickCount() + time;
}

void Enemy::StartFreezingAll(int time)
{
	isFreezingAll = true;
	freezingTimeAll = GetTickCount() + time;
}

Enemy::~Enemy()
{
}
