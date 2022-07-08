#pragma once
//--------------------------------------------------------------------------------------------------------
//                                                gameutils.h
//--------------------------------------------------------------------------------------------------------
// This header contains constant and structure declarations, and function prototypes, 
// for the SimpleQuest game.
//--------------------------------------------------------------------------------------------------------
#include <string>
using namespace std;

// constant declarations
const int START_ROOM = 0;	// index of the room where the game starts
const int NOT_FOUND = -1;	// return value for array search
const int MAX_ITEMS = 5;	// max # items in an inventory (array of item)
const int MAX_ROOMS = 16;	// max # rooms in the game
const int NO_EXIT = -1;	// value of room.north/south/etc, 
							// meaning "no exit in that direction"

// struct item - describes an item in a room or player inventory
struct item {
	string shortDesc;			// short description of the item ex "shoe"
	string longDesc;			// long/detailed description, ex "It is a blue shoe with ..."
};

// struct room - describes a room, its inventory, and exits
struct room {
	string shortDesc;			// short description of the room, ex "kitchen"
	string longDesc;			// long/detailed description, ex "There is a stove and sink here, ..."
	item   items[MAX_ITEMS];	// partial array of items; the inventory of this room
	int    numItems;			// partial array of items; the number of items in the inventory
	int    north, south, 		// exits: north=2 means "north" leads to room[2]
		east, west;			// north=NO_EXIT means there is no north exit in this room
};

// winDataStruct - a structure to collect data needed to win the game
struct winDataStruct {
	item winItem[MAX_ITEMS];	// partial array: items needed to win. 
	int numWinItems;			// partial array: number of items in the list
	string winMessage;			// message to print when the game is won
	int winRoom;				// index into the room[] array where the player can win
};

// prototypes of utility functions
void   gameAbort(string message);
string cleanString(string originalString);
void   cleanGetline(ifstream& f, string& data);
string split(string input, string& remainder);
void   printHelp();
void   printRooms(room rooms[], int numRooms);
void   printWinData(winDataStruct winData);
void   printItems(item items[], int numItems); 
