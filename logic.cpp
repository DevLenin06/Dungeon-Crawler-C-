#include <iostream>
#include <fstream>
#include <string>
#include "logic.h"

using std::cout, std::endl, std::ifstream, std::string;

/**
 * TODO: Student implement this function
 * Load representation of the dungeon level from file into the 2D map.
 * Calls createMap to allocate the 2D array.
 * @param   fileName    File name of dungeon level.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference to set starting position.
 * @return  pointer to 2D dynamic array representation of dungeon map with player's location., or nullptr if loading fails for any reason
 * @updates  maxRow, maxCol, player
 */
char** loadLevel(const string& fileName, int& maxRow, int& maxCol, Player& player) {
    //  cout << "Debug load level" << endl;
    //  cout << "Debug file name: " << fileName << endl;

    
    ifstream inFS;
    inFS.open(fileName);

    //if you can't open file
    if (!inFS.is_open()) {
        // cout <<"Debug can't open file: " << fileName << endl;
        return nullptr;
    }

    

    

    //first value is maxrows
    inFS >> maxRow;

    //second value is max columns
    inFS >> maxCol;

    if(maxRow > 999999 || maxCol > 999999 || maxRow <= 0 || maxCol <= 0) {
        return nullptr;
    }

    

    //if area is greater than INT32_max
    if ( maxRow > INT32_MAX/maxCol) {
        return nullptr;
    }

    //allocate new array with number maxRow characters
    char** map = new char*[maxRow];
    //double array needs double pointer
    int i;
    
    for (i = 0; i < maxRow; i++) {
        map[i] = new char[maxCol];
    }

    // read in starting points
    inFS >> player.row;
    inFS >> player.col;
    //out of bounds check
    if(player.row >= maxRow || player.col >= maxCol || player.col < 0 || player.row < 0) {
        return nullptr;
    }

    int j;
    int k;
    //go through lines 3-end
    bool doorFound = false;
    bool exitFound = false;
    for (j = 0; j < maxRow; j++) {
        for ( k = 0; k < maxCol; k++) {
            if (!(inFS >> map[j][k])) {
                return nullptr;
            }

            if (map[j][k] == TILE_DOOR) {
                doorFound = true;
            }else if (map[j][k] == TILE_EXIT) {
                exitFound = true;
            }
            
            if (map[j][k] != TILE_OPEN && map[j][k] != TILE_TREASURE && map[j][k] != TILE_AMULET && map[j][k] != TILE_MONSTER && map[j][k] != TILE_PILLAR && map[j][k] != TILE_DOOR && map[j][k] != TILE_EXIT ) {
                return nullptr;
            }

        }
            
    }

    if (doorFound == false && exitFound == false) {
        return nullptr;
    }
    
    
    
    
    //extra character
    // if (!inFS.eof()) {
    //     return nullptr;
    // }


    //extra character attempt 2
    char extra;
    if (inFS >> extra) {
        return nullptr;
    }

    

    //player's location
    if (map[player.row][player.col] != TILE_OPEN) {
        return nullptr;
    }
    map[player.row][player.col] = TILE_PLAYER;
    

    inFS.close();

    return map;


    
}

/**
 * TODO: Student implement this function
 * Translate the character direction input by the user into row or column change.
 * That is, updates the nextRow or nextCol according to the player's movement direction.
 * @param   input       Character input by the user which translates to a direction.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @updates  nextRow, nextCol
 */
void getDirection(char input, int& nextRow, int& nextCol) {

    


    if ( input == 'w') {
        //go up
        nextRow--;
    }else if (input == 's') {
        // go down
        nextRow++;
    }else if (input == 'a') {
        //go left
        nextCol--;
    }else if (input == 'd') {
        //go right
        nextCol++;
    }
}

/**
 * TODO: [suggested] Student implement this function
 * Allocate the 2D map array.
 * Initialize each cell to TILE_OPEN.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @return  2D map array for the dungeon level, holds char type.
 */
char** createMap(int maxRow, int maxCol) {

    

    if (maxRow <= 0 || maxCol <= 0) {
        return nullptr;
    }

    //allocate 2d array map
    char** map = new char*[maxRow];
    int i;
    for (i = 0; i < maxRow; i++) {
        map[i] = new char[maxCol];
    }

    //make each array initilized to tile_open
    int r;
    int c;
    for (r = 0; r < maxRow; r++) {
        for (c = 0; c < maxCol; c++) {
            map[r][c] = TILE_OPEN;
            
        }
    }


    return map;
}

/**
 * TODO: Student implement this function
 * Deallocates the 2D map array.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @return None
 * @update map, maxRow
 */
void deleteMap(char**& map, int& maxRow) {
    //deallocate 
    if (map == nullptr) {
        maxRow = 0;
    }
    int i;
    for (i = 0; i < maxRow; i++) {
        delete[] map[i];
    }

    
    maxRow = 0;
    delete[] map;

    

    map = nullptr;
    
}

