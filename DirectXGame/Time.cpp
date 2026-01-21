#include "Time.h"

void Time::Initialize()
{
	commaTime_ = 0.0f;
	secondTime_ = 0.0f;
	minutesTime_ = 0.0f;
}

void Time::Update()
{
	if (countTime_)
	{
		commaTime_ += 1.0f;
		if (commaTime_ >= 60.0f)
		{
			commaTime_ = 0.0f;
			secondTime_ += 1.0f;
		}
		if (secondTime_ >= 60.0f)
		{
			secondTime_ = 0.0f;
			minutesTime_ += 1.0f;
		}
	}
}

void Time::SpriteDraw()
{

}
