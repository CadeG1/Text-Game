//----------------------------------------------------------------------------
//                  CS 215 - Project 2 - MiniMud
//----------------------------------------------------------------------------
// Author:  Cade  Gallenstein
// Date:  3/16/21
// Description:  Converts a txt file to a game that is won by
// entering a win room with the specified win items.
// Assistance: I received help from the following: openstack, cplusplus.
//-----------------------------------------------------------
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include "gameutils.h"
using namespace std;


//----------------------------------------------------------------------------
//							printLogo
//----------------------------------------------------------------------------
void printLogo() {
	cout << "+---------------------------------+\n"
		 << "|    Walking and Stuff MiniMUD    |\n"
		 << "|       By: Cade Gallenstein      |\n"
		 << "+---------------------------------+\n";
}

//----------------------------------------------------------------------------
//							getInputFileName
//----------------------------------------------------------------------------
string getInputFileName() {
	string gameName;
	cout << "Enter name of game to play: ";
	cin >> gameName;
	gameName = gameName + ".txt";
	return gameName;
}

//----------------------------------------------------------------------------
//							initItems
//----------------------------------------------------------------------------
void initItems(item list[], int &numItems) {
	numItems = 0;
	for (int i = 0; i < MAX_ITEMS; i++) {
		list[i].longDesc = "";
		list[i].shortDesc = "";
	}
}

//----------------------------------------------------------------------------
//							initRooms
//----------------------------------------------------------------------------
void initRooms(room list[], int &numRooms) {
	for (int i = 0; i < MAX_ROOMS; i++) {
		list[i].longDesc = "";
		list[i].shortDesc = "";
		list[i].north = NO_EXIT;
		list[i].east = NO_EXIT;
		list[i].south = NO_EXIT;
		list[i].west = NO_EXIT;
		initItems(list[i].items, list[i].numItems);
	}
	numRooms = 0;
}

//----------------------------------------------------------------------------
//							initWinData
//----------------------------------------------------------------------------
void initWinData(winDataStruct& data) {
	data.winMessage = "";
	data.winRoom = NO_EXIT;

	initItems(data.winItem, data.numWinItems);
}

//----------------------------------------------------------------------------
//							removeItem
//----------------------------------------------------------------------------
item removeItem(int removeIndex, item list[], int &numItems) {
	item removedItem;

	if (removeIndex >= 0 && removeIndex < MAX_ITEMS - 1) {
		removedItem = list[removeIndex];
		for (int i = removeIndex; i < numItems - 1; i++) {
			if (i < MAX_ITEMS) {
				list[i] = list[i + 1];
			}
			else {
				list[MAX_ITEMS - 1].shortDesc = "";
			}
		}
		numItems = numItems - 1;
	}
	else {
		gameAbort("removeItem: invalid index = " + removeIndex);
	}

	return removedItem;
}

//----------------------------------------------------------------------------
//							addItem
//----------------------------------------------------------------------------
void addItem(item newItem, item list[], int &numItems) {
	if (numItems < MAX_ITEMS) {
		list[numItems] = newItem;
		numItems = numItems + 1;
	}
	else {
		gameAbort("addItem: maximum number of items exceeded!");
	}
}

//----------------------------------------------------------------------------
//							addItem (overloaded)
//----------------------------------------------------------------------------
void addItem(string shortDes, string longDes, item list[], int &numItems) {
	item newItem;
	newItem.shortDesc = shortDes;
	newItem.longDesc = longDes;
	addItem(newItem, list, numItems);
}