/**
 * TODO: Student implement this function
 * Resize the 2D map by doubling both dimensions.
 * Copy the current map contents to the right, diagonal down, and below.
 * Do not duplicate the player, and remember to avoid memory leaks!
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height), to be doubled.
 * @param   maxCol      Number of columns in the dungeon table (aka width), to be doubled.
 * @return  pointer to a dynamically-allocated 2D array (map) that has twice as many columns and rows in size.
 * @update maxRow, maxCol
 */
char** resizeMap(char** map, int& maxRow, int& maxCol) {

    //return nullprt if the map is null or one of the dimensions is invalid
    if (map == nullptr || maxRow <= 0 || maxCol <= 0) {
        return nullptr;
    }


    int newRow = maxRow * 2;
    int newCol = maxCol * 2;

    //allocate temp new map
    char **newMap = new char*[newRow];
    int i;
    for (i = 0; i < newRow ; i++) {
        newMap[i] = new char[newCol];
    }

    

    int row;
    int col;
    //copy the current map into sub array a exactly with the adventurer
    //Sub Array A
    //goes until max row and column
    //fill exactly the same as previous map
    for (row = 0; row < maxRow; row++) {
        for (col = 0; col < maxCol; col++) {
            newMap[row][col] = map[row][col];
            
        }
    }

    //ideas for sub array: B, C, D??
    for (int row = 0; row < newRow; row++) {
        for (int col = 0; col < newCol; col ++){
            //sub array D
            if (row >= maxRow && col >= maxCol) {
                newMap[row][col] = map[row - maxRow][col - maxCol];
                if (newMap[row][col] == TILE_PLAYER) {
                    newMap[row][col] = TILE_OPEN;
                }

            }else if (row >= maxRow) {
                //sub array C
                newMap[row][col] = map[row-maxRow][col];
                if (newMap[row][col] == TILE_PLAYER) {
                    newMap[row][col] = TILE_OPEN;
                }
            }else if (col >= maxCol) {
                //sub array B
                newMap[row][col] = map[row][col - maxCol];
                if (newMap[row][col] == TILE_PLAYER) {
                    newMap[row][col] = TILE_OPEN;
                }
            }

        }
    }


    // //SubArray B is from maxRow to newRow-1??
    // //vertical reads from o to maxCol
    // for (row = maxRow ; row < newRow; row++) {
    //     for(col = 0; col < maxCol; col++) {
    //         if(map[row][col] == TILE_PLAYER) {
    //             newMap[row][col] = TILE_OPEN;
    //         }else {
    //             newMap[row][col] = map[row][col];
    //         }
    //     }
    // }

    // //SUBARRAY C
    // //reads from 0 to maxRoW horizontally
    // //reads from maxCol to newCol-1 vertically
    // for (row = 0; row < maxRow; row++) {
    //     for (col = maxCol; col < newCol; col++) {
    //         if(map[row][col] == TILE_PLAYER) {
    //             newMap[row][col] = TILE_OPEN;
    //         }else {
    //             newMap[row][col] = map[row][col];
    //         }
    //     }
    // }


    // //SUBARRAY D
    // //Reads horizontally from maxRow to newRow-1
    // //Reads vertically from maxCol to newCol -1 
    // for (row = maxRow; row < newRow; row++) {
    //     for (col = maxCol; col < newCol; col++){
    //         if(map[row][col] == TILE_PLAYER) {
    //             newMap[row][col] = TILE_OPEN;
    //         }else {
    //             newMap[row][col] = map[row][col];
    //         }
    //     }
    // }
    

    int j;
    for (j = 0; j < maxRow; j++) {
        delete[] map[j];
    }
    delete[] map;

    map = newMap;

    //update maxRow and maxCol
    maxRow = newRow;
    maxCol = newCol;

    



    

    return newMap;
}

/**
 * TODO: Student implement this function
 * Checks if the player can move in the specified direction and performs the move if so.
 * Cannot move out of bounds or onto TILE_PILLAR or TILE_MONSTER.
 * Cannot move onto TILE_EXIT without at least one treasure. 
 * If TILE_TREASURE, increment treasure by 1.
 * Remember to update the map tile that the player moves onto and return the appropriate status.
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object to by reference to see current location.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @return  Player's movement status after updating player's position.
 * @update map contents, player
 */
