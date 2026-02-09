#include "BGMSet.h"
#include "KamataEngine.h"
#include <stdio.h>

using namespace KamataEngine;

void BGMSet::Initialize() 
{ 
	bgmHandle_ = 0;
}

void BGMSet::BGMPlay(uint32_t bgmHandle) 
{
	if (!Audio::GetInstance()->IsPlaying(bgmHandle_)) {
		bgmHandle_ = Audio::GetInstance()->PlayWave(bgmHandle, true);
		Audio::GetInstance()->SetVolume(bgmHandle_, 0.1f);
	}
}

void BGMSet::BGMStop()
{
	if (Audio::GetInstance()->IsPlaying(bgmHandle_)) {
		Audio::GetInstance()->StopWave(bgmHandle_);
	}
}

bool BGMSet::IsPlaying() const 
{ 
	return Audio::GetInstance()->IsPlaying(bgmHandle_); 
}
