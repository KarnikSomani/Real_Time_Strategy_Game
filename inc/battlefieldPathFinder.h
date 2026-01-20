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

    battelefieldMap *fieldMap = NULL;
    list<searchNode> searchTerrainQ;
    vector<int32_t> visitedTerain;
    eRetStatus initPathFinder(battelefieldMap *battelefieldMap);
    eRetStatus runPathFinder();
    eRetStatus getNextNeighbours(uint32_t currTerrain, list<searchNode> &nextNeighbours);
};
