#pragma once
#include <vector>
#include "KamataEngine.h"

enum class MapChipType {
	kBlank,
	kBlock,
	kPlayer,
	kEnemy,
};

struct MapChipDataUnit 
{
	MapChipType type;
	uint8_t subID;
};

enum MapChipCharIndex 
{
	kChipType = 0,
	kChipSubID = 1,
};

struct MapChipData 
{
	std::vector<std::vector<MapChipDataUnit>> data;
};

class MapChipField 
{
	
	
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

public:

	struct IndexSet
	{
		uint32_t xIndex;
		uint32_t yIndex;
	};

	struct Rect 
	{
		float left;
		float right;
		float bottom;
		float top;
	};

	MapChipData mapChipData_;

	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);

	uint32_t GetNumBlockVirtical() const { return kNumBlockVirtical; };

	uint32_t GetNumBlockHorizontal() const { return kNumBlockHorizontal; }

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	KamataEngine::Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	MapChipDataUnit GetMapChipDataUnit(uint32_t xIndex, uint32_t yIndex);

	IndexSet GetMapChipIndexSetByPosition(const KamataEngine::Vector3& position);

	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

	uint8_t GetMapChipSubIDByIndex(uint32_t xIndexx, uint32_t yIndex);

};
