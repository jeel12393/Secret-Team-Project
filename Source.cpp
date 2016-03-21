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
void seatAndRevenu(SeatInfo seats[ROWS][COLS]);

// Nick's
void saveSeatInfo(SeatInfo seats[ROWS][COLS], fstream &);
void getSeatInfo(SeatInfo seats[ROWS][COLS], fstream &);
void savePatronInfo(PatronInfo currPatronInfo[ROWS][COLS], fstream &);
void getPatronInfo(PatronInfo currPatronInfo[ROWS][COLS], fstream&);
void emptySeatInfo(SeatInfo seats[ROWS][COLS], PatronInfo currPatronInfo[ROWS][COLS]);
void resetCharArray(char[], int);
void generateID(int row, int col, SeatInfo seats[ROWS][COLS], PatronInfo currPatronInfo[ROWS][COLS]);
void searchPatronInfo(SeatInfo seats[ROWS][COLS], PatronInfo currPatronInfo[ROWS][COLS]);
void showPatronInfo(char[], PatronInfo currPatronInfo[ROWS][COLS]);
// input validation
bool validateName(string name);
void copyTempToFirstName(string, PatronInfo currPatronInfo[ROWS][COLS], int, int);
void copyTempToLastName(string temp, PatronInfo currPatronInfo[ROWS][COLS], int row, int col);
bool menuChoiceValidate(string input);
bool validate_Y_input(string input);
bool validatePhoneNum(string input);
bool copyTempToPhoneNum(string, PatronInfo currPatronInfo[ROWS][COLS], int, int);

