#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <fstream>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Simon.h"
#include "Background.h"
#include "Candle.h"
#include "MeleeWeapon.h"
#include "Item.h"
#include "RangedWeapon.h"
#include "Brick.h"
#include "Camera.h"
#include "Stairs.h"
#include "Zombie.h"
#include "Bat.h"
#include "Panther.h"
#include "Door.h"
#include "Merman.h"
#include "PhantomBat.h"
#include "Soldier.h"
#include "Bat2.h"
#include "MovingBlock.h"
#include "Head.h"
#include "SnakeHead.h"
#include "Medusa.h"
#include "Thorn.h"
#include "BreakableBrick.h"
#include "Effect.h"
#include "Skull.h"
#include "Stats.h"
#include "WinScene.h"
#include "Sound.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"Castlevania"

#define SCREEN_WIDTH	270
#define SCREEN_HEIGHT	270 //260
#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)

#define OBJECTS_STAGE_0_INFO_PATH		L"Objects\\Stage 0 Objects Info.txt"
#define OBJECTS_STAGE_1_INFO_PATH		L"Objects\\Stage 1 Objects Info.txt"
#define OBJECTS_STAGE_2_INFO_PATH		L"Objects\\Stage 2 Objects Info.txt"
#define OBJECTS_STAGE_3_INFO_PATH		L"Objects\\Stage 3 Objects Info.txt"
#define OBJECTS_STAGE_4_INFO_PATH		L"Objects\\Stage 4 Objects Info.txt"
#define OBJECTS_STAGE_5_INFO_PATH		L"Objects\\Stage 5 Objects Info.txt"
#define OBJECTS_STAGE_6_INFO_PATH		L"Objects\\Stage 6 Objects Info.txt"
#define OBJECTS_STAGE_7_INFO_PATH		L"Objects\\Stage 7 Objects Info.txt"

#define SIMON_TEXTURE_PATH				L"Simon Belmont\\Simon.png"
#define CANDLES_TEXTURE_PATH			L"Objects\\Candles.png"
#define MELEE_WEAPONS_TEXTURE_PATH		L"Simon Belmont\\Melee Weapons.png"
#define ITEMS_TEXTURE_PATH				L"Objects\\Items.png"
#define RANGED_WEAPONS_TEXTURE_PATH		ITEMS_TEXTURE_PATH
#define ENEMIES_TEXTURE_PATH			L"Enemies\\Enemies.png"
#define DOORS_TEXTURE_PATH				L"Objects\\Doors.png"
#define BOSSES_TEXTURE_PATH				L"Bosses\\Bosses.png"
#define MOVINGBLOCK_TEXTURE_PATH		L"Objects\\MovingBlock.png"
#define THORN_TEXTURE_PATH				L"Objects\\Thorn.png"
#define BREAKABLE_BRICKS_TEXTURE_PATH	L"Objects\\Breakable Bricks.png"
#define EFFECTS_TEXTURE_PATH			L"Objects\\Effects.png"
#define STATS_TEXTURE_PATH				L"Stats\\Stats.png"
#define WIN_SCENE_TEXTURE_PATH			L"Backgrounds\\Win Scene.png"

#define MELEE_WEAPONS_TEXTURE_BACKGROUND_COLOR		D3DCOLOR_XRGB(0, 128, 128)
#define SIMON_TEXTURE_BACKGROUND_COLOR				D3DCOLOR_XRGB(0, 128, 128)
#define CANDLES_TEXTURE_BACKGROUND_COLOR			D3DCOLOR_XRGB(34, 177, 76)
#define ITEMS_TEXTURE_BACKGROUND_COLOR				D3DCOLOR_XRGB(128, 0, 0)
#define RANGED_WEAPONS_TEXTURE_BACKGROUND_COLOR		ITEMS_TEXTURE_BACKGROUND_COLOR
#define ENEMIES_TEXTURE_BACKGROUND_COLOR			D3DCOLOR_XRGB(96, 68, 106)
#define DOORS_TEXTURE_BACKGROUND_COLOR				D3DCOLOR_XRGB(34, 177, 76)
#define BOSSES_TEXTURE_BACKGROUND_COLOR				D3DCOLOR_XRGB(168, 40, 88)
#define MOVINGBLOCK_TEXTURE_BACKGROUND_COLOR		NULL
#define THORN_TEXTURE_BACKGROUND_COLOR				D3DCOLOR_XRGB(34, 177, 76)
#define BREAKABLE_BRICKS_TEXTURE_BACKGROUND_COLOR	D3DCOLOR_XRGB(34, 177, 76)
#define EFFECTS_TEXTURE_BACKGROUND_COLOR			D3DCOLOR_XRGB(96, 68, 106)
#define STATS_TEXTURE_BACKGROUND_COLOR				NULL
#define WIN_SCENE_TEXTURE_BACKGROUND_COLOR			NULL

#define MAX_FRAME_RATE 100

#define ID_TEX_SIMON			100
#define ID_TEX_BACKGROUND		200
#define ID_TEX_CANDLES			300
#define ID_TEX_MELEE_WEAPONS	400
#define ID_TEX_ITEMS			500
#define ID_TEX_RANGED_WEAPONS	600
#define ID_TEX_ENEMIES			700
#define ID_TEX_DOORS			800
#define ID_TEX_BOSSES			900
#define ID_TEX_MOVINGBLOCK		1000
#define ID_TEX_THORN			1100
#define ID_TEX_BREAKABLE_BRICKS	1200
#define ID_TEX_EFFECTS			1300
#define ID_TEX_STATS			1400
#define ID_TEX_WIN_SCENE		1500

