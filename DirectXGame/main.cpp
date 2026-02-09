#include <Windows.h>
#include "KamataEngine.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "Tutorial.h"
#include "Clear.h"
#include "Time.h"
#include "BGMSet.h"

using namespace KamataEngine;

enum class Scene {

	kUnknown = 0,
	kTitle,
	kTutorial,
	kGame,
	kClear,
};

Scene scene = Scene::kUnknown;

GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;
Tutorial* tutorialScene = nullptr;
Clear* clearScene = nullptr;
Time* timer_ = nullptr;
BGMSet* bgm_ = nullptr;

uint32_t gameClearBgmHandle_;
uint32_t gamePlayBgmHandle_ ;
uint32_t titleBgmHandle_ ;

void ChangeScene();

void UpdateScene();

void DrawScene();

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	//エンジンの初期化
	KamataEngine::Initialize(L"LE2C_02_アキモト_カズキ_ターゲットを撃ち落とせ");

	scene = Scene::kTitle;
	titleScene = new TitleScene;
	titleScene->Initialize();
	timer_ = new Time;
	bgm_ = new BGMSet();
	bgm_->Initialize();

	titleBgmHandle_ = Audio::GetInstance()->LoadWave("./BGM/title.mp3");
	gamePlayBgmHandle_ = Audio::GetInstance()->LoadWave("./BGM/gamePlay.mp3");
	gameClearBgmHandle_ = Audio::GetInstance()->LoadWave("./BGM/gameClear.mp3");
	
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

	//エンジンの終了処理
	KamataEngine::Finalize();

	return 0;
}

void ChangeScene()
{
	switch (scene) 
	{
	case Scene::kTitle:
		if (!bgm_->IsPlaying()) 
		{
			bgm_->BGMPlay(titleBgmHandle_);
		}
		if (titleScene->IsFinished()) 
		{
			scene = Scene::kTutorial;
			delete titleScene;
			titleScene = nullptr;
			tutorialScene = new Tutorial;
			tutorialScene->Initialize();
		}
		break;
	case Scene::kTutorial:
		if (tutorialScene->IsFinished()) 
		{
			bgm_->BGMStop();
			scene = Scene::kGame;
			timer_->Initialize();
			delete tutorialScene;
			tutorialScene = nullptr;
			gameScene = new GameScene;
			gameScene->Initialize(timer_);
		}
		break;
	case Scene::kGame:
		if (!bgm_->IsPlaying()) {
			bgm_->BGMPlay(gamePlayBgmHandle_);
		}
		if (gameScene->IsFinished())
		{
			bgm_->BGMStop();
			scene = Scene::kClear;
			delete gameScene;
			gameScene = nullptr;	
			clearScene = new Clear;
			clearScene->Initialize();
			
		}
		break;
	case Scene::kClear:
		if (!bgm_->IsPlaying()) 
		{
			bgm_->BGMPlay(gameClearBgmHandle_);
		}
		if (clearScene->IsFinished())
		{
			bgm_->BGMStop();
			scene = Scene::kTitle;
			delete clearScene;
			clearScene = nullptr;
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
	}
}
