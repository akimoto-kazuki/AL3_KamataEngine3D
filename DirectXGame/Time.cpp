#include "Time.h"

Time::~Time() 
{
	for (int i = 0; i < 2; i++) {
		delete commaTimeSprites_[i];
		delete secondTimeSprites_[i];
		delete minutesTimeSprites_[i];
		commaTimeSprites_[i] = nullptr;
		secondTimeSprites_[i] = nullptr;
		minutesTimeSprites_[i] = nullptr;
	}
	for (int i = 0; i < 3; i++) 
	{
		for (int j = 0; j < 6; j++)
		{
			delete rankingSprites[i][j];
			rankingSprites[i][j] = nullptr;
		}
	}
}

void Time::Initialize() 
{
	commaTime_ = 0;
	secondTime_ = 0;
	minutesTime_ = 0;
	timerKeep_ = 0;

	Vector4 collar = {1, 1, 1, 1};
	// ===== 数字画像を読み込み =====
	for (int i = 0; i < 10; i++) {
		char path[64];
		sprintf_s(path, "num/%d.png", i);
		tex_[i] = TextureManager::Load(path);
	}

	FileOpen();

	SetScoreDisplayMode(DisplayMode::Normal);

	
	const Vector2 rankBasePos = {430.0f, 360.0f}; // 左上位置（下げたいならYを大きく）
	const float rankDigitSpacing = 20.0f;         // 桁間隔（数字の横幅方向）
	const float rankLineSpacing = 135.0f;         // 行間（数字列の縦方向）
	const float rankScale = 2.0f;                 // 疑似スケール（spacingで調整）

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 6; j++) {
			// spacingをrankScale分だけ広げることで見た目を「拡大」
			Vector2 pos = {rankBasePos.x + j * (rankDigitSpacing * rankScale), rankBasePos.y + i * (rankLineSpacing)};
			rankingSprites[i][j] = Sprite::Create(tex_[0], pos, collar, {0.0f, 0.0f}, false, false);
			rankingSprites[i][j]->SetSize({40, 40});
		}
	}
}

void Time::Update()
{
	if (countTime_)
	{
		commaTime_ += 1;
		if (commaTime_ >= 60)
		{
			commaTime_ = 0;
			secondTime_ += 1;
		}
		if (secondTime_ >= 60)
		{
			secondTime_ = 0;
			minutesTime_ += 1;
		}
	}
}

void Time::Draw() 
{
	CommaSpriteDraw();
	SecondSpriteDraw();
	MinutesSpriteDraw();
}

void Time::CommaSpriteDraw()
{
	int ketaNum = 2; // 桁数
	std::array<int, 10> keta = {0};
	int viewScore = commaTime_;
	for (int i = 0; i < ketaNum; i++) {
		keta[ketaNum - i - 1] = viewScore % 10;
		viewScore /= 10;
	}
	for (int i = 0; i < ketaNum; i++) 
	{
		commaTimeSprites_[i]->SetTextureHandle(tex_[keta[i]]);
		commaTimeSprites_[i]->Draw();
	}
}

void Time::SecondSpriteDraw() 
{
	int ketaNum = 2; // 桁数
	std::array<int, 10> keta = {0};
	int viewScore = secondTime_;
	for (int i = 0; i < ketaNum; i++) {
		keta[ketaNum - i - 1] = viewScore % 10;
		viewScore /= 10;
	}
	for (int i = 0; i < ketaNum; i++) {
		secondTimeSprites_[i]->SetTextureHandle(tex_[keta[i]]);
		secondTimeSprites_[i]->Draw();
	}
}

void Time::MinutesSpriteDraw()
{
	int ketaNum = 2; // 桁数
	std::array<int, 10> keta = {0};
	int viewScore = minutesTime_;
	for (int i = 0; i < ketaNum; i++) {
		keta[ketaNum - i - 1] = viewScore % 10;
		viewScore /= 10;
	}
	for (int i = 0; i < ketaNum; i++) 
	{
		minutesTimeSprites_[i]->SetTextureHandle(tex_[keta[i]]);
		minutesTimeSprites_[i]->Draw();
	}
}

void Time::TimerKeep() 
{ 
	timerKeep_ += minutesTime_ * 10000;
	timerKeep_ += secondTime_ * 100;
	timerKeep_ += commaTime_;
}

void Time::RankingDraw() 
{
	int ketaNum = 6; // 桁数
	std::array<std::array<int, 10>, 3> keta = {0};
	int viewScore[3];

	for (int i = 0; i < 3; i++) 
	{
		viewScore[i] = rankingScore_[i];
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < ketaNum; j++) {
			keta[i][ketaNum - j - 1] = viewScore[i] % 10;
			viewScore[i] /= 10;
		}
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < ketaNum; j++) {

			rankingSprites[i][j]->SetTextureHandle(tex_[keta[i][j]]);
			rankingSprites[i][j]->Draw();
		}
	}
}

