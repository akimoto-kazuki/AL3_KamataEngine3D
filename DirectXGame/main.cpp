#include <Windows.h>
#include "KamataEngine.h"
#include "GameScene.h"

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	
	//エンジンの初期化
	KamataEngine::Initialize(L"LE2D_03_アキモト_カズキ_AL3");
	// ゲームシーンのインスタンス
	GameScene* gameScene = new GameScene();
	// GameSceneの初期化
	gameScene->Initialize();
	//ゲームシーンの解放
	//delete gameScene;

	//メインループ
	while (true) 
	{
		//エンジンの更新
		if (KamataEngine::Update()) 
		{
			break;
		}

		//ゲームシーンの更新
		gameScene->Update();

		//描画開始
		dxCommon->PreDraw();

		//ゲームシーンの描画
		gameScene->Draw();

		//描画終了
		dxCommon->PostDraw();
	}
	//エンジンの終了処理
	KamataEngine::Finalize();
	return 0;
}