#define ID_TITLE_SCENE		0
#define ID_INTRO_SCENE		1
#define ID_NORMAL_SCENE		2
#define ID_CHANGING_SCENE	3
#define ID_DEATH_SCENE		4
#define ID_WIN_SCENE		5

#define WHITE_COLOR D3DCOLOR_XRGB(255, 255, 255) 

#define MUSIC_BR0			"Sounds\\music\\Game_Start_Prologue.wav"
#define MUSIC_BR1			"Sounds\\music\\Stage_01_Vampire_Killer.wav"
#define MUSIC_BR4			"Sounds\\music\\Stage_04_Stalker.wav"
#define MUSIC_BOSS			"Sounds\\music\\Boss_Battle_Poison_Mind.wav"
#define MUSIC_WIN			"Sounds\\music\\Stage_Clear.wav"
#define SOUND_DOOR			"Sounds\\sound\\opendoor.wav"
#define SOUND_ATTACK		"Sounds\\sound\\usingwhip.wav"
#define SOUND_HIT			"Sounds\\sound\\hit.wav"
#define SOUND_WATER			"Sounds\\sound\\splashwater.wav"

CGame *game;
CSimon *simon;
Camera *camera;
vector<LPBACKGROUND> backgrounds;
MeleeWeapon *meleeWeapon;
vector<vector<LPGAMEOBJECT>> objects;
Door *usingDoor = NULL;
int current_Stage, previous_Stage;
float background_Width;
bool hasBossFight;
bool inBossFight;
float bossFight_x;
LPGAMEOBJECT boss;
PhantomBat *phantomBat;
Medusa *medusa;
bool flag_flicker, flag_death, flag_intro, flag_title = false, flag_win = false, flag_stoped= false;
Stats *stats;
int phase;
DWORD phaseTime;
int done = 0;
int sceneID = 0;
WinScene *winScene;
bool soundIsSet;
Mix_Chunk* music_BR0 = NULL;
Mix_Chunk* music_BR1 = NULL;
Mix_Chunk* music_BR4 = NULL;
Mix_Chunk* music_BOSS = NULL;
Mix_Chunk* music_WIN = NULL;
Sound *sound = new Sound();
Sound *attackingSound = new Sound();
Sound *sounddoor = new Sound();
Sound *waterSound = new Sound();

class CSampleKeyHander : public CKeyEventHandler
{
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander * keyHandler;

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	if (KeyCode == DIK_ESCAPE)
		done = 1;

	if (sceneID == ID_TITLE_SCENE)
	{
		if (KeyCode == DIK_RETURN)
			sceneID = ID_INTRO_SCENE;
		return;
	}

	if (sceneID == ID_WIN_SCENE)
	{
		if (KeyCode == DIK_RETURN)
			flag_win = true;
		return;
	}

	if (simon->GetIsDead())
	{
		if (KeyCode == DIK_RETURN)
			flag_death = true;
		return;
	}

	if (simon->GetIsAttacking()) return;

	if (simon->GetIsOnStairs())
	{
		if (KeyCode == DIK_A && game->IsKeyDown(DIK_UP) && simon->CanThrow())
			if (simon->IsUpstairs())
				simon->SetState(SIMON_STATE_THROWING_UPSTAIRS_RIGHT);
			else simon->SetState(SIMON_STATE_THROWING_DOWNSTAIRS_RIGHT);
		else if (KeyCode == DIK_A)
		{
			if (simon->IsUpstairs())
				simon->SetState(SIMON_STATE_ATTACKING_UPSTAIRS_RIGHT);
			else simon->SetState(SIMON_STATE_ATTACKING_DOWNSTAIRS_RIGHT);
			attackingSound->playSound();
		}
		return;
	}

	// Handling Jumping when Moving
	if (KeyCode == DIK_S)
		if (game->IsKeyDown(DIK_RIGHT) || game->IsKeyDown(DIK_LEFT)) simon->SetIsJumpingForward(true);

	// Handling Crouching Throw
	if (KeyCode == DIK_A && game->IsKeyDown(DIK_UP) && simon->CanThrow())
		if (game->IsKeyDown(DIK_DOWN))
		{
			simon->SetState(SIMON_STATE_CROUCHING_THROW_RIGHT);
			return;
		}

	// Handling Crouching Attack
	if (KeyCode == DIK_A)
		if (game->IsKeyDown(DIK_DOWN))
		{
			simon->SetState(SIMON_STATE_CROUCHING_ATTACK_RIGHT);
			attackingSound->playSound();
			return;
		}

