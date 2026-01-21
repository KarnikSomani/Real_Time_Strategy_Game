/**
 * @file RealTime_Strategy_game.cpp
 * @brief Defines api to build objects and run the application
 */
#include <iostream>
#include "commonIncludes.h"
#include "battlefieldMap.h"
#include "battlefieldPathFinder.h"

using namespace std;

int main()
{
	// setup the battlefield map
	battelefieldMap fieldMap;
	if(RET_STATUS_OK != fieldMap.battlefieldMapInit())
	{
		cout<<"battlefieldMapInit status not ok"<<endl;
		return 1;
	}

	battlefieldPathFinder pathFinder;
	if(RET_STATUS_OK != pathFinder.initPathFinder(&fieldMap))
	{
		cout<<"initPathFinder status not ok"<<endl;
		return 1;
	}

	if(RET_STATUS_OK != pathFinder.runPathFinder())
	{
		cout<<"runPathFinder status not ok"<<endl;
		return 1;
	}

	if(RET_STATUS_OK != fieldMap.renderPathToJsonMap(pathFinder.visitedTerain))
	{
		cout<<"renderPathToJsonMap status not ok"<<endl;
		return 1;
	}

	return 0;
}
