#include "Stats.h"

Stats *Stats::__instance = NULL;

Stats *Stats::GetInstance(int ID_TEX_STATS, LPCWSTR STATS_TEXTURE_PATH, D3DCOLOR STATS_TEXTURE_BACKGROUND_COLOR)
{
	__instance = new Stats(ID_TEX_STATS, STATS_TEXTURE_PATH, STATS_TEXTURE_BACKGROUND_COLOR);
	return __instance;
}

Stats *Stats::GetInstance()
{
	return __instance;
}

Stats::Stats(int ID_TEX_STATS, LPCWSTR STATS_TEXTURE_PATH, D3DCOLOR STATS_TEXTURE_BACKGROUND_COLOR)
{
	simon_health = 16;
	boss_health = 16; 
	score = 0;
	heart = 5;
	timer = 300;
	tickTime = GetTickCount() + 1000;

	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_STATS, STATS_TEXTURE_PATH, STATS_TEXTURE_BACKGROUND_COLOR);

	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texStats = textures->Get(ID_TEX_STATS);

	// Background
	sprites->Add(130001, 0, 0, 256, 50, texStats);
	
	// Number
	sprites->Add(130010, 8, 50, 14, 58, texStats);
	sprites->Add(130011, 17, 50, 23, 58, texStats);
	sprites->Add(130012, 26, 50, 32, 58, texStats);
	sprites->Add(130013, 35, 50, 41, 58, texStats);
	sprites->Add(130014, 44, 50, 50, 58, texStats);
	sprites->Add(130015, 53, 50, 59, 58, texStats);
	sprites->Add(130016, 62, 50, 68, 58, texStats);
	sprites->Add(130017, 71, 50, 77, 58, texStats);
	sprites->Add(130018, 80, 50, 86, 58, texStats);
	sprites->Add(130019, 89, 50, 95, 58, texStats);

	// Health Bar
		// Simon
	sprites->Add(1300021, 98, 50, 104, 60, texStats);

		// Enemy
	sprites->Add(1300022, 110, 50, 116, 60, texStats);
		
		// Lose
	sprites->Add(1300023, 104, 50, 110, 60, texStats);
}

void Stats::Update(int current_Stage)
{
	if (GetTickCount() >= tickTime)
	{
		timer--;
		tickTime = GetTickCount() + 1000;
	}
	timer = max(0, timer);
	CSimon *simon = CSimon::GetInstance();
	simon_health = simon->GetHealth();

	if (current_Stage == 2)
	{
		PhantomBat *phantomBat = PhantomBat::GetInstance();
		boss_health = phantomBat->GetHealth();
	}
	else if (current_Stage == 7)
	{
		Medusa *medusa = Medusa::GetInstance();
		boss_health = medusa->GetHealth();
	}
	else
		boss_health = 16;

	rangedType = simon->GetRangedType();

	maximumThrows = simon->GetMaximumThrows();

	stage = current_Stage;
}

void Stats::Render()
{
	CSprites * sprites = CSprites::GetInstance();

	sprites->Get(130001)->Draw(0, 0);

	string toString;

	// Render Score
	toString = std::to_string(score);
	while (toString.length() < 6) 
		toString = '0' + toString;
	for (int i = 0; i < 6; i++)
		sprites->Get(130010 + toString[i] - 48)->Draw(59 + i * 5 * 1.0f, 6);

	// Render Time
	toString = std::to_string(timer);
	while (toString.length()<4)
		toString = '0' + toString;
	for (int i = 0; i < 4; i++)
		sprites->Get(130010 + toString[i] - 48)->Draw(142 + i * 5 * 1.0f, 6);

	// Render Stage
	toString = std::to_string(stage);
	toString = '0' + toString;
	for (int i = 0; i < 2; i++)
		sprites->Get(130010 + toString[i] - 48)->Draw(225 + i * 5 * 1.0f, 6);

	// Render Heart
	toString = std::to_string(heart);
	while (toString.length() < 2)
		toString = '0' + toString;
	for (int i = 0; i < 2; i++)
		sprites->Get(130010 + toString[i] - 48)->Draw(221 + i * 5 * 1.0f, 31);

	// Render Ranged Weapon
	switch(rangedType)
	{
	case ID_RANGED_WEAPON_NONE:
		break;
	case ID_RANGED_WEAPON_DAGGER:
		sprites->Get(50011)->Draw(165, 30);
		break;
	case ID_RANGED_WEAPON_HOLY_WATER_IN_POTION:
		sprites->Get(50061)->Draw(165, 26);
		break;
	case ID_RANGED_WEAPON_HOLY_WATER_ON_FIRE:
		sprites->Get(50061)->Draw(165, 26);
		break;
	case ID_RANGED_WEAPON_AXE:
		sprites->Get(50101)->Draw(165, 26);
		break;
	case ID_RANGED_WEAPON_STOPWATCH:
		sprites->Get(50081)->Draw(165, 26);
		break;
	case ID_RANGED_WEAPON_BOOMERANG:
		sprites->Get(50131)->Draw(165, 26);
		break;
	}

	// Render Health
		// Simon
	for (int i = 0; i < simon_health; i++)
		sprites->Get(1300021)->Draw(64 + i * 5 * 1.0f, 20);
	for (int i = max(0, simon_health); i < 16; i++)
		sprites->Get(1300023)->Draw(64 + i * 5 * 1.0f, 20);

		// Enemy
	for (int i = 0; i < boss_health; i++)
		sprites->Get(1300022)->Draw(64 + i * 5 * 1.0f, 35);
	for (int i = max(0, boss_health); i < 16; i++)
		sprites->Get(1300023)->Draw(64 + i * 5 * 1.0f, 35);

	// Render triple throw item if simon has collected it
	if (maximumThrows == 2)
		sprites->Get(50141)->Draw(237, 29);
}

Stats::~Stats()
{
}