//----------------------------------------------------------------------------
//							loadData
//----------------------------------------------------------------------------
void loadData(room rooms[], int &numRooms, winDataStruct &winData) {
	// variable declarations
	ifstream f;				// input file handle
	string tag;				// first word extracted from each line
	string filename;		// name of input file
	string data;			// data read from file using cleanGetline()
	int roomNdx = -1;		// index of the "current room" being read/populated
							// this should always be rumRooms-1 

	// initialize the rooms and winData
	initRooms(rooms, numRooms);
	initWinData(winData);

	// get input file name and open the input file; abort on failure
	filename = getInputFileName();
	f.open(filename); 
	if (!f.is_open()) {
		gameAbort("Unable to open " + filename);
	}

	// read line-by-line and store data in the game data structures
	f >> tag;
	while (tag != "END:") {
		if (tag == "ROOM:") {
			numRooms++;		// starting a new room
			roomNdx++;		// increment every time numRooms is incremented
			cleanGetline(f, rooms[roomNdx].shortDesc); // short desc on remainder of ROOM: line
			cleanGetline(f, rooms[roomNdx].longDesc);  // long desc on next line by itself (no tag)
		}
		else if (tag == "ITEM:") {
			string shortDesc, longDesc;
			cleanGetline(f, shortDesc);
			cleanGetline(f, longDesc);
			addItem(shortDesc, longDesc, rooms[roomNdx].items, rooms[roomNdx].numItems);
		}
		else if (tag == "WIN_ITEM:") {
			cleanGetline(f, data);
			addItem(data, "", winData.winItem, winData.numWinItems);
		}
		else if (tag == "NORTH:")
			f >> rooms[roomNdx].north;
		else if (tag == "SOUTH:")
			f >> rooms[roomNdx].south;
		else if (tag == "EAST:")
			f >> rooms[roomNdx].east;
		else if (tag == "WEST:")
			f >> rooms[roomNdx].west;
		else if (tag == "WIN_ROOM:")
			f >> winData.winRoom;
		else if (tag == "REMARK:")
			cleanGetline(f, data);		// data is junk, throw it away
		else if (tag == "WIN_TEXT:") {
			cleanGetline(f, winData.winMessage);
		}
		else if (tag == "END:") {
			f.close();
		}
		else
			gameAbort("Unknown tag in " + filename + ": " + tag);
		

		// check for read failure and read the next tag
		if (f.fail())
			gameAbort("Failure while reading input file " + filename);
		f >> tag;
	}
} // loadData()


//----------------------------------------------------------------------------
//							getCmd
//----------------------------------------------------------------------------
string getCmd(string& remain) {
	string finalCommand;
	cout << "===> ";
	if (cin.peek() == '\n') cin.ignore();
	getline(cin, finalCommand);

	finalCommand = split(finalCommand, remain);

	return finalCommand;
}


//----------------------------------------------------------------------------
//							makeExitList ***DONE****
//----------------------------------------------------------------------------
string makeExitList(room thisRoom) {
	string exitList = "You can go:";
	if (thisRoom.north != NO_EXIT) exitList += " north,";
	if (thisRoom.south != NO_EXIT) exitList += " south,";
	if (thisRoom.east != NO_EXIT) exitList += " east,";
	if (thisRoom.west != NO_EXIT) exitList += " west.";
	exitList[exitList.length() - 1] = '.'; // change last comma to a period
	return exitList;
} // makeExitList()


//----------------------------------------------------------------------------
//							doLook
//----------------------------------------------------------------------------
void doLook(room currentRoom, bool shortdesc) {
	if (shortdesc) {
		cout << currentRoom.shortDesc << endl;
	}
	else {
		cout << currentRoom.longDesc << endl;
	}

	if (currentRoom.numItems >= 2) {
		cout << "You notice the following: ";

		for (int i = 0; i < currentRoom.numItems; i++) {
			
			if (i == currentRoom.numItems - 1) {
			cout << currentRoom.items[i].shortDesc + "." << endl;
			}
			else {
			cout << currentRoom.items[i].shortDesc + ", ";
			}
		}
	}
	else if (currentRoom.numItems == 1) {
		cout << "You notice the following: " + currentRoom.items[0].shortDesc + "." << endl;
	}

	string exitList = makeExitList(currentRoom);
		cout << exitList << endl;
}


//----------------------------------------------------------------------------
//							doLook (overloaded)
//----------------------------------------------------------------------------
void doLook(room currentRoom) {
	doLook(currentRoom, false);
}


//----------------------------------------------------------------------------
//							startUp
//----------------------------------------------------------------------------
void startUp(room rooms[], item items[], winDataStruct &win, int &numRooms, int &numItems) {

	printLogo();
	loadData(rooms, numRooms, win);
	
	initItems(items, numItems);

	cout << "You fall asleep and dream of a place far away..." << endl;

	doLook(rooms[START_ROOM]);
}



