# Real Time Strategy Game (RTS)

RTS is a path finding algorithm game, which finds the most optimal path for a battle unit to traverse from starting point to target point in a terrain represented by grid of X by Y number of ground level terrains and elevated terrains. A battle must follow the ground level terrains to reach the target position.

### Build Environment setup
- IDE: Eclipse for C/C++ Developers
- Toolchain: MinGW GCC compiler
- Builder: Eclipse Internal CDT builder

### Build procedure
- Clone repository [Real_Time_Strategy_Game](https://github.com/KarnikSomani/Real_Time_Strategy_Game/tree/main)
- Open Eclipse -> On the welcome window open select "Add a Local Project" at the bottom of the window -> Enter "Project path" i.e root director of the repository -> Enter "Workspace" same as Project path -> Click on Finish.
- Once the project gets imported, go to Project -> Build All to start building.
- By default the build configuration is set in Release mode.
- Build executable can be found in Root -> Release -> RealTime_Strategy_game.exe

### Getting Started
To get started with using the application, user needs to
- Generate a Terrain Map ( .json file ) using [RiskyLab Tile Map Editor](https://riskylab.com/tilemap/#).
- Following items needs to be added into the Map. 1] Start position 2] Target Position 3] All the elevated terrains that cannot be traversed. An icon can be picked from from the Tileset on the left side of the map.
- Take a note of the icon values as the its position on the grid (column number.row number).
- Export the Map and place it under root/config folder.
- User need to update various macros in /config/battlefieldConfig.h to reflect the icon value for each type of icon noted previously.
- Now build the application and run it.

### Soup Libraries
- [nlohmann / json](https://github.com/nlohmann/json) Used for parsing back and forth a RiskyLab generated Json Terrain Map file.

### Software Detailed Design
#### Components
class - battlefieldMap
class - battlefieldPathFinder
Library - nlohmann / json
#### API Documentaion
- Doxygen format has used for commenting in the codebase.
- User may follow following steps to generate doxygen based api documentaion .
#### Design
For finding the path from start to target position, breath first search algorithm has been employed along and to overcome its drawbacks greedy best first search algorithm has also been used.

From the start terrain, for every terrain at the top of a searchQueue, there are neighbouring terrains looked for and if it has never been found before, it is added to back of the searchQueue, and the cycle repeats.
This way all the neighbours on the same level are visited first.

Searching terrains this way has a drawback that we are searching terrains in all the direction, which consumes more iterations. So we always keep the searchQueue sorted such a way that we iterate through those terrains first which are closest to both starting index and target index. For calculating distance between two points on graph, we use the formula |x2-x1| + |y2-y1|.

When a new neighbour is found on the grid, its index is updated with the parent terrain index in a vector called visitedTerrains. This would help to trace back the route from target terrain back to start terrain.

## License
TBD