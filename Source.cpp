#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <ctime>

using namespace std;

#define clearConsole system("cls");

// Constants
// Array sizes
const int ID_SIZE = 7, FNAME_SIZE = 26, LNAME_SIZE = 26, DIGITS = 11,
ROWS = 10, COLS = 16;

struct SeatInfo
{
	int row, col, price;
	bool sold;
	char IDS[ID_SIZE]; // change to char IDS[ID_SIZE];
};
struct PatronInfo { // Nick
	char id[ID_SIZE];
	char firstName[FNAME_SIZE];
	char lastName[LNAME_SIZE];
	char phoneNum[DIGITS];
};

// Function Prototypes
// Jeel's
void showMenu();
void errorCheckin(char);
void showSeatingchar();
void SetColor(int);

// Nick's
bool checkFile(fstream &, string);
void saveSeatInfo(SeatInfo seats[ROWS][COLS], fstream &);
void getSeatInfo(SeatInfo seats[ROWS][COLS], fstream &);
void savePatronInfo(PatronInfo currPatronInfo[ROWS][COLS], fstream &);
void getPatronInfo(PatronInfo currPatronInfo[ROWS][COLS], fstream&);
void emptySeatInfo(SeatInfo seats[ROWS][COLS], PatronInfo currPatronInfo[ROWS][COLS]);
void resetCharArray(char[], int);
void generateID(int row, int col, SeatInfo seats[ROWS][COLS], PatronInfo currPatronInfo[ROWS][COLS]);
char menuChoiceValidate();
bool validateName(string name, int row, int col, SeatInfo seats[ROWS][COLS]);
bool menuChoiceValidate(string);
// jeel's
void searchPatronInfo(SeatInfo seats[ROWS][COLS], PatronInfo currPatronInfo[ROWS][COLS]);
void showPatronInfo(char[], PatronInfo currPatronInfo[ROWS][COLS]);

// Reign's
void initSeat(SeatInfo tempseats[ROWS][COLS]);
void sellSeat(SeatInfo seatstemp[ROWS][COLS], PatronInfo currPatronInfo[ROWS][COLS]);
void getNumbers(int &thedata, string message, int lowerbound, int upperbound);
void menuChoice(char &choice);
void updateSeatChart(SeatInfo seats[ROWS][COLS]);
void sellBlock(SeatInfo seats[ROWS][COLS], PatronInfo currPatronInfo[ROWS][COLS]);

static char SChart[ROWS][COLS];

int main()
{
	// file objects
	fstream seatFile;
	fstream patronFile;
	// 2-d arrays
	SeatInfo seats[ROWS][COLS];
	PatronInfo currPatronInfo[ROWS][COLS];

	char choice = NULL;

	initSeat(seats);
	emptySeatInfo(seats, currPatronInfo);
	// read files for data
	// load data into seats and patron array
	getSeatInfo(seats, seatFile);
	getPatronInfo(currPatronInfo, patronFile);


	while (choice != 'H')
	{
		choice = 'X';
		updateSeatChart(seats);
		showSeatingchar(); // show  current seat chart
		showMenu();
		menuChoice(choice);
		choice = toupper(choice);
		switch (choice)
		{
		case 'A':
			sellSeat(seats, currPatronInfo);
			break;
		case 'B':
			sellBlock(seats, currPatronInfo);
			break;
		case 'C':
			// call patron info search
			searchPatronInfo(seats, currPatronInfo);
			break;
		case 'D':
			//total revenue/seats remaining
			break;
		case 'E':
			//reset a seats info
			break;
		case 'F':
			emptySeatInfo(seats, currPatronInfo);
			break;
		case 'G':
			//call credits
			break;
		}
		system("CLS");
	}

	saveSeatInfo(seats, seatFile);
	savePatronInfo(currPatronInfo, patronFile);

	return 0;
}

