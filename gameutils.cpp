//--------------------------------------------------------------------------------------
//                                    gameutils.cpp
//--------------------------------------------------------------------------------------
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <string>
#include "gameutils.h"
using namespace std;

//--------------------------------------------------------------------------------------
//                                      gameAbort()
//--------------------------------------------------------------------------------------
// Given: a message to print before aborting the game
// Use this function any time the game needs to be aborted 
//--------------------------------------------------------------------------------------
void gameAbort(string message) {
	cout << message << endl;
	system("pause");
	exit(0);
} // gameAbort()

//--------------------------------------------------------------------------------------
//                                      cleanString()
//--------------------------------------------------------------------------------------
// Given:   a string from the user or an input file
// Returns: the given string with leading/trailing white space removed, and
//          all internal white space reduced to one space.
// Example: Given:   "     this      is     a    test    " 
//          Returns: "this is a test"
//--------------------------------------------------------------------------------------
string cleanString(string originalString) {
	stringstream ss = stringstream(originalString); // convert given string to stringstream
	string cleaned = "", word;
	ss >> cleaned;                        // extract first word
	ss >> word;                           // extract second word, if any
	while (!ss.fail()) {                  // while another word was extracted
		cleaned = cleaned + " " + word;   // append word to cleaned word with 1 space
		ss >> word;                       // try to extract another word
	}
	return cleaned;
} // cleanString()

//--------------------------------------------------------------------------------------
//                                      cleanGetline()
//--------------------------------------------------------------------------------------
// Given:    an already-opened ifstream  
// Modifies: the cleaned string read
// Reads data from the input stream using standard getline(), ignoring any leading
// newline. Cleans the read string.
//--------------------------------------------------------------------------------------
void cleanGetline(ifstream& f, string& data) {
	string rawData;
	if (f.peek() == '\n') f.ignore();
	getline(f, rawData);
	data = cleanString(rawData);
} // cleanGetline()

//--------------------------------------------------------------------------------------
//                                       split()
//--------------------------------------------------------------------------------------
// Given:    a string from the user or an input file
// Returns:  the first word of the given string, cleaned
// Modifies: the remaining words of the given string, cleaned
// Example:  Given:    "   take   red    shoe";  
//           Returns:  "take" with remainder="red shoe"
//--------------------------------------------------------------------------------------
string split(string input, string& remainder) {
	string cleaned = cleanString(input);	// clean the entire string
	int spaceIndex = cleaned.find(" ");		// find the first blank, if any
	remainder = "";							// initialize remainder to empty
	if (spaceIndex >= 0) {					// if there is a space in the cleaned string
		remainder = cleaned.substr(spaceIndex + 1); // get all chars after the first space
		cleaned = cleaned.substr(0, spaceIndex);    // remove first space
	}
	return cleaned;
} // split()

//--------------------------------------------------------------------------------------
//                                       printHelp()
//--------------------------------------------------------------------------------------
void printHelp() {
	cout << "Commands: [shortcuts in brackets]\n";
	cout << "- look [l]: get a detailed description of your current location\n";
	cout << "- inv  [i]: get a list of items in your inventory\n";
	cout << "- exa <item> [x] : (examine)get a detailed description of the item in your\n";
	cout << "current location or in your inventory.\n";
	cout << "- take <item> [t]: take an item from your location and put it in your inventory\n";
	cout << "- drop <item> [d]: take an item from your inventory and put it in your location\n";
	cout << "- north, south, east, west [n,s,e,w]: walk in that direction, if you can\n";
	cout << "\n";
	cout << "To win the game : explore the world; discover your quest; find and take the\n";
	cout << "quest items; drop them all in the 'win room'\n";
	cout << "\n";
} // printHelp()

//--------------------------------------------------------------------------------------
//                                       printItems()
//--------------------------------------------------------------------------------------
void printItems(item items[], int numItems) {
	cout << "Num Items:  " << numItems << endl;
	if (numItems < 0 || numItems > MAX_ITEMS)
		numItems = MAX_ITEMS;
	for (int i = 0; i < numItems; i++) {
		cout << "   item[" << setw(2) << i << "] short='"
			<< items[i].shortDesc << "'\n            long='"
			<< items[i].longDesc << "'\n";
	}
} // printItems()

//--------------------------------------------------------------------------------------
//                                       printWinData
//--------------------------------------------------------------------------------------
void printWinData(winDataStruct winData) {
	cout << "WIN DATA: -----------------------------\n";
	cout << "WinRoom:     " << winData.winRoom << endl;
	cout << "WinMessage:  " << winData.winMessage << endl;
	cout << "numWinItems: " << winData.numWinItems << endl;
	printItems(winData.winItem, winData.numWinItems);
	system("pause");
	cout << endl;
} // printWinData()

//--------------------------------------------------------------------------------------
//                                       printRooms()
//--------------------------------------------------------------------------------------
// given: a partial array of room objects
// Prints the contents of the partial array. Used for debugging purposes only
// [suggestion: after completing readFile(), use this to test the results]
//--------------------------------------------------------------------------------------
void printRooms(room rooms[], int numRooms) {
	cout << "PRINT ROOMS: #rooms = " << numRooms << endl;
	if (numRooms < 0 || numRooms > MAX_ROOMS)
		numRooms = MAX_ROOMS;
	for (int i = 0; i < numRooms; i++) {
		cout << "ROOM[" << i << "]  --------------\n";
		cout << "Short:      " << rooms[i].shortDesc << endl;
		cout << "Long:       " << rooms[i].longDesc << endl;
		cout << "  North=" << rooms[i].north
			<< "  South=" << rooms[i].south
			<< "  East=" << rooms[i].east
			<< "  West=" << rooms[i].west << endl;
		printItems(rooms[i].items, rooms[i].numItems);
		cout << endl;
		system("pause");
	}
} // printRooms()