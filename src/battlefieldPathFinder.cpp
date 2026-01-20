#include "commonIncludes.h"
#include "battlefieldMap.h"
#include "battlefieldPathFinder.h"

eRetStatus battlefieldPathFinder::initPathFinder(battelefieldMap *battelefieldMap)
{
    // set battlefield map dependency
	if(NULL == battelefieldMap)
	{
		cout << "initPathFinder: battelefieldMap is NULL" << endl;
		return RET_STATUS_ERR_UNKNOWN;
	}

    fieldMap = battelefieldMap;
    visitedTerain.resize(fieldMap->battlefieldTerrainData.size());
    for(uint32_t i = 0; i < visitedTerain.size(); i++)
    {
        visitedTerain[i] = UNOCCUPIED_GROUND_ICON_VAL;
    }

    return RET_STATUS_OK;
}

eRetStatus battlefieldPathFinder::runPathFinder()
{
    bool isTargetPosReached = false;

    // push the start position into queue
    searchNode startNode;
    startNode.mapIndex = fieldMap->battleUnitStartPos;
    startNode.totalDeviation = 0U;

    searchTerrainQ.clear();
    searchTerrainQ.push_back(startNode);

    // init a while loop until target terrain index is reached
    while(!searchTerrainQ.empty())
    {
        // process item from queue
        searchNode &currTerrain = searchTerrainQ.front();
        /// @remark : vector should be used instead for nextNeighbours since we already know the max neighbours
        list<searchNode> nextNeighbours;
        if(RET_STATUS_OK != getNextNeighbours(currTerrain.mapIndex, nextNeighbours))
        {
            cout << "runPathFinder: getNextNeighbours failed" << endl;
            searchTerrainQ.pop_front();
            continue;
        }

        searchTerrainQ.pop_front();

        if(!nextNeighbours.empty())
        {
            // parse through all the neighbours
            for(searchNode neighbour : nextNeighbours)
            {
                // check target terrain is reached
                if(neighbour.mapIndex == fieldMap->battleUnitTargetPos)
                {
                    isTargetPosReached = true;
                    break;
                }

                // insert in search Q and sort in the order of accending deviated distance from linear route
                auto sortPtr = searchTerrainQ.begin();
                while (sortPtr != searchTerrainQ.end() && sortPtr->totalDeviation <= neighbour.totalDeviation)
                {
                    ++sortPtr;
                }
                searchTerrainQ.insert(sortPtr, neighbour);
            }
        }

        if(true == isTargetPosReached)
        {
            cout << "Target terrain reached. Exit." << endl;
            break;
        }
    }

    if(true == isTargetPosReached)
    {
        return RET_STATUS_OK;
    }
    else
    {
        cout << "Target terrain could not be reached." << endl;
        return RET_STATUS_ERR_UNKNOWN;
    }
}

eRetStatus battlefieldPathFinder::getNextNeighbours(uint32_t currTerrainIdx, list<searchNode> &nextNeighbours)
{
    /**
     * @note Assuming that the terrain index increment from column 0 to column n
     * and rollsover to column 0 for next row.
     * Calculate total deviation from linear path.
     */
    auto calTotalDeviation = [=](searchNode srcTerrainNode){

        int32_t srcTerrainPosX = srcTerrainNode.mapIndex % fieldMap->battlefieldLenXAxis;
        int32_t srcTerrainPosY = srcTerrainNode.mapIndex / fieldMap->battlefieldLenXAxis;

        int32_t distanceFromStartPos = abs(srcTerrainPosX - (int32_t)fieldMap->battleUnitStartPosXAxis)
                                            + abs(srcTerrainPosY - (int32_t)fieldMap->battleUnitStartPosYAxis);

        int32_t distanceFromTargetPos = abs(srcTerrainPosX - (int32_t)fieldMap->battleUnitTargetPosXAxis)
                                            + abs(srcTerrainPosY - (int32_t)fieldMap->battleUnitTargetPosYAxis);
        
        srcTerrainNode.totalDeviation = (uint32_t)(distanceFromStartPos + distanceFromTargetPos);
    };

    // find upper neighbour
    for(int32_t typeIdx = NEIGHBOUR_TYPE_FIRST; typeIdx < NEIGHBOUR_TYPE_LAST; typeIdx++)
    {
        uint32_t neighbourIdx = 0;
        if(RET_STATUS_OK == fieldMap->getNeighbourIdx(currTerrainIdx, (eNeighbourType)typeIdx, neighbourIdx))
        {
            // make sure the neighbour terrain is not already searched
        	//cout << "visited terrain of idx: " << neighbourIdx << " : " << visitedTerain[neighbourIdx] << endl;
            if(visitedTerain[neighbourIdx] != UNOCCUPIED_GROUND_ICON_VAL)
            {
                continue;
            }
            
            // Calculate deviation from linear path and add to next set of neighbours to be searched
            searchNode neighbourNode;
            neighbourNode.mapIndex = neighbourIdx;
            calTotalDeviation(neighbourNode);
            nextNeighbours.push_back(neighbourNode);

            // Set current terrain index as parent terrain index 
            visitedTerain[neighbourIdx] = currTerrainIdx;
        }
    }

    return RET_STATUS_OK;
}
