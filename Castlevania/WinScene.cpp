#include "WinScene.h"

WinScene *WinScene::__instance = NULL;

WinScene *WinScene::GetInstance(int ID_TEX_WIN_SCENE, LPCWSTR WIN_SCENE_TEXTURE_PATH, D3DCOLOR WIN_SCENE_TEXTURE_BACKGROUND_COLOR)
{
	__instance = new WinScene(ID_TEX_WIN_SCENE, WIN_SCENE_TEXTURE_PATH, WIN_SCENE_TEXTURE_BACKGROUND_COLOR);
	return __instance;
}

WinScene *WinScene::GetInstance()
{
	return __instance;
}

WinScene::WinScene(int ID_TEX_WIN_SCENE, LPCWSTR WIN_SCENE_TEXTURE_PATH, D3DCOLOR WIN_SCENE_TEXTURE_BACKGROUND_COLOR)
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_WIN_SCENE, WIN_SCENE_TEXTURE_PATH, WIN_SCENE_TEXTURE_BACKGROUND_COLOR);

	CSprites * sprites = CSprites::GetInstance();

	LPDIRECT3DTEXTURE9 texWinScene = textures->Get(ID_TEX_WIN_SCENE);

	ifstream in;
	in.open(WINSCENE_SPRITES_PATH);
	int n, nState, id, left, top, right, bottom;
	
	in >> nState;
	for (int i = 0; i < nState; i++)
	{
		in >> n;
		for (int j = 0; j < n; j++)
		{
			in >> id >> left >> top >> right >> bottom;
			sprites->Add(id, left, top, right, bottom, texWinScene);
		}
	}

	in.close();
}

void WinScene::Render()
{
	CSprites * sprites = CSprites::GetInstance();

	sprites->Get(140001)->Draw(0, 0);

	string toString = std::to_string(score);

	for (UINT i = 0; i < toString.length(); i++)
		sprites->Get(140010 + toString[i] - 48)->Draw(106 + i * 12 * 1.0f, 139);

}


WinScene::~WinScene()
{
}