//----------------------------------------------------------------------------
//							doInventory    ***DONE***
//----------------------------------------------------------------------------
// Given: the player's inventory (partial array of items)
// This is the game's response to the "inv"/"i" (inventory) command.
// Prints a message telling the player what he/she is holding.
//----------------------------------------------------------------------------
void doInventory(item item[], int numItems) {
	if (numItems == 0)
		cout << "You don't have anything.\n";
	else {
		cout << "You are holding: ";
		for (int i = 0; i < (numItems - 1); i++)		// for all but last item
			cout << item[i].shortDesc << ", ";			// print short desc with comma
		cout << item[numItems - 1].shortDesc << ".\n";  // for last item, use period and newline
	}
} // doInventory()

//----------------------------------------------------------------------------
//							goNorth   ***DONE***
//----------------------------------------------------------------------------
// Given:    partial array of rooms
// Modifies: index of current room
// This is the game's response to the player entering the "north"/"n" command.
// When the north exit exists, changes the current room index and performs
// a "look" command with a short room description
void goNorth(int& currentRoom, room rooms[]) {
	if (rooms[currentRoom].north == NO_EXIT)
		cout << "You can't go north.\n";
	else {
		currentRoom = rooms[currentRoom].north;
		doLook(rooms[currentRoom], true); // true = show short room desc.
	}
} // goNorth()


//----------------------------------------------------------------------------
//							goEast
//----------------------------------------------------------------------------
void goEast(int& currentRoom, room rooms[]) {
	if (rooms[currentRoom].east == NO_EXIT)
		cout << "You can't go east.\n";
	else {
		currentRoom = rooms[currentRoom].east;
		doLook(rooms[currentRoom], true); // true = show short room desc.
	}
}

//----------------------------------------------------------------------------
//							goSouth
//----------------------------------------------------------------------------
void goSouth(int& currentRoom, room rooms[]) {
	if (rooms[currentRoom].south == NO_EXIT)
		cout << "You can't go south.\n";
	else {
		currentRoom = rooms[currentRoom].south;
		doLook(rooms[currentRoom], true); // true = show short room desc.
	}
}

//----------------------------------------------------------------------------
//							goWest
//----------------------------------------------------------------------------
void goWest(int& currentRoom, room rooms[]) {
	if (rooms[currentRoom].west == NO_EXIT)
		cout << "You can't go west.\n";
	else {
		currentRoom = rooms[currentRoom].west;
		doLook(rooms[currentRoom], true); // true = show short room desc.
	}
} 

//----------------------------------------------------------------------------
//							findItem
//----------------------------------------------------------------------------
int findItem(string itemName, item list[], int numItems) {
	for (int i = 0; i < numItems; i++) {
		if (list[i].shortDesc == itemName) 
				return i;	
	}
	return NOT_FOUND;
}


//----------------------------------------------------------------------------
//							doExamine
//----------------------------------------------------------------------------
void doExamine(string itemDesc, item room[], item inv[], int roomItems, int numItems) {

	int found;

	found = findItem(itemDesc, room, roomItems);
	if (found != NOT_FOUND && itemDesc != "") 
		cout << room[found].longDesc << endl;
	else if (itemDesc == "") {
		cout << "Specify an item you would like to examine." << endl;
	}
	else{
		found = findItem(itemDesc, inv, numItems);
		if (found != NOT_FOUND && itemDesc != "")
			cout << inv[found].longDesc << endl;
		else if (itemDesc == "") {
			cout << "Specify an item you would like to examine." << endl;
		}
		else 
			cout << "You see no " + itemDesc << endl;
		
	}
}


//----------------------------------------------------------------------------
//							doDrop
//----------------------------------------------------------------------------
	void doTake(string itemName, item room[], item inventory[], int &roomItems, int &invItems) {
		int foundItem = NOT_FOUND;
		item change;
		foundItem = findItem(itemName, room, roomItems);
		if (foundItem != NOT_FOUND && invItems < MAX_ITEMS && itemName != "") {
			change = removeItem(foundItem, room, roomItems);
			addItem(change, inventory, invItems);
			cout << "You take the " + change.shortDesc << endl;
		}
		else if (foundItem != NOT_FOUND && invItems >= MAX_ITEMS) {
			cout << "Your hands are full, you can't take that." << endl;
		}
		else {
			foundItem = findItem(itemName, inventory, invItems);
			if (foundItem != NOT_FOUND && itemName != "") {
				cout << "You already have the " + itemName << endl;
			}
			else if (itemName == "") {
				cout << "Specify something to pick up" << endl;
			}
			else {
				cout << "You see no " + itemName << endl;
			}

		}
	}