int doPlayerMove(char** map, int maxRow, int maxCol, Player& player, int nextRow, int nextCol) {
    //bounds
    // cout << nextRow << endl;
    // cout << nextCol << endl;
    if ( nextRow < 0|| nextRow >= maxRow || nextCol < 0|| nextCol >= maxCol){
        //return status stay if out of bounds
        nextRow = player.row;
        nextCol = player.col;
        return STATUS_STAY;
    }


    //the type of tile it goes to next
    char nextPos = map[nextRow][nextCol];

    //if TILE_TREASURE increment by one
    if (nextPos == TILE_TREASURE ) {
        //increment the adventurer's treasure
        player.treasure++;
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow; //the row attribute is now a new value
        player.col = nextCol;
        map[nextRow][nextCol] = TILE_PLAYER;
        //set appropriate status
        return STATUS_TREASURE;
    }else if (nextPos == TILE_AMULET){
        //set appropriate status
        // cout << "reached amulet status" << endl;
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow;
        player.col = nextCol;
        map[nextRow][nextCol] = TILE_PLAYER;
        return STATUS_AMULET;
    }else if (nextPos == TILE_DOOR ) {
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow;
        player.col = nextCol;
        map[nextRow][nextCol] = TILE_PLAYER;
        return STATUS_LEAVE;
    }else if(nextPos == TILE_EXIT && player.treasure >=1) {
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow;
        player.col = nextCol;
        map[nextRow][nextCol] = TILE_PLAYER;
        return STATUS_ESCAPE;
    }else if (nextPos == TILE_EXIT && player.treasure < 1) {
        //recent change
        nextRow = player.row;
        nextCol = player.col;
        return STATUS_STAY;
    }else if (nextPos == TILE_PILLAR || nextPos == TILE_MONSTER) {
        nextRow = player.row;
        nextCol = player.col;
        return STATUS_STAY;
    }
    else {
        //update the map; set adventurer's position to next position
    //"Debug"first make the old position into an open tile
    //position on the map not that actual tile itself
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow; //the row attribute is now a new value
        player.col = nextCol; //the column attribute is now a new value
        map[nextRow][nextCol] = TILE_PLAYER;
        return STATUS_MOVE;
    }

    
    
    return 0;
}

/**
 * TODO: Student implement this function
 * Update monster locations:
 * We check up, down, left, right from the current player position.
 * If we see an obstacle, there is no line of sight in that direction, and the monster does not move.
 * If we see a monster before an obstacle, the monster moves one tile toward the player.
 * We should update the map as the monster moves.
 * At the end, we check if a monster has moved onto the player's tile.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference for current location.
 * @return  Boolean value indicating player status: true if monster reaches the player, false if not.
 * @update map contents
 */
bool doMonsterAttack(char** map, int maxRow, int maxCol, const Player& player) {
    //rows are in decreasing format on map, so one row above the player the row - 1
    //when you move left and right you are moving columns

    //represents what row and column the player is at
    int Playrow = player.row;
    int Playcol = player.col;
    //row at current position
    int row;

    bool playerCaught = false;

    //up direction, when you keep decreasing the row number, you keep going up(think about maps)
    for (row = Playrow - 1; row >= 0; row--) {
        //what is the tile at that row, the column does not change
        char tile = map[row][Playcol];


        //if there is a monster then move it one step closer
        if(tile == TILE_MONSTER) {
            //make that tile empty and make the tile that is one step closer the monster tile
            //moved closer from above means the row gets bigger
            if (map[row + 1][Playcol] != TILE_PILLAR) {
                map[row][Playcol] = TILE_OPEN;
                map[row + 1][Playcol] = TILE_MONSTER;
            }
            
            

            //check if it is the players tile(the row column for up)
            if (row + 1 == Playrow) {
                //the player is caught
                playerCaught = true;
            }
        //continue UNTIL pillar, so stop when it is pillar?
        }else if (tile == TILE_PILLAR) {
            //can't see monster
            break;
        }
    }


    //down direction
    for (row = Playrow  + 1; row < maxRow; row++){
        char tile = map[row][Playcol];

        if (tile == TILE_MONSTER) {
            if (map[row-1][Playcol] != TILE_PILLAR) {
                map[row][Playcol] = TILE_OPEN;
            //the tile above, so minus is one step closer
                map[row - 1][Playcol] = TILE_MONSTER;
            }
            

            if (row - 1 == Playrow) {
                playerCaught = true;
            }
        }else if (tile == TILE_PILLAR) {
            break;
        }
    }


    //left direction
    int col;

    for (col = Playcol - 1; col >= 0; col --) {
        char tile = map[Playrow][col];


        if(tile == TILE_MONSTER) {
            if (map[Playrow][col+1] != TILE_PILLAR) {
                map[Playrow][col] = TILE_OPEN;
                map[Playrow][col +1] = TILE_MONSTER;
            }
            

            if (col + 1 == Playcol) {
                playerCaught = true;
            }
        }else if (tile == TILE_PILLAR) {
            break;
        }

    }


    //right direction

    for (col = Playcol + 1; col < maxCol; col++){
        char tile = map[Playrow][col];

        if(tile == TILE_MONSTER) {
            if (map[Playrow][col - 1] != TILE_PILLAR) {
                //column needs to decrease
                map[Playrow][col] = TILE_OPEN;
             map[Playrow][col -1] = TILE_MONSTER;
            }
            

            if(col-1 == Playcol) {
                playerCaught =  true;
            }
        }else if (tile == TILE_PILLAR) {
            break;
        }
    }


    return playerCaught;
}
