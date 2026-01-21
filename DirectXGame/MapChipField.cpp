#include "MapChipField.h"
#include <map>
#include <fstream>
#include <sstream>
#include "KamataEngine.h"
#include <cassert>


namespace 
{
    std::map<char, MapChipType> MapChipTypeTable =
    {
        {'B',MapChipType::kBlock},
        {'P',MapChipType::kPlayer},
        {'E',MapChipType::kEnemy},
    };
}

    MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) 
    {
	
        if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) 
        {
		    return MapChipType::kBlank;
	    }
	    if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex)
        {
		    return MapChipType::kBlank;
	    }
	    return mapChipData_.data[yIndex][xIndex].type;
    }
    
    uint8_t MapChipField::GetMapChipSubIDByIndex(uint32_t xIndex, uint32_t yIndex) 
    {
	    return mapChipData_.data[yIndex][xIndex].subID;
    }

    KamataEngine::Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) 
    {
        return KamataEngine::Vector3(kBlockWidth * xIndex,kBlockHeight * (kNumBlockVirtical - 1 - yIndex),0); 
    }

    MapChipDataUnit MapChipField::GetMapChipDataUnit(uint32_t xIndex, uint32_t yIndex) 
    { 
        return mapChipData_.data[yIndex][xIndex];
    }

    MapChipField::IndexSet MapChipField::GetMapChipIndexSetByPosition(const KamataEngine::Vector3& position) 
    {
	    IndexSet indexSet = {};
	    indexSet.xIndex = static_cast<uint32_t>((position.x + kBlockWidth / 2) / kBlockWidth);
	    indexSet.yIndex = kNumBlockVirtical - 1 - static_cast<uint32_t>((position.y + kBlockHeight / 2) / kBlockHeight);
        return indexSet; 
    }

    MapChipField::Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) 
    {
        // 指定ブロックの中心座標を取得する
	    KamataEngine::Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);

	    Rect rect;

	    rect.left = center.x - kBlockWidth / 2.0f;
	    rect.right = center.x + kBlockWidth / 2.0f;
	    rect.bottom = center.y - kBlockHeight / 2.0f;
	    rect.top = center.y + kBlockHeight / 2.0f;

        return rect;
    }

    void MapChipField::ResetMapChipData() 
    { 
        mapChipData_.data.clear();
	    mapChipData_.data.resize(kNumBlockVirtical);
	    for (std::vector<MapChipDataUnit>& mapChipDataLine:mapChipData_.data)
        {
		    mapChipDataLine.resize(kNumBlockHorizontal);
	    }
    }

    void MapChipField::LoadMapChipCsv(const std::string& filePath) 
    { 
        ResetMapChipData();
	    std::ifstream file;
	    file.open(filePath);
	    assert(file.is_open());

        std::stringstream mapChipCsv;
	    mapChipCsv << file.rdbuf();

	    file.close();

        for (uint32_t i = 0; i < kNumBlockVirtical; ++i)
        {
		    std::string line;
		    getline(mapChipCsv, line);

            std::istringstream line_stream(line);

            for (uint32_t j = 0; j < kNumBlockHorizontal; ++j)
            {
			    std::string word;
			    getline(line_stream, word, ',');
                // 空白の場合スキップ
			    if (word.empty())
                {
				    continue;
			    }
                // 先頭文字がいずれかのマップチップ種別に該当するか確認
			    if (!MapChipTypeTable.contains(word[kChipType]))
                {
				    continue;
			    }
			    mapChipData_.data[i][j].type = MapChipTypeTable[word[kChipType]];

                if (word.size() <= kChipSubID) 
                {
				    continue;
			    }

                mapChipData_.data[i][j].subID = static_cast<uint8_t>(word[kChipSubID] - '0');
            }

	    }
    }