/*
function to initialize the seats array for prices and col/row numbers
*/
void initSeat(SeatInfo tempseats[10][16])
{
	for (int count = 0; count<10; count++)
	{
		for (int i = 0; i<16; i++)
		{
			if (count <= 3)
				tempseats[count][i].price = 50;
			if (count >= 4 && count <= 7)
				tempseats[count][i].price = 35;
			if (count >= 8)
				tempseats[count][i].price = 25;
			tempseats[count][i].col = i + 1;
			tempseats[count][i].row = count + 1;
			tempseats[count][i].sold = false;
			resetCharArray(tempseats[count][i].IDS, ID_SIZE);
		}
	}
}

/*
Function designed to return an integer between an upper bound and lower bound
Good for getting and validating input data
*/
void getNumbers(int &thedata, string message, int lowerbound, int upperbound)
{
	cin.clear();
	fflush(stdin);
	char test[999];
	while (thedata>upperbound || thedata<lowerbound)
	{
		cout << message;
		cin.getline(test, INT_MAX);
		thedata = atoi(test);
		cin.clear();
		fflush(stdin);
	}
}

/*
function to sell a single seat to the user
*/
void sellSeat(SeatInfo seatstemp[ROWS][COLS], PatronInfo currPatronInfo[ROWS][COLS])
{
	int row = -1, column = -1;
	getNumbers(row, "Enter the row for the seat the patron is buying.\n\n", 1, 10);
	//row = row - 1;
	getNumbers(column, "Enter the column for the seat that the patron is buying.\n\n", 1, 16);
	column = column - 1;
	cout << "Enter first name\n\n";
	row = 10 - row; // new assignment
	cin.getline(currPatronInfo[row][column].firstName, FNAME_SIZE);
	// validate name
	//
	cout << "Last name\n";
	cin.getline(currPatronInfo[row][column].lastName, LNAME_SIZE);
	// validate name
	//
	cout << "Phone # in format nnnnnnnnnn\n\n";
	// validate phone number
	cin.getline(currPatronInfo[row][column].phoneNum, DIGITS);
	seatstemp[row][column].sold = true;
	// generate ID for patron
	generateID(row, column, seatstemp, currPatronInfo);

	// show generated id
	cout << currPatronInfo[row][column].id << endl;
	cout << seatstemp[row][column].IDS << endl;
	system("pause");
}

/*
Updates the seating chart
based on the values in seats[][].sold
*/
void updateSeatChart(SeatInfo seats[ROWS][COLS])
{
	for (int i = 0; i<ROWS; i++)
	{
		for (int j = 0; j<COLS; j++)
		{
			if (seats[i][j].sold == true)
				SChart[i][j] = 'X';
			else
				SChart[i][j] = 'O';
		}
	}
}

/*
Function to sell blocks of seats at once
*/

