#pragma once
class Time 
{
public:

	void Initialize();

	void Update();

	void SpriteDraw();

	bool SetCountTime(bool countTime) { commaTime_ = countTime; }

private:

	bool countTime_ = true;
	// コンマタイマ
	float commaTime_;
	// 秒
	float secondTime_;
	// 分
	float minutesTime_;

};