	switch (KeyCode)
	{
	case DIK_S:
		simon->SetState(SIMON_STATE_JUMPING_RIGHT);
		break;
	case DIK_A:
		if (game->IsKeyDown(DIK_UP))
		{
			if (simon->CanThrow())
				simon->SetState(SIMON_STATE_THROW_RIGHT);
		}
		else 
		{
			simon->SetState(SIMON_STATE_ATTACK_RIGHT);
			attackingSound->playSound();
		}
		break;
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CSampleKeyHander::KeyState(BYTE *states)
{
	// if simon is dead, deny all key control
	if (simon->GetIsDead()) return;

	// if simon is attacking or falling, deny all key control
	if (simon->GetIsAttacking() || simon->GetIsFalling()) return;

	if (simon->GetIsOnStairs())
	{
		if (game->IsKeyDown(DIK_UP))
		{
			if (!simon->CheckOutStairs(true))
				if ((!simon->IsUpstairs() && simon->GetState() % 2 == 0) || (simon->IsUpstairs() && simon->GetState() % 2 == 1))
					simon->SetState(SIMON_STATE_WALKING_UPSTAIRS_LEFT);
				else simon->SetState(SIMON_STATE_WALKING_UPSTAIRS_RIGHT);
		}
		else if (game->IsKeyDown(DIK_DOWN))
		{
			if (!simon->CheckOutStairs(false))
				if ((simon->IsUpstairs() && simon->GetState() % 2 == 0) || (!simon->IsUpstairs() && simon->GetState() % 2 == 1))
					simon->SetState(SIMON_STATE_WALKING_DOWNSTAIRS_LEFT);
				else simon->SetState(SIMON_STATE_WALKING_DOWNSTAIRS_RIGHT);
		}
		else if (simon->ny == -1)
		{
			if (simon->nx == 1)
				simon->SetState(SIMON_STATE_IDLE_UPSTAIRS_RIGHT);
			else simon->SetState(SIMON_STATE_IDLE_UPSTAIRS_LEFT);
		}
		else
		{
			if (simon->nx == 1)
				simon->SetState(SIMON_STATE_IDLE_DOWNSTAIRS_RIGHT);
			else simon->SetState(SIMON_STATE_IDLE_DOWNSTAIRS_LEFT);
		}
		return;
	}

	int res;
	if (game->IsKeyDown(DIK_UP))
	{
		res = simon->CheckOnStairs(&objects, true);
		if (res != -1)
		{
			if (res == 0) simon->SetState(SIMON_STATE_WALKING_UPSTAIRS_RIGHT);
			else simon->SetState(SIMON_STATE_WALKING_UPSTAIRS_LEFT);
			return;
		}
	}

	if (game->IsKeyDown(DIK_DOWN))
	{
		res = simon->CheckOnStairs(&objects, false);
		if (res != -1)
		{
			if (res == 0) simon->SetState(SIMON_STATE_WALKING_DOWNSTAIRS_LEFT);
			else simon->SetState(SIMON_STATE_WALKING_DOWNSTAIRS_RIGHT);
			return;
		}
	}

	if (game->IsKeyDown(DIK_RIGHT))
		simon->SetState(SIMON_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		simon->SetState(SIMON_STATE_WALKING_LEFT);
	else if (game->IsKeyDown(DIK_DOWN))
		simon->SetState(SIMON_STATE_CROUCH_RIGHT);
	else simon->SetState(SIMON_STATE_IDLE_RIGHT);
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int screen_width, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			screen_width,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

void LoadMap()
{
	/*	Intro Scene at background 0
		Title Scene at background 8
		Death Scene at background 9 */
	previous_Stage = 0;
	current_Stage = 0;
	Background *background;
	for (int i = 0; i < 10; i++)
	{
		background = new Background(ID_TEX_BACKGROUND, BACKGROUND_COLOR, i);
		backgrounds.push_back(background);
	}
}

void LoadStageInfo()
{
	soundIsSet = false;
	objects.clear();
	for (int i = 0; i < 20; i++)
	{
		vector<LPGAMEOBJECT> object;
		object.clear();
		objects.push_back(object);
	}

	// Read Info from files
	ifstream in;
	int n;
	float x, y;
	int itemType;

	simon->ResetState();
	inBossFight = false;
	simon->SetNumberOfThrows(0);

	switch (current_Stage)
	{
	case 0:
		in.open(OBJECTS_STAGE_0_INFO_PATH);
		stats->SetTimer(300);
		background_Width = 270;
		hasBossFight = false;
		simon->SetPosition(250.0f, 117.0f);
		break;
	case 1:
		in.open(OBJECTS_STAGE_1_INFO_PATH);
		stats->SetTimer(300);
		background_Width = 768;
		hasBossFight = false;
		simon->SetPosition(0.0f, 112.0f);
		break;
	case 2:
		in.open(OBJECTS_STAGE_2_INFO_PATH);
		background_Width = 2816;
		hasBossFight = true;
		bossFight_x = background_Width;
		phantomBat = PhantomBat::GetInstance(2656, 16);
		boss = phantomBat;

		if (previous_Stage == 1)
			simon->SetPosition(0.0f, 112.0f);
		else
		{
			if (simon->GetX() < 200)
				simon->SetPosition(1568.0f, 125.0f);
			else
				simon->SetPosition(1888.0f, 125.0f);
		}
		break;
	case 3:		
		in.open(OBJECTS_STAGE_3_INFO_PATH);
		background_Width = 512;
		hasBossFight = false;

		if (simon->GetX() < 1800)
			simon->SetPosition(100.0f, 15.0f);
		else
			simon->SetPosition(450.0f, 45.0f);
		break;
	case 4:
		in.open(OBJECTS_STAGE_4_INFO_PATH);
		stats->SetTimer(400);
		background_Width = 256;
		hasBossFight = false;
		
		if (previous_Stage == 5)
			simon->SetPosition(74.0f, 30.0f);
		else
		simon->SetPosition(50.0f, 122.0f);
		
		break;
	case 5:
		in.open(OBJECTS_STAGE_5_INFO_PATH);
		background_Width = 1280;
		hasBossFight = false;

		if (previous_Stage == 4)
			simon->SetPosition(1220.0f, 64.0f);
		else
		simon->SetPosition(47.0f, 1.0f);

		break;
	case 6:
		in.open(OBJECTS_STAGE_6_INFO_PATH);
		background_Width = 1536;
		hasBossFight = false;

		if (previous_Stage == 5)
			simon->SetPosition(869.0f, 110.0f);
		else
			simon->SetPosition(653.0f, 2.0f);

		break;
	case 7:
		in.open(OBJECTS_STAGE_7_INFO_PATH);
		background_Width = 767;
		hasBossFight = true;
		bossFight_x = 0;
		medusa = Medusa::GetInstance(101, 88);
		boss = medusa;

		if (previous_Stage == 6)
			simon->SetPosition(719.0f, 96.0f);
		else
			simon->SetPosition(719.0f, 96.0f);

		break;
	}


	// 1: Read Candles type 0
	in >> n;
	for (int i = 0; i < n; i++)
	{
		in >> x >> y >> itemType;
		Candle *candle = new Candle(x, y, CANDLE_TYPE_BIG, itemType);
		objects[int(x / 256)].push_back(candle);
	}

	// 2: Read Candles type 1 (tbc..)
	in >> n;
	for (int i = 0; i < n; i++)
	{
		in >> x >> y >> itemType;
		Candle *candle = new Candle(x, y, CANDLE_TYPE_SMALL, itemType);
		objects[int(x / 256)].push_back(candle);
	}

	// 3: Read Bricks
	Brick *brick;
	float left, top, right, bottom;
	in >> n;
	for (int i = 0; i < n; i++)
	{
		in >> left >> top >> right >> bottom;
		brick = new Brick(left, top, right, bottom);
		if (right - left > 256)
		{
			for (int j = int(left / 256); j < int(right / 256); j++)
				objects[j].push_back(brick);
		}
		else objects[int(left / 256)].push_back(brick);
	}

	// 4: Read Stairs
	int state;
	Stairs *stairs;
	float leftt, topp, rightt, bottomm;
	in >> n;
	for (int i = 0; i < n; i++)
	{
		in >> state >> left >> top >> right >> bottom >> leftt >> topp >> rightt >> bottomm;
		stairs = new Stairs(state, left, top, right, bottom, leftt, topp, rightt, bottomm);;
		objects[int(left / 256)].push_back(stairs);
		if (int(left / 256) != int(leftt / 256))
			objects[int(leftt / 256)].push_back(stairs);
	}

	// 5: Read Panthers
	in >> n;
	for (int i = 0; i < n; i++)
	{
		in >> x >> y;
		Panther *panther = new Panther(x, y);
		objects[int(x / 256)].push_back(panther);
	}

	// 6: Read Doors
	int nx;
	in >> n;
	for (int i = 0; i < n; i++)
	{
		in >> x >> y >> nx;
		Door *door = new Door(x, y, nx);
		objects[int(x / 256)].push_back(door);
	}

	// 7: Read Soldier
	float xmin, xmax;
	in >> n;
	for (int i = 0; i < n; i++)
	{
		in >> x >> y >> xmin >> xmax;
		Soldier *soldier = new Soldier(x, y, xmin, xmax);
		objects[int(x / 256)].push_back(soldier);
	}
	
	// 8: Read Bat2
	in >> n;
	for (int i = 0; i < n; i++)
	{
		in >> x >> y;
		Bat2 *bat2 = new Bat2(x, y);
		objects[int(x / 256)].push_back(bat2);
	}

	// 9: Read MovingBlock
	in >> n;
	for (int i = 0; i < n; i++)
	{
		in >> xmin>> y >> xmax;
		MovingBlock *movingBlock = new MovingBlock(y, xmin, xmax);
		objects[int(xmin / 256)].push_back(movingBlock);
	}

	// 10: Read SnakeHead
	in >> n;
	for (int i = 0; i < n; i++)
	{
		in >> x >> y;
		SnakeHead *snakeHead = new SnakeHead(x, y);
		objects[int(x / 256)].push_back(snakeHead);
	}
	// 11: Read Thorn
	int z;
	in >> n;
	for (int i = 0; i < n; i++)
	{
		in >> x >> y >> z;
		Thorn *thorn = new Thorn(x, y, z);
		objects[int(x / 256)].push_back(thorn);
	}

	// 12: Read Breakable Block
	int type;
	in >> n;
	for (int i = 0; i < n; i++)
	{
		in >> x >> y >> type >> itemType;
		BreakableBrick *breakableBrick = new BreakableBrick(x, y, type, itemType);
		objects[int(x / 256)].push_back(breakableBrick);
	}

	// 13: Read Skull
	in >> n;
	for (int i = 0; i < n; i++)
	{
		in >> x >> y;
		Skull *skull = new Skull(x, y);
		objects[int(x / 256)].push_back(skull);
	}

	in.close();
	camera->SetUpCamera(SCREEN_WIDTH, SCREEN_HEIGHT, background_Width, &objects);
}
/*
	Load all game resources.
*/
void LoadResources()
{
	simon = CSimon::GetInstance(ID_TEX_SIMON, SIMON_TEXTURE_PATH, SIMON_TEXTURE_BACKGROUND_COLOR);

	RangedWeapon::LoadRangedWeaponTexture(ID_TEX_RANGED_WEAPONS, RANGED_WEAPONS_TEXTURE_PATH, RANGED_WEAPONS_TEXTURE_BACKGROUND_COLOR);
	meleeWeapon = MeleeWeapon::GetInstance(ID_TEX_MELEE_WEAPONS, MELEE_WEAPONS_TEXTURE_PATH, MELEE_WEAPONS_TEXTURE_BACKGROUND_COLOR);
	stats = Stats::GetInstance(ID_TEX_STATS, STATS_TEXTURE_PATH, STATS_TEXTURE_BACKGROUND_COLOR);
	winScene = WinScene::GetInstance(ID_TEX_WIN_SCENE, WIN_SCENE_TEXTURE_PATH, WIN_SCENE_TEXTURE_BACKGROUND_COLOR);

	camera = Camera::GetInstance();

	Candle::LoadCandleTextures(ID_TEX_CANDLES, CANDLES_TEXTURE_PATH, CANDLES_TEXTURE_BACKGROUND_COLOR);
	Item::LoadItemTextures(ID_TEX_ITEMS, ITEMS_TEXTURE_PATH, ITEMS_TEXTURE_BACKGROUND_COLOR);
	Door::LoadDoorTextures(ID_TEX_DOORS, DOORS_TEXTURE_PATH, DOORS_TEXTURE_BACKGROUND_COLOR);
	Enemy::LoadEnemyTextures(ID_TEX_ENEMIES, ENEMIES_TEXTURE_PATH, ENEMIES_TEXTURE_BACKGROUND_COLOR);
	Enemy::LoadBossTextures(ID_TEX_BOSSES, BOSSES_TEXTURE_PATH, BOSSES_TEXTURE_BACKGROUND_COLOR);
	MovingBlock::LoadMovingBlockTextures(ID_TEX_MOVINGBLOCK, MOVINGBLOCK_TEXTURE_PATH, MOVINGBLOCK_TEXTURE_BACKGROUND_COLOR);
	Thorn::LoadThornTextures(ID_TEX_THORN, THORN_TEXTURE_PATH, THORN_TEXTURE_BACKGROUND_COLOR);
	BreakableBrick::LoadBreakableBrickTextures(ID_TEX_BREAKABLE_BRICKS, BREAKABLE_BRICKS_TEXTURE_PATH, BREAKABLE_BRICKS_TEXTURE_BACKGROUND_COLOR);
	Effect::LoadEffectTextures(ID_TEX_EFFECTS, EFFECTS_TEXTURE_PATH, EFFECTS_TEXTURE_BACKGROUND_COLOR);
	LoadStageInfo();

	phase = 0;
	srand(time(NULL));

	sound->SetSound(-1, NULL, -1);
	attackingSound->SetSound(-1, NULL, 0);	
	Mix_Chunk * mix = Mix_LoadWAV(SOUND_ATTACK);
	attackingSound->SetSound(1, mix, 0);

	music_BR0 = Mix_LoadWAV(MUSIC_BR0);
	music_BR1 = Mix_LoadWAV(MUSIC_BR1);
	music_BR4 = Mix_LoadWAV(MUSIC_BR4);
	music_BOSS = Mix_LoadWAV(MUSIC_BOSS);
	music_WIN = Mix_LoadWAV(MUSIC_WIN);

	mix = NULL;
	sounddoor->SetSound(100, mix, -1);
	mix = Mix_LoadWAV(SOUND_DOOR);
	sounddoor->SetSound(100, mix, 0);

	mix = NULL;
	waterSound->SetSound(101, mix, -1);
	mix = Mix_LoadWAV(SOUND_WATER);
	waterSound->SetSound(101, mix, 0);
}

void ClearObjects()
{
	/*vector<LPGAMEOBJECT> deleteLater;
	deleteLater.clear();
	float left, top, right, bottom;
	UINT k;

	for (UINT i = 0; i < objects.size(); i++)
		for (UINT j = 0; j < objects[i].size(); j++)
		{
			objects[i][j]->GetBoundingBox(left, top, right, bottom);
			if (right - left > 256)
			{
				k = 0;
				for (k = 0; k < deleteLater.size(); k++)
					if (objects[i][j] == deleteLater[k])
						break;
				if (k >= deleteLater.size())
					deleteLater.push_back(objects[i][j]);
			}
			else delete objects[i][j];
		}
	for (UINT i = 0; i < deleteLater.size(); i++)
		delete deleteLater[i];*/
	objects.clear();
}

void ClearResources()
{
	delete meleeWeapon, stats, camera;
	ClearObjects();
}

void AddBoss()
{
	// Delete all creeps before boss fight
	UINT j;
	for (UINT i = 0; i < objects.size(); i++)
	{
		j = 0;
		while (j < objects[i].size())
		{
			if (dynamic_cast<LPENEMY>(objects[i][j]))
			{
				delete objects[i][j];
				objects[i].erase(objects[i].begin() + j);
			}
			else j++;
		}
	}

	if (current_Stage == 2)
	{
		objects[int(phantomBat->GetX() / 256)].push_back(phantomBat);
		inBossFight = true;
		phantomBat->SetState(PHANTOMBAT_STATE_ACTIVE);
	}

	if (current_Stage == 7)
	{
		objects[int(medusa->GetX() / 256)].push_back(medusa);
		inBossFight = true;
		medusa->SetState(MEDUSA_STATE_ACTIVE);
	}
	
	sound->StopSound();
	sound->SetSound(-1, music_BOSS, -1);
	sound->playSound();
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	// If Simon meets the conditions, the stage will change
	int checkStage = Background::ChangeStage(current_Stage);
	if (checkStage!=current_Stage)
	{
		if (checkStage == 8 && current_Stage == 7)
		{
			sceneID = ID_WIN_SCENE;
			winScene->SetScore(stats->GetScore());
			return;
		}

		previous_Stage = current_Stage;
		current_Stage = checkStage;
		soundIsSet = false;

		ClearObjects();

		LoadStageInfo();
	}

	// reset all door state and delete all objects that doesn't exist
	UINT j;
	for (UINT i = 0; i < objects.size(); i++)
	{
		j = 0;
		while (j < objects[i].size())
		{
			if (dynamic_cast<Door *>(objects[i][j]))
			{
				Door *door = dynamic_cast<Door *>(objects[i][j]);
				door->SetIsUsed(false);
			}

			// Delete enemy that is out of screen
			if (dynamic_cast<LPENEMY>(objects[i][j]))
			{
				LPENEMY enemy = dynamic_cast<LPENEMY>(objects[i][j]);
				if (!enemy->DoesExist())
				{
					if (enemy->GetHealth() <= 0)
					{
						Effect *effect = new Effect(objects[i][j]->GetX(), objects[i][j]->GetY(), EFFECT_TYPE_DESTROYED, 4);
						objects[int(objects[i][j]->GetX() / 256)].push_back(effect);
						stats->IncreaseScore(100);
					}
					if (dynamic_cast<PhantomBat *>(objects[i][j]) || dynamic_cast<Medusa *>(objects[i][j]))
					{
						Item *item = new Item(objects[i][j]->GetX(), objects[i][j]->GetY(), ITEM_TYPE_CRYSTAL);
						objects[int(objects[i][j]->GetX() / 256)].push_back(item);
					}

					delete objects[i][j];
					objects[i].erase(objects[i].begin() + j);
					continue;
				}
			}
			// Delete effect that is out of screen
			if (dynamic_cast<Effect *>(objects[i][j]))
			{
				Effect *effect = dynamic_cast<Effect *>(objects[i][j]);
				if (!effect->DoesExist())
				{
					delete objects[i][j];
					objects[i].erase(objects[i].begin() + j);
					continue;
				}
			}

			// Delete ranged weapons that don't exist
			if (dynamic_cast<RangedWeapon *>(objects[i][j]))
			{
				RangedWeapon *rangedWeapon = dynamic_cast<RangedWeapon *>(objects[i][j]);
				if (!rangedWeapon->GetDoesExist())
				{
					delete objects[i][j];
					objects[i].erase(objects[i].begin() + j);
					simon->DecreaseNumberOfThrows();
					continue;
				}
			}

			j++;
		}
	}
	
	// Update object's position on grid based on its position in game
	int X;
	for (UINT i = 0; i < objects.size(); i++)
	{
		j = 0;
		while (j < objects[i].size())
		{
			X = int(objects[i][j]->GetX() / 256);
			if (X!= i && !dynamic_cast<Brick *>(objects[i][j]) && !dynamic_cast<Stairs *>(objects[i][j]))
			{
				if (X < 0)
					objects[0].push_back(objects[i][j]);
				else
					objects[X].push_back(objects[i][j]);

				objects[i].erase(objects[i].begin() + j);
			}
			else j++;
		}
	}

	// Spawn enemies
	int spawnPos;
	// Only spawn when there is no boss fight at moment
	if ((hasBossFight && !inBossFight) || !hasBossFight)
		switch (current_Stage)
		{
		case 1:
			// no enemies in stage 1
			break;
		case 2:
			// if spawnPos == 1 spawn from the left, spawnPos == 2 spawn from the right
			spawnPos = Zombie::SpawnPos();
			if (spawnPos > 0)
			{
				Zombie *zombie;
				if (spawnPos == 1)
					zombie = new Zombie(camera->GetX() - camera->GetScreenWidth() / 2, 126, 1);
				else
					zombie = new Zombie(camera->GetX() + camera->GetScreenWidth() / 2, 126, -1);

				objects[int(zombie->GetX() / 256)].push_back(zombie);
			}

			spawnPos = Bat::SpawnPos();
			if (spawnPos > 0)
			{
				Bat *bat;
				if (spawnPos == 1)
					bat = new Bat(camera->GetX() - camera->GetScreenWidth() / 2, 0, 120, 1);
				else
					bat = new Bat(camera->GetX() + camera->GetScreenWidth() / 2, 0, 120, -1);

				objects[int(bat->GetX() / 256)].push_back(bat);
			}
			break;
		case 3:
			spawnPos = Merman::SpawnPos();
			if (spawnPos > 0)
			{
				Merman *merman = new Merman();
				Effect *effect;
				for (int i = 0; i < 4; i++)
				{
					effect = new Effect(merman->GetX(), merman->GetY(), EFFECT_TYPE_WATER, i);
					objects[int(effect->GetX() / 256)].push_back(effect);
				}
				objects[int(merman->GetX() / 256)].push_back(merman);
				
				waterSound->playSound();
			}
			break;
		case 4:
			break; 
		case 5:
			break;
		case 6:
			spawnPos = Head::SpawnPos();
			if (spawnPos > 0)
			{
				Head *head;
				if (spawnPos == 1)
					head = new Head(camera->GetX() - camera->GetScreenWidth() / 2, simon->GetY() - 20, 1);
				else
					head = new Head(camera->GetX() + camera->GetScreenWidth() / 2, simon->GetY() - 20, -1);

				objects[int(head->GetX() / 256)].push_back(head);
			}
			break;
		case 7:
			break;
		}

	simon->Update(dt, &objects);

	// if not in a boss fight, update camera, otherwise if simon get out of screen, get him back
	if ((hasBossFight && !inBossFight) || !hasBossFight)
		camera->Update();
	else
	{
		simon->SetPosition(max(simon->GetX(), camera->GetX() - SCREEN_WIDTH / 2), simon->GetY());
		simon->SetPosition(min(simon->GetX(), camera->GetX() + (SCREEN_WIDTH - 55) / 2), simon->GetY());
	}
	meleeWeapon->Update(dt, &objects);
	stats->Update(current_Stage);
	for (UINT i = 0; i < objects.size(); i++)
		for (UINT j = 0; j < objects[i].size(); j++)
			objects[i][j]->Update(dt, &objects);

	if (hasBossFight && !inBossFight)
		if ((bossFight_x > 0 && camera->GetX() + (SCREEN_WIDTH - 25) / 2 >= bossFight_x)
			|| (bossFight_x == 0 && camera->GetX() - (SCREEN_WIDTH) / 2 <= 0))
			AddBoss();
}

/*
	Render a frame
*/
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		if (simon->GetIsflickeringBackground() && flag_flicker)
			d3ddv->ColorFill(bb, NULL, WHITE_COLOR);
		else 
			d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		flag_flicker = !flag_flicker;

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		backgrounds[current_Stage]->Render();

		for (UINT i = 0; i < objects.size(); i++)
			for (UINT j = 0; j < objects[i].size(); j++)
				objects[i][j]->Render();

		simon->Render();
		meleeWeapon->Render();
		stats->Render();
		if (hasBossFight && !inBossFight) 
			boss->Render();

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

void RenderBackground(int background_id)
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		backgrounds[background_id]->Render();

		spriteHandler->End();
		d3ddv->EndScene();
	}
	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

void RunningIntroScene(DWORD dt)
{
	// phase 0: get simon walk to the gate
	if (simon->GetX() > 122)
	{
		simon->SetState(SIMON_STATE_WALKING_LEFT);
		simon->Update(dt, &objects);
		flag_intro = false;
	}

	//phase 1: show simon standing against gate, wait 0.5s
	if (simon->GetX() <= 122 && !flag_intro)
	{
		simon->SetState(SIMON_STATE_TURN_BACK);
		phaseTime = GetTickCount() + 1000;
		flag_intro = true;
	}

	//phase 2: change to stage 1
	if (flag_intro && GetTickCount() >= phaseTime)
	{
		sceneID = ID_NORMAL_SCENE;
		current_Stage++;
		LoadStageInfo();
		simon->SetState(SIMON_STATE_IDLE_RIGHT);
	}
	camera->Update();
}

void RunningChangeScene(DWORD dt)
{
	// Get the door that is using
	if (usingDoor == NULL)
		for (UINT i = 0; i < objects.size(); i++)
			for (UINT j = 0; j < objects[i].size(); j++)
				if (dynamic_cast<Door *>(objects[i][j]))
				{
					Door *door = dynamic_cast<Door *>(objects[i][j]);
					if (door->GetIsUsed())
					{
						usingDoor = door;
						break;
					}
				}

	// Phase 1: set simon state to idle
	if (simon->GetIsFalling())
	{
		//Update(dt);
		simon->Update(dt, &objects);
		if (!simon->GetIsFalling())
			if (usingDoor->nx == -1)
				simon->SetState(SIMON_STATE_IDLE_RIGHT);
			else
				simon->SetState(SIMON_STATE_IDLE_LEFT);
		return;
	}
	if (usingDoor->nx == -1)
	{
		// Phase 2: set position of camera to position of door
		float camera_x = camera->GetX();
		float door_x = usingDoor->GetX();

		if (camera_x < door_x)
		{
			camera->SetX(camera_x + CAMERA_CHANGING_SCENE_SPEED * dt);

			if (camera->GetX() > door_x)
				camera->SetX(door_x);

			camera_x = camera->GetX();
		}

		// Phase 3: render opening door animations, once it's done, get simon to walk through the door.
		float simon_x = simon->GetX();

		if (camera_x == door_x)
			// if simon has walked through the door, get him to do it
			if (simon_x < door_x + 50)
			{
				// if the door hasn't opened yet, open it first
				if (usingDoor->GetState() == DOOR_STATE_CLOSED)
				{
					sounddoor->playSound();
					usingDoor->SetState(DOOR_STATE_OPENING);
				}
				else if (usingDoor->GetState() == DOOR_STATE_OPENED)
				{
					if (simon->GetState() % 2 == 0)
						simon->SetState(SIMON_STATE_WALKING_RIGHT);
					else
						simon->SetState(SIMON_STATE_WALKING_LEFT);

					simon->Update(dt, &objects);
					simon_x = simon->GetX();
				}
			}
		// when simon has done, close the door, move camera a bit
			else
			{
				
				if (simon->GetState() % 2 == 0)
					simon->SetState(SIMON_STATE_IDLE_RIGHT);
				else
					simon->SetState(SIMON_STATE_IDLE_LEFT);

				if (usingDoor->GetState() == DOOR_STATE_OPENED)
				{
					sounddoor->playSound();
					usingDoor->SetState(DOOR_STATE_CLOSING);
				}
				else if (usingDoor->GetState() == DOOR_STATE_CLOSED)
					camera->SetX(camera_x + CAMERA_CHANGING_SCENE_SPEED * dt);
				camera_x = camera->GetX();
			}

		// Phase 4: Move camera
		if (camera_x > door_x)
		{
			camera->SetX(camera_x + CAMERA_CHANGING_SCENE_SPEED * dt);

			if (camera->GetX() > door_x + 16 + SCREEN_WIDTH / 2)
			{
				camera->SetX(door_x + 16 + SCREEN_WIDTH / 2);
				simon->SetIsChangingScene(false);
				sceneID = ID_NORMAL_SCENE;
				usingDoor = NULL;
			}
			camera_x = camera->GetX();
		}
	} 
	else
	{
		// Phase 2: set position of camera to position of door
		float camera_x = camera->GetX();
		float door_x = usingDoor->GetX();

		if (camera_x > door_x)
		{
			camera->SetX(camera_x - CAMERA_CHANGING_SCENE_SPEED * dt);

			if (camera->GetX() < door_x)
				camera->SetX(door_x);

			camera_x = camera->GetX();
		}

		// Phase 3: render opening door animations, once it's done, get simon to walk through the door.
		float simon_x = simon->GetX();

		if (camera_x == door_x)
			// if simon has walked through the door, get him to do it
			if (simon_x > door_x - 50)
			{
				// if the door hasn't opened yet, open it first
				if (usingDoor->GetState() == DOOR_STATE_CLOSED)
				{
					usingDoor->SetState(DOOR_STATE_OPENING);
					sounddoor->playSound();
				}
				else if (usingDoor->GetState() == DOOR_STATE_OPENED)
				{
					if (simon->GetState() % 2 == 0)
						simon->SetState(SIMON_STATE_WALKING_RIGHT);
					else
						simon->SetState(SIMON_STATE_WALKING_LEFT);

					simon->Update(dt, &objects);
					simon_x = simon->GetX();
				}
			}
		// when simon has done, close the door, move camera a bit
			else
			{
				if (simon->GetState() % 2 == 0)
					simon->SetState(SIMON_STATE_IDLE_RIGHT);
				else
					simon->SetState(SIMON_STATE_IDLE_LEFT);

				if (usingDoor->GetState() == DOOR_STATE_OPENED)
				{
					usingDoor->SetState(DOOR_STATE_CLOSING);
					sounddoor->playSound();
				}
				else if (usingDoor->GetState() == DOOR_STATE_CLOSED)
					camera->SetX(camera_x - CAMERA_CHANGING_SCENE_SPEED * dt);
				camera_x = camera->GetX();
			}

		// Phase 4: Move camera
		if (camera_x < door_x)
		{
			camera->SetX(camera_x - CAMERA_CHANGING_SCENE_SPEED * dt);

			if (camera->GetX() < door_x + 47 - SCREEN_WIDTH / 2 - 24)
			{
				camera->SetX(door_x + 47 - SCREEN_WIDTH / 2 - 24);
				simon->SetIsChangingScene(false);
				sceneID = ID_NORMAL_SCENE;
				usingDoor = NULL;
			}
			camera_x = camera->GetX();
		}

	}
}

void RunningDeathScene()
{
	/* phase 0: show the player simon's death
	   phase 1: show the death scene 
	   phase 2: wait for player */
	sound->StopSound();
	if (phase == 0)
	{
		phaseTime = GetTickCount() + 1000;
		flag_death = false;
		phase++;
	}
	if (phase == 1)
	{
		Render();
		if (GetTickCount() >= phaseTime)
		{
			ClearResources();
			phase++;
		}
	}
	if (phase == 2)
	{
		RenderBackground(9);
		if (flag_death)
		{
			LoadResources();
			sceneID = ID_NORMAL_SCENE;
			if (current_Stage < 4)
				sound->SetSound(-1, music_BR1, -1);
			else
				sound->SetSound(-1, music_BR4, -1);
			sound->playSound();
		}
	}
}

void RunningWinScene()
{
	if (!flag_stoped)
	{
		sound->StopSound();
		sound->SetSound(-1, music_WIN, 0);
		sound->playSound();
		flag_stoped = true;
	}

	if (!flag_win)
	{
		LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
		LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
		LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

		if (d3ddv->BeginScene())
		{
			// Clear back buffer with a color
			d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

			spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

			winScene->Render();

			spriteHandler->End();
			d3ddv->EndScene();
		}
		// Display back buffer content to the screen
		d3ddv->Present(NULL, NULL, NULL, NULL);
	}
	else
	{
		sceneID = ID_TITLE_SCENE;
		ClearResources();
		LoadMap();
		LoadResources();
		flag_win = false;
	}
}

void Run()
{	
	MSG msg;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{

			frameStart = now;
			if (simon->GetIsChangingScene()) sceneID = ID_CHANGING_SCENE;
			if (simon->GetIsDead()) sceneID = ID_DEATH_SCENE;
			switch (sceneID)
			{
			case ID_TITLE_SCENE:
				game->ProcessKeyboard();
				RenderBackground(8);
				break;
			case ID_INTRO_SCENE:
				RunningIntroScene(dt);
				Render();
				break;
			case ID_NORMAL_SCENE:
				game->ProcessKeyboard();
				Render();
				Update(dt);
				break;
			case ID_CHANGING_SCENE:
				RunningChangeScene(dt);
				Render();
				break;
			case ID_DEATH_SCENE:
				game->ProcessKeyboard();
				RunningDeathScene();
				break;
			case ID_WIN_SCENE:
				game->ProcessKeyboard();
				RunningWinScene();
				break;
			default:
				break;
			}

			if (!soundIsSet)
			{
				switch (current_Stage)
				{
				case 0:
					sound->StopSound();
					sound->SetSound(-1, music_BR0, -1);
					break;
				case 1:
					sound->StopSound();
					sound->SetSound(-1, music_BR1, -1);
					break;
				case 4:
					sound->StopSound();
					sound->SetSound(-1, music_BR4, -1);
					break;
				}
				sound->playSound();
				soundIsSet = true;
			}
		}
		else
			Sleep(tickPerFrame - dt);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);

	LoadMap();
	LoadResources();
	Run();

	return 0;
}