void sellBlock(SeatInfo seats[ROWS][COLS], PatronInfo currPatronInfo[ROWS][COLS])
{
	cout << "One patron's information will be used for every seat in a group sale.\n";

	int row = -1, colstart = -1, colend = -1, tempcol = -1;
	char choice = 'M', tempFirst[FNAME_SIZE], tempLast[LNAME_SIZE], tempNum[DIGITS], tempID[ID_SIZE];
	bool fail = false;

	do
	{
	    row = -1, colstart = -1, colend = -1;
		fail = false;
		getNumbers(row, "Enter the row for the first seat the patron is buying.\n", 1, 10);
		row = 10 - row;
		getNumbers(colstart, "Enter the column for the first seat that the patron is buying.\n", 1, 16);
		colstart = colstart - 1;
		getNumbers(colend, "Enter the column for the last seat that the patron is buying.\n", 1, 16);
		colend = colend - 1;
		if (colstart>colend)
		{
			tempcol = colend;
			colend = colstart;
			colstart = tempcol;
		}
		for (int i = colstart; i<=colend; i++)
		{
			if (seats[row][i].sold == true)
			{
				fail = true;
				cout << "One or more of those seats is taken.\n Please make another selection.\n";
			}
		}
		if (fail == false && colstart - colend<-7)
		{
			cout << "This range of seats crosses the aisle, are you sure you want to sell these seats?\nEnter A or a to confirm selling the seats.\n";
			cin.clear();
			fflush(stdin);
			cin >> choice;
			choice = menuChoiceValidate();
			if (choice == 'a' || choice == 'A')
				break;
			else
				fail = true;
		}
	} while (fail == true);

	cout << "Enter patron's first name\n";
	cin >> tempFirst;
	cout << "Enter patron's last name\n";
	cin >> tempLast;
	cout << "Enter patron's phone number in format nnnnnnnnnn\n";
	cin >> tempNum;

    // generate ID for patron
	generateID(row, colstart, seats, currPatronInfo);

	// show generated id
	cout << currPatronInfo[row][colstart].id << endl;
	cout << seats[row][colstart].IDS << endl;
	system("pause");

	for (int i = colstart; i<=colend; i++)
	{
	    strcpy(currPatronInfo[row][i].id, currPatronInfo[row][colstart].id);
	    strcpy(seats[row][i].IDS, seats[row][colstart].IDS);
		strcpy(currPatronInfo[row][i].firstName, tempFirst);
		strcpy(currPatronInfo[row][i].lastName, tempLast);
		strcpy(currPatronInfo[row][i].phoneNum, tempNum);
		seats[row][i].sold = true;
	}
}


//jeel's code

void showSeatingchar()
{
	const char TAKEN = 'X';//seats taken
	const char EMPTY = 'O';//seats free
	const int row = 10;
	const int col = 16;
	SetColor(14);
	cout << endl << endl << endl << endl;
	cout << setw(48) << "HUNTINGTON PLAYHOUSE" << endl;
	cout << setw(48) << "SEATING CHART\n\n\n";
	cout << setw(42) << "Rear\n\n\n";
	cout << setw(10) << "            Seats " << endl;
	cout << setw(10) << "Row" << setw(2) << " ";
	for (int k = 1; k <= col; k++)
	{
		cout << setw(1) << k << " ";
		if (k<9)
		{
			cout << " ";
		}
		if (k == 8)
		{
			cout << "     ";
		}
	}
	cout << endl;
	cout << setw(40) << "Aisle" << endl;
	for (int i = 0; i<10; i++)
	{
		cout << endl;
		cout << setw(10) << 10 - i << setw(1) << "  ";
		if (i<10)
		{
			cout << "";
		}
		for (int j = 0; j<col; j++)
		{
			//   cout<<setw(1)<< EMPTY<<" ";
			cout << setw(1) << SChart[i][j] << " ";
			if (j<9)
			{
				cout << " ";
			}
			if (j>8)
			{
				cout << " ";
			}
			if (j == 7)
			{
				cout << "     ";
			}
		}
	}
	cout << endl;
	cout << endl;
	cout << setw(40) << "Front" << endl;
	string line1, line2;
	line1 = line1.assign(49, '-');
	line2 = line2.assign(30, '-');
	cout << endl;
	cout << setw(13) << "+" << line1 << "+" << endl;
	cout << setw(13) << "|" << setw(50) << "|" << endl;
	cout << setw(13) << "|" << setw(27) << "STAGE" << setw(23) << "|" << endl;
	cout << setw(13) << "|" << setw(50) << "|" << endl;
	cout << setw(13) << "+" << line1 << "+" << endl;
	{
		SetColor(13);
		cout << "+" << line2 << "+" << endl;
		cout << "|" << "Row 1-4 Cost $50.00 Each" << setw(7) << "|" << endl;
		cout << "|" << "Row 5-8 Cost $35.00 Each" << setw(7) << "|" << endl;
		cout << "|" << "Row 9-10 Cost $25.00 Each" << setw(6) << "|" << endl;
		cout << "+" << line2 << "+" << endl;
	}

}

