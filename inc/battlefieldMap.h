/**
 * @file battlefieldMap.h
 * @brief Declares class and members to parse Terrain map to and from json file.
 */
#pragma once

#include "commonIncludes.h"

#define CURR_DIR_ARR_LEN 200U ///< defines length of array used to stores cwd.

using namespace std;
using json = nlohmann::json;

/// @enum Defines position of a neighbour wrt to terrain currently visited
typedef enum{
	NEIGHBOUR_TYPE_FIRST = 0,
	NEIGHBOUR_TYPE_LEFT = NEIGHBOUR_TYPE_FIRST,
	NEIGHBOUR_TYPE_RIGHT,
	NEIGHBOUR_TYPE_TOP,
	NEIGHBOUR_TYPE_BOTTOM,
	NEIGHBOUR_TYPE_LAST = NEIGHBOUR_TYPE_BOTTOM
}eNeighbourType;

/**
 * @class battlefieldMap, parses input RiskyLab generated json map into local structure
 * and provides methods to navigate through the terrain
 */
class battelefieldMap
{
public:
	uint32_t battleUnitStartPos; ///< battle unit start position in grid index
	uint32_t battleUnitStartPosXAxis; ///< battle unit start position in grid x(column) coordinate
	uint32_t battleUnitStartPosYAxis; ///< battle unit start position in grid y(column) coordinate

	uint32_t battleUnitTargetPos; ///< battle unit target position in grid index
	uint32_t battleUnitTargetPosXAxis; ///< battle unit target position in grid x(column) coordinate
	uint32_t battleUnitTargetPosYAxis; ///< battle unit target position in grid y(column) coordinate

	uint32_t battlefieldLenXAxis = 0U; ///< battlefield grid x axis length
	uint32_t battlefieldLenYAxis = 0U; ///< battlefield grid y axis length

	json battlefieldMapDataJson; ///< container to stored parsed terrain map .jason file
	vector<float> battlefieldTerrainData; ///< vector to store each terrain charateristic of the battlefield grid

	battelefieldMap(){}
	~battelefieldMap(){}

	char currentDir[CURR_DIR_ARR_LEN];

	/**
	 * @brief Init local battlefield map members by parsing .json map file.
	 * 
	 * @retval RET_STATUS_OK if success else error
	 */
	eRetStatus battlefieldMapInit(void);

	/**
	 * @brief convert battlefield position from grid index to x and y coordinates
	 * 
	 * @param[in] posIdx Index value to be converted
	 * @param[out] posX converted x coordinate
	 * @param[out] posX converted y coordinate
	 */
	void getMapPosIdxToXY(uint32_t posIdx, uint32_t &posX, uint32_t &posY);

	/**
	 * @brief convert battlefield position from x and y coordinates to grid index
	 * 
	 * @param[out] posIdx Index value to be converted
	 * @param[in] posX converted x coordinate
	 * @param[in] posX converted y coordinate
	 */
	void getMapPosXYToIdx(uint32_t &posIdx, uint32_t posX, uint32_t posY);

	/**
	 * @brief Find neighbouring terrain in direction left, right, top or bottom,
	 * for a given terrain such that they have not been searched before.
	 * 
	 * @param[in] currPosIdx current terrain
	 * @param[in] neighbourType direction of neighbour to find left, right, top or bottom
	 * @param[out] neighbourPosIdx found neighbour index
	 * 
	 * @retval RET_STATUS_OK if success else error
	 */
	eRetStatus getNeighbourIdx(uint32_t currPosIdx, eNeighbourType neighbourType, uint32_t &neighbourPosIdx);
	
	/**
	 * @brief Construct path by backtracking from target position to start position
	 * using parent index stored in each index of the visited terrains.
	 * Paint this route with an icon and dump updated battlefield grid into .json file.
	 * 
	 * @param[in] visitedTerrain vector of searched terrain with each searched 
	 * terrain holding index value of its parent terrain.
	 * 
	 * @retval RET_STATUS_OK if success else error
	 */
	eRetStatus renderPathToJsonMap(vector<int32_t> &visitedTerrains);
};
