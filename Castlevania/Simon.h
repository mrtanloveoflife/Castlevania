#pragma once
#include "Item.h"
#include "MeleeWeapon.h"
#include "RangedWeapon.h"
#include "Brick.h"
#include "Textures.h"
#include "Background.h"
#include "Stairs.h"
#include "Door.h"
#include "MovingBlock.h"
#include "BreakableBrick.h"
#include "Stats.h"
#include "Thorn.h"
#include "SnakeHead.h"
#include "Soldier.h"
#include "Panther.h"
#include "Skull.h"
#include "Sound.h"
#include "Bat2.h"

#define SIMON_WALKING_SPEED			0.08f
#define SIMON_WALKING_STAIRS_SPEED	0.05f
#define SIMON_JUMP_SPEED_Y			0.25f
#define SIMON_ATTACKING_TIME		300

#define SIMON_STATE_IDLE_RIGHT					0
#define SIMON_STATE_IDLE_LEFT					1
#define SIMON_STATE_WALKING_RIGHT				2
#define SIMON_STATE_WALKING_LEFT				3
#define SIMON_STATE_CROUCH_RIGHT				4
#define SIMON_STATE_CROUCH_LEFT					5
#define SIMON_STATE_JUMPING_RIGHT				6
#define SIMON_STATE_JUMPING_LEFT				7
#define SIMON_STATE_ATTACK_RIGHT				8
#define SIMON_STATE_ATTACK_LEFT					9
#define SIMON_STATE_CROUCHING_ATTACK_RIGHT		10
#define SIMON_STATE_CROUCHING_ATTACK_LEFT		11
#define SIMON_STATE_THROW_RIGHT					12
#define SIMON_STATE_THROW_LEFT					13
#define SIMON_STATE_CROUCHING_THROW_RIGHT		14
#define SIMON_STATE_CROUCHING_THROW_LEFT		15

#define SIMON_STATE_IDLE_UPSTAIRS_RIGHT			16
#define SIMON_STATE_IDLE_UPSTAIRS_LEFT			17
#define SIMON_STATE_IDLE_DOWNSTAIRS_RIGHT		18
#define SIMON_STATE_IDLE_DOWNSTAIRS_LEFT		19
#define SIMON_STATE_WALKING_UPSTAIRS_RIGHT		20
#define SIMON_STATE_WALKING_UPSTAIRS_LEFT		21
#define SIMON_STATE_WALKING_DOWNSTAIRS_RIGHT	22
#define SIMON_STATE_WALKING_DOWNSTAIRS_LEFT		23
#define SIMON_STATE_ATTACKING_UPSTAIRS_RIGHT	24
#define SIMON_STATE_ATTACKING_UPSTAIRS_LEFT		25
#define SIMON_STATE_ATTACKING_DOWNSTAIRS_RIGHT	26
#define SIMON_STATE_ATTACKING_DOWNSTAIRS_LEFT	27
#define SIMON_STATE_THROWING_UPSTAIRS_RIGHT		28
#define SIMON_STATE_THROWING_UPSTAIRS_LEFT		29
#define SIMON_STATE_THROWING_DOWNSTAIRS_RIGHT	30
#define SIMON_STATE_THROWING_DOWNSTAIRS_LEFT	31
#define SIMON_STATE_HURTING_RIGHT				32
#define SIMON_STATE_HURTING_LEFT				33
#define SIMON_STATE_DIE_RIGHT					34
#define SIMON_STATE_DIE_LEFT					35

#define SIMON_STATE_TURN_BACK					36

#define SIMON_WALKING_BBOX_WIDTH  16
#define SIMON_WALKING_BBOX_HEIGHT 31
#define SIMON_CROUCHING_BBOX_WIDTH  16
#define SIMON_CROUCHING_BBOX_HEIGHT 23

#define SOUND_COLLECT			"Sounds\\sound\\collectitem.wav"
#define SOUND_COLLECT_WEAPON	"Sounds\\sound\\collectweapon.wav"
#define SOUND_COLLECT_CROSS		"Sounds\\sound\\holycross.wav"
#define SIMON_SPRITES_PATH		"Simon Belmont\\Simon Sprites.txt"

class CSimon : public CGameObject
{
	static CSimon * __instance;
	bool isAttacking;
	bool isThrowing;
	DWORD attackingTime;
	bool isJumpingForward;
	bool isOnStairs;
	Stairs *currentStairs;
	bool isUntouchable;
	bool firstHurtJump;
	DWORD untouchableTime;
	bool isChangingScene;
	DWORD FlickeringTime;
	int health;
	bool isDead;
	bool didGetCrystal;
	Sound *collectSound, *collectWeaponSound, *collectCrossSound;
	int maximumThrows, numberOfThrows;
	int rangedType;
public:
	int ny; // note if simon go up or down
	static CSimon *GetInstance();
	static CSimon *GetInstance(int ID_TEX_SIMON, LPCWSTR SIMON_TEXTURE_PATH, D3DCOLOR SIMON_TEXTURE_BACKGROUND_COLOR);

	CSimon(int ID_TEX_SIMON, LPCWSTR SIMON_TEXTURE_PATH, D3DCOLOR SIMON_TEXTURE_BACKGROUND_COLOR);

	void SetState(int state);
	bool GetIsAttacking() { return isAttacking; }
	bool GetIsThrowing() { return isThrowing; }
	void SetIsJumpingForward(bool value) { isJumpingForward = value; }
	bool GetIsJumpingForward() { return isJumpingForward; }
	bool GetIsOnStairs() { return isOnStairs; }
	bool IsUpstairs();
	bool GetIsUntouchable() { return isUntouchable; }
	bool GetIsChangingScene() { return isChangingScene; }
	void SetIsChangingScene(bool value) { isChangingScene = value; }
	void ResetState();
	void SetUntouchable(int time);
	bool GetIsflickeringBackground() { return GetTickCount() < FlickeringTime; }
	int GetHealth() { return health; }
	bool GetIsDead() { return isDead; }
	bool GetDidGetCrystal() { return didGetCrystal; }
	void SetDidGetCrystal(int value) { didGetCrystal = value; }
	void DecreaseNumberOfThrows() { numberOfThrows--; }
	int GetRangedType() { return rangedType; }
	bool CanThrow() { return numberOfThrows < maximumThrows; }
	int GetMaximumThrows() { return maximumThrows; }
	void SetNumberOfThrows(int value) { numberOfThrows = value; }

	bool MovingBlockCollision(vector<vector<LPGAMEOBJECT>> *Objects = NULL);
	bool BricksCollision(vector<vector<LPGAMEOBJECT>> *Objects = NULL);
	void ItemsCollision(vector<vector<LPGAMEOBJECT>> *Objects = NULL);
	void EnemiesCollision(vector<vector<LPGAMEOBJECT>> *Objects = NULL);
	int CheckOnStairs(vector<vector<LPGAMEOBJECT>> *Objects, bool isUp);
	bool CheckOutStairs(bool isUp);
	void DoorsCollision(vector<vector<LPGAMEOBJECT>> *Objects = NULL);

	virtual void Update(DWORD dt, vector<vector<LPGAMEOBJECT>> *coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};