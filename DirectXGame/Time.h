#pragma once
#include <algorithm>
#include <cassert>
#include "KamataEngine.h"

using namespace KamataEngine;

class Time 
{
public:

	enum class DisplayMode
	{
		Normal, 
		Result, 
	};

	~Time();

	void Initialize();

	void Update();

	void Draw();

	void CommaSpriteDraw();

	void SecondSpriteDraw();

	void MinutesSpriteDraw();

	void TimerKeep();

	 // ランキング描画
	void RankingDraw();

	bool SetCountTime(bool countTime) { commaTime_ = countTime; }

	void SetScoreDisplayMode(DisplayMode mode);

	void FileOpen();

	void FileWrite();

private:

	bool countTime_ = true;
	// コンマタイマ
	int commaTime_;
	// 秒
	int secondTime_;
	// 分
	int minutesTime_;

	int timerKeep_;

	

	std::array<int, 3> rankingScore_;
	std::array<int, 10> tex_ = {};                  // 配列 数字
	std::array<Sprite*, 2> commaTimeSprites_ = {}; // 配列 描画
	std::array<Sprite*, 2> secondTimeSprites_ = {}; // 配列 描画
	std::array<Sprite*, 2> minutesTimeSprites_ = {}; // 配列 描画
	std::array<std::array<KamataEngine::Sprite*, 6>, 3> rankingSprites = {};
	DisplayMode displayMode_ = DisplayMode::Normal;

};
