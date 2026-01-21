/**
 * @file battlefieldMap.cpp
 * @brief Definitions to parse Terrain map to and from json file.
 */
#include "commonIncludes.h"
#include "battlefieldMap.h"
#include <iostream>
#include <cstdio>
#include <direct.h>

using namespace std;
using json = nlohmann::json;

eRetStatus battelefieldMap::battlefieldMapInit(void)
{
    // Read and parse riskyLab generated map
	if(NULL != getcwd(currentDir, CURR_DIR_ARR_LEN))
	{
		cout << "cwd: " << string(currentDir) << endl;
	}

	// form 
	string mapPath = string(currentDir) + string("\\..\\") + string(BATTLEFIELD_MAP_LOCATION);
	ifstream battlefieldMapDataStream(mapPath.c_str());
    battlefieldMapDataJson = json::parse(battlefieldMapDataStream);

	if(!battlefieldMapDataJson.contains("layers"))
	{
		cout << "Input Json Map does not contain field: layers\n";
		return RET_STATUS_ERR_ARGUEMENT;
	}

	if(!battlefieldMapDataJson.contains("canvas"))
	{
		cout << "Input Json Map does not contain field: canvas\n";
		return RET_STATUS_ERR_ARGUEMENT;
	}

	if(!battlefieldMapDataJson["layers"][0].contains("data"))
	{
		cout << "Input Json Map data does not contain field: data\n";
		return RET_STATUS_ERR_ARGUEMENT;
	}

	battlefieldLenXAxis = sqrt((uint32_t)battlefieldMapDataJson["canvas"]["width"]);
	battlefieldLenYAxis = sqrt((uint32_t)battlefieldMapDataJson["canvas"]["height"]);
	 
	battlefieldTerrainData = battlefieldMapDataJson["layers"][0].value("data",vector<float>{});

	for(uint32_t idx = 0; idx < battlefieldTerrainData.size(); idx++)
	{
		if(battlefieldTerrainData[idx] == START_POS_ICON_VAL)
		{
			battleUnitStartPos = idx;
			getMapPosIdxToXY(idx, battleUnitStartPosXAxis, battleUnitStartPosYAxis);
		}
		else if(battlefieldTerrainData[idx] == TARGET_POS_ICON_VAL)
		{
			battleUnitTargetPos = idx;
			getMapPosIdxToXY(idx, battleUnitTargetPosXAxis, battleUnitTargetPosYAxis);
		}
		else
		{
			// for static analysis check
		}
	}

    return RET_STATUS_OK;
}

void battelefieldMap::getMapPosIdxToXY(uint32_t posIdx, uint32_t &posX, uint32_t &posY)
{
	posX = posIdx % battlefieldLenXAxis;
	posY = posIdx / battlefieldLenXAxis;
	
	return;
}

void battelefieldMap::getMapPosXYToIdx(uint32_t &posIdx, uint32_t posX, uint32_t posY)
{
	posIdx = (posY * battlefieldLenXAxis) + posX;
	return;
}

eRetStatus battelefieldMap::getNeighbourIdx(uint32_t currPosIdx, eNeighbourType neighbourType, uint32_t &neighbourPosIdx)
{
	uint32_t currPosX = 0;
	uint32_t currPosY = 0;

	getMapPosIdxToXY(currPosIdx, currPosX, currPosY);

	switch (neighbourType)
	{
	case NEIGHBOUR_TYPE_LEFT:
		if(currPosX > 0)
		{
			neighbourPosIdx = currPosIdx - 1;
		}
		else
		{
			return RET_STATUS_ERR_UNKNOWN;
		}
		break;
	case NEIGHBOUR_TYPE_RIGHT:
		if(currPosX < battlefieldLenXAxis - 1)
		{
			neighbourPosIdx = currPosIdx + 1;
		}
		else
		{
			return RET_STATUS_ERR_UNKNOWN;
		}
		break;
	case NEIGHBOUR_TYPE_TOP:
		if(currPosY > 0)
		{
			neighbourPosIdx = currPosIdx - battlefieldLenXAxis;
		}
		else
		{
			return RET_STATUS_ERR_UNKNOWN;
		}
		break;
	case NEIGHBOUR_TYPE_BOTTOM:
		if(currPosY < battlefieldLenYAxis - 1)
		{
			neighbourPosIdx = currPosIdx + battlefieldLenXAxis;
		}
		else
		{
			return RET_STATUS_ERR_UNKNOWN;
		}
		break;
	default:
		cout << "getNeighbourIdx: Invalid Neigbour passed" << endl;
		return RET_STATUS_ERR_UNKNOWN;
		break;
	}

	if(battlefieldTerrainData[neighbourPosIdx] == ELEVATED_GROUND_ICON_VAL)
	{
		return RET_STATUS_ERR_UNKNOWN;
	}

	return RET_STATUS_OK;
}

eRetStatus battelefieldMap::renderPathToJsonMap(vector<int32_t> &visitedTerrains)
{
	// Initiate from parent position of target position and trace back until start position is reached.
	uint32_t parentTerrainPosIdx = visitedTerrains[battleUnitTargetPos];
	while(parentTerrainPosIdx != battleUnitStartPos)
	{
		// update json map data
		battlefieldTerrainData[parentTerrainPosIdx] = PLAYER_1_ICON_VAL;
		parentTerrainPosIdx = visitedTerrains[parentTerrainPosIdx];
	}

	battlefieldMapDataJson["layers"][0]["data"] = battlefieldTerrainData;

	string mapPath = string(currentDir) + string("\\..\\") + string(BATTLEFIELD_UPDATED_MAP_LOCATION);
	if (0 == remove(mapPath.c_str())) {
        cout << "Already existing updated map file deleted\n";
	} else {
		cout << "Failed to delete or updated map file does not exist exist\n";
	}

	// output updated json data
	ofstream battlefieldUpdatedMapDataStream(string(currentDir) + string("\\..\\") + string(BATTLEFIELD_UPDATED_MAP_LOCATION));
	battlefieldUpdatedMapDataStream << battlefieldMapDataJson;
	battlefieldUpdatedMapDataStream.close();

	return RET_STATUS_OK;
}
