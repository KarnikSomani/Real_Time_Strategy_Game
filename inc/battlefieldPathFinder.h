/**
 * @file battlefieldPathFinder.h
 * @brief Declares class and member to execute path finding algorithm
 */
#pragma once

#include "commonIncludes.h"
#include "battlefieldMap.h"
using namespace std;

typedef struct{
    uint32_t mapIndex;
    uint32_t totalDeviation;
} searchNode;

class battlefieldPathFinder
{
public:
    battlefieldPathFinder(){}
    ~battlefieldPathFinder(){}

    battelefieldMap *fieldMap = NULL; ///< pointer to battlefield map class object 
    list<searchNode> searchTerrainQ; ///< queue to mainitain terrains whose neighbours are yet be found.
    vector<int32_t> visitedTerain; ///< vector for storing terrains that are visited with the parent terrain that it was searched from.
    
    /**
     * @brief Initialize the class members.
     * Resolve dependency for battlefield map and init vector visitedTerrains.
     * 
     * @param[in] battelefieldMap Pointer to access battlefield terrain data and api to navigate and update the map.
     * 
     * @retval RET_STATUS_OK if success else error
     */
    eRetStatus initPathFinder(battelefieldMap *battelefieldMap);

    /**
     * @brief Launches the path finder algorithm to find optimal path from
     * start to target position traversing through only ground level terrains.
     * 
     * @retval RET_STATUS_OK if success else error
     * 
     * @details The path finder alogorithm works as follows,
     * For finding the path from start to target position, breath first search algorithm has been
     * employed along and to overcome its drawbacks greedy best first search algorithm has also been used.
     * 
     * From the start terrain, for every terrain at the top of a searchQueue, thereare neighbouring terrains
     * looked for and if it has never been found before, it is added to back of the searchQueue, and the cycle repeats.
     * This way all the neighbours on the same level are visited first.
     * 
     * Searching terrains this way has a drawback that we are searching terrains in all the direction,
     * which consumes more iterations. So we always keep the searchQueue sorted such a way that we
     * iterate through those terrains first which are closest to both starting index and target index.
     * For calculating distance between two points on graph, we use the formula |x2-x1| + |y2-y1|.
     * 
     * When a new neighbour is found on the grid, its index is updated with the parent terrain index in a vector
     * called visitedTerrains. This would help to trace back the route from target terrain back to start terrain.
     */
    eRetStatus runPathFinder();

    /**
     * @brief Finds a list of next available neighbours to given terrain that are not already visited before.
     * 
     * @param[in] currTerrain Current terrain whose neighbours are to be found
     * @param[out] nextNeighbours List of newly discovered neighbours
     * 
     * @retval RET_STATUS_OK if success else error
     */
    eRetStatus getNextNeighbours(uint32_t currTerrain, list<searchNode> &nextNeighbours);
};