//----------------------------------------------------------------------------
//							doDrop
//----------------------------------------------------------------------------
	void doDrop(string itemName, item room[], item inventory[], int &roomItems, int &invItems) {
		int foundItem = NOT_FOUND;
		item change;
		foundItem = findItem(itemName, inventory, invItems);
		if (foundItem != NOT_FOUND && itemName != "" && roomItems < MAX_ITEMS) {
			change = removeItem(foundItem, inventory, invItems);
			addItem(change, room, roomItems);
			cout << "You drop the " + change.shortDesc << endl;
		}
		else if (foundItem != NOT_FOUND && roomItems >= MAX_ITEMS) {
			cout << "The room is full of junk, you can't drop that." << endl;
		}
		else {
			foundItem = findItem(itemName, room, roomItems);
			if (foundItem != NOT_FOUND && itemName != "") {
				cout << "You don't have the " + itemName << endl;
			}
			else if (itemName == "") {
				cout << "Specify something to drop" << endl;
			}
			else {
				cout << "You have no " + itemName << endl;
			}
			
		}

	}

//----------------------------------------------------------------------------
//							gameWon
//----------------------------------------------------------------------------
	bool gameWon(winDataStruct win, room rooms[]) {
		int check = NOT_FOUND;
		for (int i = 0; i < win.numWinItems; i++) {
			check = findItem(win.winItem[i].shortDesc, rooms[win.winRoom].items, rooms[win.winRoom].numItems);
				if (check == NOT_FOUND) {
					return false;
				}
			}
		return true;
		}

//----------------------------------------------------------------------------
//							main
//----------------------------------------------------------------------------
int main() {
	// declare data structures
	item inventory[MAX_ITEMS];
	room allRooms[MAX_ROOMS];
	winDataStruct winData;
	int currentRoom = START_ROOM;

	// declare other variables
	int numRooms;
	int numItems;
	string cmd;					// command entered by user (first word of input) 
	string remainder;			// remainder of command entered by user
	bool gameWasWon = false;	// check with gameWon() as needed

	// start up: load game data from file and initialize player inventory
	startUp(allRooms, inventory, winData, numRooms, numItems);

	// get first command input
	cmd = getCmd(remainder);

	// repeat until "exit" entered or the game is won
	while (cmd != "exit" && !gameWasWon) {
		if (cmd == "help")
			printHelp();
		// add cases for other commands and invocations
		else if (cmd == "inv" || cmd == "i") {
			doInventory(inventory, numItems);
		}
		else if (cmd == "look" || cmd == "l") {
			doLook(allRooms[currentRoom], false);
		}
		else if (cmd == "north" || cmd == "n") {
			goNorth(currentRoom, allRooms);
		}
		else if (cmd == "east" || cmd == "e") {
			goEast(currentRoom, allRooms);
		}
		else if (cmd == "south" || cmd == "s") {
			goSouth(currentRoom, allRooms);
		}
		else if (cmd == "west" || cmd == "w") {
			goWest(currentRoom, allRooms);
		}
		else if (cmd == "exa" || cmd == "x") {
			doExamine(remainder, allRooms[currentRoom].items, inventory, allRooms[currentRoom].numItems, numItems);
		}
		else if (cmd == "take" || cmd == "t") {
			doTake(remainder, allRooms[currentRoom].items, inventory, allRooms[currentRoom].numItems, numItems);
		}
		else if (cmd == "drop" || cmd  == "d") {
			doDrop(remainder, allRooms[currentRoom].items, inventory, allRooms[currentRoom].numItems, numItems);
		}
		else // the user entered an unrecognized command
			cout << "What??\n";

		// check if game was won and print win message or get next user input
		gameWasWon = gameWon(winData, allRooms);
		if (gameWasWon)
			cout << winData.winMessage << endl;
		else
			cmd = getCmd(remainder);
	}

	system("pause");
	return 0;
}