// Reign's
void initSeat(SeatInfo tempseats[ROWS][COLS]);
void sellSeat(SeatInfo seatstemp[ROWS][COLS], PatronInfo currPatronInfo[ROWS][COLS]);
void getNumbers(int &thedata, string message, int lowerbound, int upperbound);
void menuChoice(char &choice);
void updateSeatChart(SeatInfo seats[ROWS][COLS]);
void sellBlock(SeatInfo seats[ROWS][COLS], PatronInfo currPatronInfo[ROWS][COLS]);
void refundSeat(SeatInfo seats[ROWS][COLS], PatronInfo currPatronInfo[ROWS][COLS]);

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
			seatAndRevenu(seats);
			break;
		case 'E':
			refundSeat(seats, currPatronInfo);
			break;
		case 'F':
			emptySeatInfo(seats, currPatronInfo);
			break;
		case 'G':
			//call credits
			break;
		}
		clearConsole;
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
	for (int count = 0; count<ROWS; count++)
	{
		for (int i = 0; i<COLS; i++)
		{
			if (count <= 1)
				tempseats[count][i].price = 25;
			if (count >= 2 && count <= 5)
				tempseats[count][i].price = 35;
			if (count >= 6)
				tempseats[count][i].price = 50;
			tempseats[count][i].col = i + 1;
			tempseats[count][i].row = 10 - count;
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
		cout << setw(7) << " " << message << setw(7) << " ";
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
    string nameTemp = "";
    bool flag = true;

	int row = -1, column = -1;
	getNumbers(row, "Enter the row for the seat the patron is buying.\n\n", 1, 10);
	getNumbers(column, "Enter the column for the seat that the patron is buying.\n\n", 1, 16);
	column = column - 1;
	row = 10 - row; // new assignment
	// prompt for and validate first name
	while (flag) {
        cout << "Enter first name: ";
        cin >> nameTemp;
        flag = validateName(nameTemp);
	}
	// copy contents of name to patron array stuct member firstName
	copyTempToFirstName(nameTemp, currPatronInfo, row, column);

	// prompt for and validate last name
	flag = true;
	nameTemp = "";
	cin.clear();
	while (flag) {
        cout << "Enter last name: ";
        cin >> nameTemp;
        flag = validateName(nameTemp);
	}
	copyTempToLastName(nameTemp, currPatronInfo, row, column);

	cin.ignore();
	cin.ignore();
	flag = true;
	// validate phone number
	while (flag) {
        cout << setw(7) << " " << "Phone # in format nnnnnnnnnn\n\n";
        cin >> phoneNum;
        flag = validatePhoneNum(phoneNum);
	}
	copyTempToPhoneNum(phoneNum, currPatronInfo, row, column);
	
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
	cout << setw(7) << " " << "One patron's information will be used for every seat in a group sale.\n";

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
				cout << setw(7) << " " << "One or more of those seats is taken.\n" << setw(7) << " " << "Please make another selection.\n";
			}
		}
		if (fail == false && (colstart < 8 && colend > 7))
		{
			cin.clear();
			fflush(stdin);

			string userInput = "";
            bool flag = true;
            // validate user input
            while (flag){
                clearConsole;
                cout << setw(7) << " " << "This range of seats crosses the aisle.\n";
                cout << setw(7) << " " << "\nEnter Y or y to confirm selling the seats.\n";
                cin.ignore();
                cin >> userInput;
                flag = validate_Y_input(userInput);
            }
            choice = userInput[0];

			if (choice == 'y' || choice == 'Y')
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

void refundSeat(SeatInfo seats[ROWS][COLS], PatronInfo currPatronInfo[ROWS][COLS])
{
	int row = -1, col = -1;
	char choice = 'M';
//	string tempID="";

	getNumbers(row, "Enter the row for the seat the patron wants to refund.\n\n", 1, 10);
	getNumbers(col, "Enter the seat number(column) for the seat the patron wants to refund.\n\n", 1, 16);
	col = col - 1;
	row = 10 - row;
//	tempID=currPatronInfo[row][col].id;
//    showPatronInfo(tempID, currPatronInfo)
	cout << endl << currPatronInfo[row][col].id;
	cout << endl << currPatronInfo[row][col].firstName;
	cout << endl << currPatronInfo[row][col].lastName;
	cout << endl << currPatronInfo[row][col].phoneNum;
	cout << endl << "All of this info will be deleted\nThis cannot be undone.\nEnter Y or y to confirm deletion.";
    string userInput = "";
    bool flag = true;
            // validate user input
    while (flag)
    {
        cin.ignore();
        cin >> userInput;
        flag = validate_Y_input(userInput);
    }
    choice = userInput[0];

    if (choice == 'y' || choice == 'Y')
    {
        // make this stuff down here a function?
        resetCharArray(currPatronInfo[row][col].id, ID_SIZE);
        resetCharArray(currPatronInfo[row][col].lastName, FNAME_SIZE);
        resetCharArray(currPatronInfo[row][col].firstName, LNAME_SIZE);
        resetCharArray(currPatronInfo[row][col].phoneNum, DIGITS);
        resetCharArray(seats[row][col].IDS, ID_SIZE);
        seats[row][col].sold= false;
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
    string userInput = "";
    bool flag = true;

	cin.clear();
	fflush(stdin);
	// prompt for menu choice
	while (flag) {
        cout << "\n\n";
        cout << setw(7) << " " << "Enter an option A - H: ";
        cin >> userInput;
        flag = menuChoiceValidate(userInput);
	}
	choice = userInput[0]; // assign choice to first index of userInput

	// if user chose choice H
	if (choice == 'h' || choice == 'H')
	{
		cin.clear();
		fflush(stdin);
		// reset variables for validating
		userInput = "";
		flag = true;
		while (flag){
           cout << "Are you sure that you want to exit the program?\nIf you are sure enter y or Y\n";
           cin.ignore();
           cin >> userInput;
           flag = validate_Y_input(userInput);
		}
		choice = userInput[0];
		if (choice == 'y' || choice == 'Y')
			choice = 'H';
		else
			choice = 'X';
	}
	// if user chose choice F
	if (choice == 'f' || choice == 'F')
	{
		cin.clear();
		fflush(stdin);
		// reset variables for validating
		userInput = "";
		flag = true;
		while (flag){
           cout << "Are you sure you want to delete all ticket and patron information?\nThis cannot be undone.\nEnter y or Y if you are sure.";
           cin.ignore();
           cin >> userInput;
           flag = validate_Y_input(userInput);
		}
		choice = userInput[0];

		if (choice == 'y' || choice == 'Y')
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

void seatAndRevenu(SeatInfo seats[ROWS][COLS])
{
    system("CLS");
    int emptySeat =160;
    int totalRevenu=0;
    int taken;
    for(int i=0; i<ROWS;i++)
     {
        for(int j=0;j<COLS;j++)
            {
                if(seats[i][j].sold==true)
                {
                   emptySeat=emptySeat-1;
                   totalRevenu=totalRevenu+seats[i][j].price;
                }

            }
    }
     SetColor(3);
     cout<<endl;
     cout<<setw(25)<<"Total Empty Seat is: "<<setw(5)<<emptySeat<<endl;
     cout<<setw(25)<<"Total Taken Seat is: "<<setw(5)<<160-emptySeat<<endl;
     cout<<setw(19)<<"Total Seat is: "<<setw(11)<<"160"<<endl;
     cout<<setw(20)<<"Total Revenue is"<<setw(10)<<totalRevenu<<endl;
     cout<<endl;
     system("PAUSE");
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
	// concatenate number and letters
	ostringstream convert;
	convert << randNum;
	tempID += convert.str();
	// store ID in seat array and patron array
	for (int i = 0; i < ID_SIZE; i++) {
		seats[row][col].IDS[i] = tempID[i];
		currPatronInfo[row][col].id[i] = tempID[i];
	}
}

//************************************************************
//      Definition of validateFName                          *
//                                                           *
//       This function will
//************************************************************

bool validateName(string name) {
	bool flag = true;
	string errorNotLetter = "Name must contain letters only\n";
	string errorTooLong = "Name can only be 25 letters long.\n";
	try {
		// check if each character is a letter
		for (int i = 0; i < name.length(); i++) {
			if (!(isalpha(name[i])))
				throw errorNotLetter;
		}
		// check if name length is longer than firstName length
		if (name.length() > (FNAME_SIZE - 1))
			throw errorTooLong;
	}
	catch (string errorNotLetter) {
		cout << setw(7) << " " << "ERROR: Invalid Input." << endl;
		cout << setw(7) << " " << errorNotLetter;
		return flag;
	}
	catch (string errorTooLong) {
	    cout << setw(7) << " " << "ERROR: Invalid Input." << endl;
		cout << setw(7) << " " << errorTooLong;
		return flag;
	}
	return flag = false;
}

//************************************************************
//
//************************************************************

void copyTempToFirstName(string temp, PatronInfo currPatronInfo[ROWS][COLS], int row, int col) {

    for (int i = 0; i < FNAME_SIZE - 1; i++) {
        currPatronInfo[row][col].firstName[i] = temp[i];
    }
}

//************************************************************
//
//************************************************************

void copyTempToLastName(string temp, PatronInfo currPatronInfo[ROWS][COLS], int row, int col) {

    for (int i = 0; i < FNAME_SIZE - 1; i++) {
        currPatronInfo[row][col].lastName[i] = temp[i];
    }
}

//*************************************************
//    Definition of menuChoiceValidate            *
//                                                *
//      This function will ensures that the user
//*************************************************

bool menuChoiceValidate(string input) {
	bool flag = false;
	bool error = true;

	try {
	// check length of input, if greater than 1, input is wrong
	if (input.length() > 1)
        throw error;
    // check if first character is a letter
    if (!(isalpha(input[0])))
        throw error;
    // check if input was an option given
    if (!((input[0] >= 65 && input[0] <= 72) ||
			(input[0] >= 97 && input[0] <= 104)))
			throw error;
	}
	catch (bool error) {
	    cout << "\n\n";
	    cout << setw(7) << " " << "ERROR: Invalid Input." << endl;
	    cout << setw(7) << " " << "Choice must be a letter A - F" << endl;
	    return flag = true;
	}

	return flag;
}

//************************************************************
//      Definition of function validatePhoneNum              *
//                                                           *
//        This function will validate the phone number       *
//        entered by the user by determining if the phone    *
//        number is greater than 10 characters long or if    *
//        the number contains a characters that is not a     *
//        digit.                                             *
//        Parameters: input of type string                   *
//************************************************************

bool validatePhoneNum(string input) {
    // variables
    bool flag = true;
    string errorMsgTooLong = "Phone number can only be 10 digits long.";
    string errorMsgNotDigits = "Phone number must only contain numbers.";

    try {
        // determine if user entered a number longer than 10 numbers
        if (input.length() > 10)
            throw errorMsgTooLong;
        // determine if user a non-digit character
        for(int i = 0; i < input.length(); i++) {
            if ( !(isdigit(input[i])) )
                throw errorMsgNotDigits;
        }
    }
    catch (string errorMsgTooLong) {
        cout << setw(7) << "Error: Invalid Input." << endl;
        cout << setw(7) << errorMsgTooLong << endl;
        return flag;
    }
    catch (string errorMsgNotDigits) {
        cout << setw(7) << "Error: Invalid Input." << endl;
        cout << setw(7) << errorMsgNotDigits << endl;
        return flag;
    }
    // no errors
    return flag = false;
}

//************************************************************
//     Definition of copyTempToPhoneNum                      *
//                                                           *
//       This function is used with the validatePhoneNum     *
//       function. It 'copies' the contents of the string    *
//       phone number into the char array of the struct array*
//       member function dot phoneNum                        *
//************************************************************

bool copyTempToPhoneNum(string temp, PatronInfo currPatronInfo[ROWS][COLS], int row, int col) {

    for (int i = 0; i < DIGITS - 1; i++) {
        currPatronInfo[row][col].phoneNum[i] = temp[i];
    }
}

//************************************************************
//
//************************************************************

bool validate_Y_input(string input) {
    bool flag = true;
    string errorMsg = "Error: Invalid Input.";

    try {
        // determine if user entered more than one character
        if (input.length() > 1)
            throw errorMsg;
        // determine if user entered a letter other than y or Y
        if ( !(isalpha(input[0])) )
            throw errorMsg;
    }
    catch (string errorMsg) {
        cout << setw(7) << " " << errorMsg << endl;
        return flag;
    }
    return flag = false;
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
	cout << setw(7) << " " << "What does the patron sit?" << endl;
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

