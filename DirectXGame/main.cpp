#include <Windows.h>
#include "KamataEngine.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "Tutorial.h"
#include "Clear.h"
#include "Over.h"

using namespace KamataEngine;

enum class Scene {

	kUnknown = 0,
	kTitle,
	kTutorial,
	kGame,
	kClear,
	kOver,

};

Scene scene = Scene::kUnknown;

GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;
Tutorial* tutorialScene = nullptr;
Clear* clearScene = nullptr;
Over* overScene = nullptr;

void ChangeScene();

void UpdateScene();

void DrawScene();

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	//エンジンの初期化
	KamataEngine::Initialize(L"LE2C_02_アキモト_カズキ_小さな部屋～敵を回避しろ～");

	scene = Scene::kTitle;
	titleScene = new TitleScene;
	titleScene->Initialize();
	//GameSceneの初期化
	//ゲームシーンの解放
	
	// nullptrの代入
	//gameScene = nullptr;

	//メインループ
	while (true) 
	{
		//エンジンの更新
		if (KamataEngine::Update()) 
		{
			break;
		}

		ChangeScene();

		//ゲームシーンの更新
		UpdateScene();

		//描画開始
		dxCommon->PreDraw();

		//ゲームシーンの描画
		DrawScene();

		//描画終了
		dxCommon->PostDraw();
	}

	delete titleScene;
	delete gameScene;
	delete tutorialScene;
	delete clearScene;
	delete overScene;

	//エンジンの終了処理
	KamataEngine::Finalize();

	return 0;
}

void ChangeScene()
{
	switch (scene) 
	{
	case Scene::kTitle:
		if (titleScene->IsFinished()) 
		{
			scene = Scene::kGame;
			delete titleScene;
			titleScene = nullptr;
			gameScene = new GameScene;
			gameScene->Initialize();

			//scene = Scene::kTutorial;
			//delete titleScene;
			//titleScene = nullptr;
			//tutorialScene = new Tutorial;
			//tutorialScene->Initialize();
		}
		break;
	case Scene::kTutorial:
		if (tutorialScene->IsFinished()) 
		{
			scene = Scene::kGame;
			delete tutorialScene;
			tutorialScene = nullptr;
			gameScene = new GameScene;
			gameScene->Initialize();
		}
		break;
	case Scene::kGame:
		if (gameScene->IsFinished())
		{
			if (gameScene->IsClear())
			{
				scene = Scene::kClear;
				clearScene = new Clear;
				clearScene->Initialize();
			} 
			else
			{
				scene = Scene::kOver;
				overScene = new Over;
				overScene->Initialize();
			}
			
			delete gameScene;
			gameScene = nullptr;
			
		}
		break;
	case Scene::kClear:
		if (clearScene->IsFinished())
		{
			scene = Scene::kTitle;
			delete clearScene;
			clearScene = nullptr;
			titleScene = new TitleScene;
			titleScene->Initialize();
		}
		break;
	case Scene::kOver:
		if (overScene->IsFinished()) 
		{
			scene = Scene::kTitle;
			delete overScene;
			overScene = nullptr;
			titleScene = new TitleScene;
			titleScene->Initialize();
		}
		break;
	}
}

void UpdateScene()
{
	switch (scene)
	{
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kTutorial:
		tutorialScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	case Scene::kClear:
		clearScene->Update();
		break;
	case Scene::kOver:
		overScene->Update();
		break;
	}
}

void DrawScene() 
{
	switch (scene)
	{
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kTutorial:
		tutorialScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	case Scene::kClear:
		clearScene->Draw();
		break;
	case Scene::kOver:
		overScene->Draw();
		break;
	}
}