void Time::SetScoreDisplayMode(DisplayMode mode) 
{
	displayMode_ = mode;

	// 共通の色
	Vector4 color = {1, 1, 1, 1};

	// 各モードごとの設定
	float scale = 1.0f; // スプライトの拡大率
	float baseX = 0.0f;
	float baseY = 0.0f;
	float spacing = 10.0f; // 行間隔

	if (mode == DisplayMode::Normal) {
		scale = 50.0f;
		baseX = 920.0f;
		baseY = 10.0f;
		spacing = 35.0f;
	} else if (mode == DisplayMode::Result) {
		scale = 70.0f;
		baseX = 600.0f; // 画面中央寄りに
		baseY = 150.0f;
		spacing = 45.0f;
	}
	
	// 一旦既存スプライト削除（再作成するため）
	for (int i = 0; i < 2; i++)
	{
		delete commaTimeSprites_[i];
		delete secondTimeSprites_[i];
		delete minutesTimeSprites_[i];
		commaTimeSprites_[i] = nullptr;
		secondTimeSprites_[i] = nullptr;
		minutesTimeSprites_[i] = nullptr;
	}
	
	// 新しいスプライトを再生成
	for (int i = 0; i < 2; i++) 
	{
		Vector2 pos = {baseX + spacing * (8 + i), baseY};
		commaTimeSprites_[i] = Sprite::Create(tex_[0], pos, color, {0.0f, 0.0f}, false, false);
		// スプライトのサイズ変更
		commaTimeSprites_[i]->SetSize({scale, scale});
		Vector2 pos2 = {baseX + spacing * (5 + i), baseY};
		secondTimeSprites_[i] = Sprite::Create(tex_[0], pos2, color, {0.0f, 0.0f}, false, false);
		// スプライトのサイズ変更
		secondTimeSprites_[i]->SetSize({scale, scale});
		Vector2 pos3 = {baseX + spacing * (2 + i), baseY};
		minutesTimeSprites_[i] = Sprite::Create(tex_[0], pos3, color, {0.0f, 0.0f}, false, false);
		// スプライトのサイズ変更
		minutesTimeSprites_[i]->SetSize({scale, scale});
	}
}

void Time::FileOpen() 
{
	FILE* fp;
	char scoreFile[] = "score.txt";
	int err;
	err = fopen_s(&fp, scoreFile, "r");
	int readScore = {};
	if (err != 0) {
		// return err;
	} else {
		for (int i = 0; i < 3; i++) {
			while (fscanf_s(fp, "%d", &readScore) == 10) {
				fscanf_s(fp, "%d", &readScore);
			}
			rankingScore_[i] = readScore;
		}

		fclose(fp);
	}
}

void Time::FileWrite() 
{
	FILE* fp;
	char scoreFile[] = "score.txt";
	int err;
	err = fopen_s(&fp, scoreFile, "r");
	int readScore = {};
	if (err != 0) {
		// return err;
	} else {
		for (int i = 0; i < 3; i++) {
			while (fscanf_s(fp, "%d", &readScore) == 10) {
				fscanf_s(fp, "%d", &readScore);
			}
			rankingScore_[i] = readScore;
		}
		fclose(fp);
	}
	err = fopen_s(&fp, scoreFile, "w");
	if (err != 0) {
		// return err;
	}
	else 
	{
		if (timerKeep_ > rankingScore_[0])
		{
			while (1) {
				fprintf_s(fp, "%d\n", timerKeep_);
				fprintf_s(fp, "%d\n", rankingScore_[0]);
				fprintf_s(fp, "%d\n", rankingScore_[1]);
				break;
			}
		} 
		else if (timerKeep_ > rankingScore_[1])
		{
			while (1) {
				fprintf_s(fp, "%d\n", rankingScore_[0]);
				fprintf_s(fp, "%d\n", timerKeep_);
				fprintf_s(fp, "%d\n", rankingScore_[1]);
				break;
			}
		} 
		else if (timerKeep_ > rankingScore_[2]) 
		{
			while (1) {
				fprintf_s(fp, "%d\n", rankingScore_[0]);
				fprintf_s(fp, "%d\n", rankingScore_[1]);
				fprintf_s(fp, "%d\n", timerKeep_);
				break;
			}
		}
		else 
		{
			while (1) {
				fprintf_s(fp, "%d\n", rankingScore_[0]);
				fprintf_s(fp, "%d\n", rankingScore_[1]);
				fprintf_s(fp, "%d\n", rankingScore_[2]);
				break;
			}
		}

		fclose(fp);
	}
}