void SetColor(int value){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), value);
}

/*
function to get user choice at main menu
*/
void menuChoice(char &choice)
{
	cin.clear();
	fflush(stdin);
	// prompt for menu choice
	cout << setw(7) << " " << "Enter an option A - F: ";
	choice = menuChoiceValidate();
	if (choice == 'h' || choice == 'H')
	{
		cout << "Are you sure that you want to exit the program?\nIf you are sure enter a or A\n";
		cin.clear();
		fflush(stdin);
		cin >> choice;
		choice = menuChoiceValidate();
		if (choice == 'a' || choice == 'A')
			choice = 'H';
		else
			choice = 'X';
	}
	if (choice == 'f' || choice == 'F')
	{
		cout << "Are you sure you want to delete all ticket and patron information?\nThis cannot be undone.\nEnter a or A if you are sure.";
		cin.clear();
		fflush(stdin);
		cin >> choice;
		choice = menuChoiceValidate();
		if (choice == 'a' || choice == 'A')
			choice = 'F';
		else
			choice = 'X';
	}
}

void showMenu()
{
	cout << " A) Sell a ticket";
	cout << setw(45) << " E) Refund a ticket" << endl;
	cout << " B) Sell a group of tickets";
	cout << setw(42) << " F) Delete all information" << endl;
	cout << " C) Search patron information";
	cout << setw(37) << " G) Credits for program" << endl;
	cout << " D) Total revenue and seats remaining";
	cout << setw(22) << " H) Quit Program" << endl;
}


void errorCheckin(char choice)
{
	while (choice <'A' || choice>'F'&choice<'a' || choice>'f')
	{
		cout << "Opps Enter Wrong Choice" << endl;
		cout << "Please Enter Correct choice" << endl;
		cin >> choice;
		cin.clear();
		cin.ignore();
	}
}


//*********************************************************
//    Definition of Function saveSeatFile                 *
//       This function will write each struct element     *
//       to binary file seatsInfoFile.dat                 *
//*********************************************************

void saveSeatInfo(SeatInfo seats[ROWS][COLS], fstream & seatFile) {
	string fileName = "seatsInfoFile.dat";
	seatFile.open(fileName.c_str(), ios::out | ios::binary);

	// write each stuct element to binary file
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			seatFile.write(reinterpret_cast<char *>(&seats[i][j]), sizeof(seats[i][j]));
		}
	}
	//close file
	seatFile.close();

}

//*********************************************************
//   Definition of function getSeatingInfo, this function *
//      will read a binary file containing records of     *
//      seating information and store them into an array  *
//      of structures.                                    *
//*********************************************************

// search and store patron info from file into array
void getSeatInfo(SeatInfo seats[ROWS][COLS], fstream &seatFile) {
	// open file for reading
	string fileName = "seatsInfoFile.dat";
	seatFile.open(fileName.c_str(), ios::in | ios::binary);
	// read each records and store into seat array
	while (seatFile.good()) {
		for (int i = 0; i < ROWS; i++) {
			for (int j = 0; j < COLS; j++) {
				seatFile.read(reinterpret_cast<char *>(&seats[i][j]),
					sizeof(seats[i][j]));
			}
		}
	}
	//close file
	seatFile.close();

}

//*********************************************************
//    Definition of Function savePatrongFile              *
//       This function will write each struct element     *
//       to binary file PatronInfo.dat                    *
//*********************************************************

void savePatronInfo(PatronInfo currPatronInfo[ROWS][COLS], fstream & patronFile) {
	// open file to write to
	string fileName = "PatronInfo.dat";
	patronFile.open(fileName.c_str(), ios::out | ios::binary);

	// write each stuct element to binary file
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			patronFile.write(reinterpret_cast<char *>(&currPatronInfo[i][j]), sizeof(currPatronInfo[i][j]));
		}
	}
	//close file
	patronFile.close();

}

