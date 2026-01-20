#pragma once

#include "commonIncludes.h"

#define CURR_DIR_ARR_LEN	200U

using namespace std;
using json = nlohmann::json;

typedef enum{
	NEIGHBOUR_TYPE_FIRST = 0,
	NEIGHBOUR_TYPE_LEFT = NEIGHBOUR_TYPE_FIRST,
	NEIGHBOUR_TYPE_RIGHT,
	NEIGHBOUR_TYPE_TOP,
	NEIGHBOUR_TYPE_BOTTOM,
	NEIGHBOUR_TYPE_LAST = NEIGHBOUR_TYPE_BOTTOM
}eNeighbourType;

class battelefieldMap
{
public:
	uint32_t battleUnitStartPos;
	uint32_t battleUnitStartPosXAxis;
	uint32_t battleUnitStartPosYAxis;

	uint32_t battleUnitTargetPos;
	uint32_t battleUnitTargetPosXAxis;
	uint32_t battleUnitTargetPosYAxis;

	uint32_t battlefieldLenXAxis = 0U;
	uint32_t battlefieldLenYAxis = 0U;

	json battlefieldMapDataJson;
	vector<float> battlefieldTerrainData;

	battelefieldMap(){}
	~battelefieldMap(){}

	char currentDir[CURR_DIR_ARR_LEN];
	eRetStatus battlefieldMapInit(void);
	void getMapPosIdxToXY(uint32_t posIdx, uint32_t &posX, uint32_t &posY);
	void getMapPosXYToIdx(uint32_t &posIdx, uint32_t &posX, uint32_t &posY);
	eRetStatus getNeighbourIdx(uint32_t currPosIdx, eNeighbourType neighbourType, uint32_t &neighbourPosIdx);
	eRetStatus renderPathToJsonMap(vector<int32_t> &visitedTerrains);
};