//*********************************************************
//   Definition of function getPatronInfo, this function  *
//     will read a binary file containing records of      *
//     Patron information and store them into an array of *
//     structures.                                        *
//*********************************************************

// search and store patron info from file into array
void getPatronInfo(PatronInfo currPatronInfo[ROWS][COLS], fstream& patronFile) {
	// open Patron file
	string fileName = "PatronInfo.dat";
	patronFile.open(fileName.c_str(), ios::in | ios::binary);

	// read in each struct record
	while (patronFile.good()) {
		for (int i = 0; i < ROWS; i++) {
			for (int j = 0; j < COLS; j++) {
				patronFile.read(reinterpret_cast<char *>(&currPatronInfo[i][j]), sizeof(currPatronInfo[i][j]));
			}
		}
	}
	// close file
	patronFile.close();
}

//**********************************************************
//     Definition of Function emptySeatInfo                *
//        This function will reset the information of the  *
//        seats array and curPatronInfo array. This Func-  *
//        tion will also call the resetCharArray to assign *
//        x to each element in a char array.               *
//**********************************************************

void emptySeatInfo(SeatInfo seats[ROWS][COLS], PatronInfo currPatronInfo[ROWS][COLS]) {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			// empty XO char array
			SChart[i][j] = 'O';
			// reset info in seats array
			seats[i][j].sold = 0;
			resetCharArray(seats[i][j].IDS, ID_SIZE);
			// reset info in patrons array
			resetCharArray(currPatronInfo[i][j].id, ID_SIZE);
			resetCharArray(currPatronInfo[i][j].firstName, FNAME_SIZE);
			resetCharArray(currPatronInfo[i][j].lastName, LNAME_SIZE);
			resetCharArray(currPatronInfo[i][j].phoneNum, DIGITS);
		}
	}
}

//**********************************************************
//     Definition of Function resetCharArray               *
//        This function accepts a char array and integer   *
//        parameter. The function will assign x to each    *
//        element in the array.                            *
//**********************************************************

void resetCharArray(char cString[], int SIZE) {
	for (int index = 0; index < SIZE; index++)
		cString[index] = 'x';
}


//************************************************************
//     Definition of Function generateID                     *
//         This function generates the id for a new patron   *
//         by getting the first letter of the patrons first  *
//         and last name and concatenating them with a       *
//         randomly generated number.                        *
//************************************************************

void generateID(int row, int col, SeatInfo seats[ROWS][COLS], PatronInfo currPatronInfo[ROWS][COLS]) {
	unsigned seed = time(0);
	string tempID = "";
	int randNum = 0;
	// get first two letters of ID
	tempID += currPatronInfo[row][col].firstName[0];
	tempID += currPatronInfo[row][col].lastName[0];
	// generate numbers for ID
	srand(seed);
	randNum = (rand() % 9999 - 1000 + 1) + 1000;
	// concatnate number and letters
	ostringstream convert;
	convert << randNum;
	tempID += convert.str();
	// store ID in seat array and patron array
	for (int i = 0; i < ID_SIZE; i++) {
		seats[row][col].IDS[i] = tempID[i];
		currPatronInfo[row][col].id[i] = tempID[i];
	}
}

//***********************************************
//
//************************************************

char menuChoiceValidate() {
	bool error = 1;
	char choice;
	try {
		cin >> choice;
		if (cin.fail()) {
			throw error;
		}
		else if (!((choice >= 65 && choice <= 72) ||
			(choice >= 97 && choice <= 104))) {
			throw error;
		}
		else return choice;
	}
	catch (bool error) {
		bool flag = true;
		while (flag) {
			cin.clear();
			//			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			clearConsole
				cout << endl << endl << endl;
			cout << setw(7) << " " << "Error: Invalid Input" << endl;
			cout << setw(7) << " " << "Enter a choice A - F" << endl;
			cout << setw(7) << " ";
			cin >> choice;
			if ((choice >= 65 && choice <= 72) ||
				(choice >= 97 && choice <= 104)) {
				flag = false;
			}
		}
	}
	return choice;
}

//************************************************************
//
//************************************************************

bool validateFName(string name, int row, int col, PatronInfo currPatronInfo[ROWS][COLS]) {
	bool flag = false;
	bool error = true;

	try {
		// check if each character is a letter
		for (int i = 0; i < FNAME_SIZE; i++) {
			if (!(isalpha(currPatronInfo[row][col].firstName[i])))
				throw error;
		}
		// check if name length is longer than firstName length
		if (name.length() > FNAME_SIZE)
			throw error;
	}
	catch (bool error) {
		cout << setw(7) << " " << "ERROR: Invalid Input." << endl;
		cout << setw(7) << " " << "Name must contain letters only, and can only be 25 characters long." << endl;
		return flag;
	}

	// strncpy(currPatronInfo[row][col].firstName, name, FNAME_SIZE);
	return flag = false;
}

//************************************************************
//
//************************************************************

bool validateMenuChoice(string) {
	bool flag = false;

	return flag;
}

//************************************************************
//     Definition of function searchPatronInfo               *
//                                                           *
//      This fucntion allows the user to search for patron   *
//       information by entering the patrons seat.           *
//************************************************************

void searchPatronInfo(SeatInfo seats[ROWS][COLS], PatronInfo currPatronInfo[ROWS][COLS]) {
	// variables
	int row, col;
	char tempID[ID_SIZE];
	char emptyID[ID_SIZE] = "xxxxxx";

	clearConsole;
	cout << endl << endl << endl;
	// prompt for seat row;
	cout << setw(7) << " " << "What deos the patron sit?" << endl;
	cout << setw(7) << " " << "Enter the seat row: ";
	cin >> row;
	// adjust row for array
	row = 10 - row;
	cout << endl << endl << endl;
	// prompt for seat number/column
	cout << setw(7) << " " << "Enter the seat number(column): ";
	cin >> col;
	// adjust col for array
	col = col - 1;

	// search seats arrray for id by using row and col numbers
	cout << seats[row][col].IDS << endl;
	system("pause");
	// determine if id is not xxxxxx
	if (strcmp(seats[row][col].IDS, emptyID) == 0) {
		cout << setw(7) << " " << "Seat is currently empty. No patron information." << endl;
	}
	// if not empty call showPatronInfo function
	else {
		strncpy(tempID, seats[row][col].IDS, ID_SIZE - 1);
		cout << tempID << endl;

		system("pause");

		showPatronInfo(tempID, currPatronInfo);
	}
}

//***************************************************************
//    Definition of showPatronInfo                              *
//                                                              *
//        This function will run through each element of        *
//        the patron array and compare the patron id to the     *
//        searched id. If the id is the patron array, the       *
//        the contents will be displayed to the user.           *
//***************************************************************

void showPatronInfo(char tempID[], PatronInfo currPatronInfo[ROWS][COLS]) {

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			// compare seat temp ID with id in patron array
			if (strcmp(tempID, currPatronInfo[i][j].id) == 0) {
				clearConsole;
				cout << "\n\n\n";
				cout << setw(5) << " " << "Seat Row: " << (ROWS - i) << endl;
				cout << setw(5) << " " << "Seat Number/Column : " << (j + 1) << endl << endl;
				cout << setw(15) << " " << "Patron Information" << endl;
				cout << setw(7) << " " << "ID:         " << currPatronInfo[i][j].id << endl;
				cout << setw(7) << " " << "First Name: " << currPatronInfo[i][j].firstName << endl;
				cout << setw(7) << " " << "Last Name:  " << currPatronInfo[i][j].lastName << endl;
				cout << setw(7) << " " << "Phone:      " << currPatronInfo[i][j].phoneNum << endl;
				system("pause");
			}
		}
	}
}
