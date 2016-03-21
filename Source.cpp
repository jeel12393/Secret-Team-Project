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


static char SChart[ROWS][COLS]; // Static global used for printing the seating chart


// Structures are self explanatory in their contents via variable names.
struct SeatInfo // Reign
{
	int row, col, price;
	bool sold;
	char IDS[ID_SIZE];
};

struct PatronInfo { // Nick
	char id[ID_SIZE];
	char firstName[FNAME_SIZE];
	char lastName[LNAME_SIZE];
	char phoneNum[DIGITS];
};

// Function Prototypes
// Jeel's functions
void showMenu();
void errorCheckin(char);
void showSeatingchar();
void SetColor(int);
void seatAndRevenu(SeatInfo seats[ROWS][COLS]);
void credit();

// Nick's functions
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

// Reign's functions
void initSeat(SeatInfo seats[ROWS][COLS]); // Used to initialize a seats structure with base(row, column, price) as well as blank data
void sellSeat(SeatInfo seatstemp[ROWS][COLS], PatronInfo currPatronInfo[ROWS][COLS]); // Sell a single seat to the user
void getNumbers(int &thedata, string message, int upperbound); // Get numbers below a certain value, good for getting seat rows and columns
void menuChoice(char &choice); // Used to get user input for the menu
void updateSeatChart(SeatInfo seats[ROWS][COLS]); // Updates the SChart static global used for displaying the seating chart
void sellBlock(SeatInfo seats[ROWS][COLS], PatronInfo currPatronInfo[ROWS][COLS]); // Sells a portion of a row of seats to the user
void refundSeat(SeatInfo seats[ROWS][COLS], PatronInfo currPatronInfo[ROWS][COLS]); // Refunds a single seat to the user.
bool soldCheck(int row, int col, SeatInfo seats[ROWS][COLS]); // Aborts refund/search action if the seat is not already sold.

//-----------------------------------Reign's Code Below-----------------------------------
int main()
{
	// file objects
	fstream seatFile;
	fstream patronFile;

	// 2 dimensional arrays
	SeatInfo seats[ROWS][COLS];
	PatronInfo currPatronInfo[ROWS][COLS];

	char choice = NULL;

    // Function calls to initialize seats structures with prices, row & column numbers, and blank fields for other data.
	initSeat(seats);
	emptySeatInfo(seats, currPatronInfo);

	// read files for data
	// load data into seats and patron array
	getSeatInfo(seats, seatFile);
	getPatronInfo(currPatronInfo, patronFile);

    // Sentinel value used to control loop iteration
	while (choice != 'H')
	{

	    // Ensure choice is a value that won't take control from the user
		choice = 'X';

		// Updates the seating chart, prints it, and then prints the menu
		updateSeatChart(seats);
		showSeatingchar();
		showMenu();

		// Get user choice for the menu, and make it upper case
		menuChoice(choice);
		choice = toupper(choice);

		// Control logic for what the program will do next. 'H' is not shown for exit as it controls the loop above.
		switch (choice)
		{
		case 'A':
			sellSeat(seats, currPatronInfo);
			break;
		case 'B':
			sellBlock(seats, currPatronInfo);
			break;
		case 'C':
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
			credit();
			break;
		}
		clearConsole;
	}

    // Save information to disk
	saveSeatInfo(seats, seatFile);
	savePatronInfo(currPatronInfo, patronFile);

	return 0;
}


//**************************************************************************
//  function to initialize the seats array for prices, column numbers,     *
//  row numbers, and sold status.                                          *
//  For row numbers we ended up having [0][0] at the top left so           *
//  We use 10 - row (or in this case 10 - count) to decide which array     *
//  element is manipulated.                                                *
//**************************************************************************
void initSeat(SeatInfo seats[ROWS][COLS])
{
	for (int count = 0; count<ROWS; count++)
	{
		for (int i = 0; i<COLS; i++)
		{
			if (count <= 1)
				seats[count][i].price = 25;
			if (count >= 2 && count <= 5)
				seats[count][i].price = 35;
			if (count >= 6)
				seats[count][i].price = 50;
			seats[count][i].col = i + 1;
			seats[count][i].row = 10 - count;
		}
	}
}


//****************************************************************************
//  Function designed to return an integer between an upper bound            *
//  lower bound as specified by paramaters. Good for getting and validating  *
//  input data for row and column numbers. Also passes a message to be       *
//  repeated if data input is not good.                                      *
//****************************************************************************
void getNumbers(int &thedata, string message, int upperbound)
{
	cin.clear();
	fflush(stdin);
	char test[999];
	while (thedata>upperbound)
	{
		cout << setw(7) << " " << message << setw(7) << " ";
		cin.getline(test, INT_MAX);
		thedata = atoi(test);
		cin.clear();
		fflush(stdin);
	}
}

//**********************************************************************
//  function to sell a single seat to the user                         *
//**********************************************************************
void sellSeat(SeatInfo seatstemp[ROWS][COLS], PatronInfo currPatronInfo[ROWS][COLS])
{
    string nameTemp = "";
    string phoneNum = "";
    bool flag = true;

	int row = 50, column = 50;
	cout << "       Enter a 0 or negative number to go back to the main menu.\n";
	getNumbers(row, "Enter the row for the seat the patron is buying.\n", 10);

	//Logic to break the input+validation loop if the user decides he does not wish to continue with action
	if(row<1)
        return;

	getNumbers(column, "Enter the column for the seat that the patron is buying.\n", 16);
	if(column<1)
        return;
	column = column - 1;
	row = 10 - row;
	// prompt for and validate first name
	while (flag) {
        cout << setw(7) << " " << "Enter first name: ";
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
        cout << setw(7) << "  " << "Enter last name: ";
        cin >> nameTemp;
        flag = validateName(nameTemp);
	}
	copyTempToLastName(nameTemp, currPatronInfo, row, column);

	cin.ignore();
	flag = true;
	// validate phone number
	while (flag) {
        cout << setw(7) << " " << "Phone # in format nnnnnnnnnn\n       ";
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


//*******************************************
//  Updates the seating chart               *
//  based on the values in seats[][].sold   *
//*******************************************
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


//*******************************************
//  Function to sell blocks of seats at once
//*******************************************
void sellBlock(SeatInfo seats[ROWS][COLS], PatronInfo currPatronInfo[ROWS][COLS])
{
	cout << setw(7) << " " << "One patron's information will be used for every seat in a group sale.\n";

    // Initialize temporary variables used in function outside of input loop
    // because they are used both inside and outside of it
    string nameTemp = "";
    string phoneNum = "";
    bool flag = true;
	int row = 50, colstart = 50, colend = 50, tempcol = 50;
	char choice = 'M';
	bool fail = false;

	do
	{
	    // Ensure data will not pass error checking without new user input.
	    // EG if a good row is entered on first pass, but a bad colstart or colend
	    // is entered, a new request will be made for each value.
	    row = 50, colstart = 50, colend = 50;
		fail = false;

		// Get user input and then change to proper element numbers with 10-row and col-1
		getNumbers(row, "Enter the row for the first seat the patron is buying.\n", 10);
        if(row<1)
            return;
		row = 10 - row;
		getNumbers(colstart, "Enter the seat number(column) for the first seat.\n", 16);
        if(colstart<1)
            return;
		colstart = colstart - 1;
		getNumbers(colend, "Enter the seat number(column) for the last seat.\n", 16);
        if(colend<1)
            return;
		colend = colend - 1;

		// To avoid having lots of extra code for selling seats in reverse
		// I just compare the "start" to the "end" and ensure they are
		// the lower and higher values by swapping them if needed.
		if (colstart>colend)
		{
			tempcol = colend;
			colend = colstart;
			colstart = tempcol;
		}

		// Loop to check if any of the seats in the chosen range are taken.
		for (int i = colstart; i<=colend; i++)
		{
			if (seats[row][i].sold == true)
			{
				fail = true;
				cout << setw(7) << " " << "One or more of those seats is taken.\n" << setw(7) << " " << "Please make another selection.\n";
			}
		}

		// If statement to check if the seats cross the aisle, because some
		// patrons might be upset if they are not seated close to one another
		if (fail == false && (colstart < 8 && colend > 7))
		{
		    // Clear buffers
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
            // Assign choice to the user input once it is validated.
            choice = userInput[0];

            // If else statement to decide if seats are sold are not.
			if (choice == 'y' || choice == 'Y')
				break;
			else
				fail = true;
		}
	} while (fail == true);



    // First name and last name must be entered into string before
    // Batch processing to generate the ID

    // prompt for and validate first name
	while (flag) {
        cout << setw(7) << " " << "Enter first name: ";
        cin >> nameTemp;
        flag = validateName(nameTemp);
	}
	// copy contents of name to patron array stuct member firstName
	copyTempToFirstName(nameTemp, currPatronInfo, row, colstart);

	// prompt for and validate last name
	flag = true;
	nameTemp = "";
	cin.clear();
	while (flag) {
        cout << setw(7) << "  " << "Enter last name: ";
        cin >> nameTemp;
        flag = validateName(nameTemp);
	}
	copyTempToLastName(nameTemp, currPatronInfo, row, colstart);

	cin.ignore();
	flag = true;
	// validate phone number
	while (flag) {
        cout << setw(7) << " " << "Phone # in format nnnnnnnnnn\n\n";
        cin >> phoneNum;
        flag = validatePhoneNum(phoneNum);
	}
	copyTempToPhoneNum(phoneNum, currPatronInfo, row, colstart);



    // generate ID for patron
	generateID(row, colstart, seats, currPatronInfo);

	// show generated id
	cout << currPatronInfo[row][colstart].id << endl;
	cout << seats[row][colstart].IDS << endl;
	system("pause");

    // Copy patron's information to all sold seats.
	for (int i = colstart; i<=colend; i++)
	{
	    strcpy(currPatronInfo[row][i].id, currPatronInfo[row][colstart].id);
	    strcpy(seats[row][i].IDS, seats[row][colstart].IDS);
		strcpy(currPatronInfo[row][i].firstName, currPatronInfo[row][colstart].firstName);
		strcpy(currPatronInfo[row][i].lastName, currPatronInfo[row][colstart].lastName);
		strcpy(currPatronInfo[row][i].phoneNum, currPatronInfo[row][colstart].phoneNum);
		seats[row][i].sold = true;
	}
}

//*******************************
//  Function to refund a seat   *
//*******************************

void refundSeat(SeatInfo seats[ROWS][COLS], PatronInfo currPatronInfo[ROWS][COLS])
{
	int row = 50, col = 50;
	char choice = 'M';
	char tempID[ID_SIZE];

    // Usual get data and return if negative or 0 control structure
	getNumbers(row, "Enter the row for the seat the patron wants to refund.\n", 10);
    if(row<1)
        return;
	getNumbers(col, "Enter the seat number(column) for the seat.\n", 16);
    if(col<1)
        return;
	col = col - 1;
	row = 10 - row;

	// Check if the seat is actually sold or not.
	if(!soldCheck(row, col, seats))
	{
	    cout << "\n       This seat has not been sold.\n\n";
	    system("pause");
        return;
    }
    strncpy(tempID, seats[row][col].IDS, ID_SIZE - 1);
    showPatronInfo(tempID, currPatronInfo);

	cout << endl << "       All of this info will be deleted\n\n       This cannot be undone."
    << "\n\n       Enter Y or y to confirm deletion.\n\n       Enter any other character to cancel deletion: ";
    string userInput = "";
    bool flag = true;
            // validate user input
    while (flag)
    {
        cin >> userInput;
        flag = validate_Y_input(userInput);
    }
    choice = userInput[0];

    if (choice == 'y' || choice == 'Y')
    {
        resetCharArray(currPatronInfo[row][col].id, ID_SIZE);
        resetCharArray(currPatronInfo[row][col].lastName, FNAME_SIZE);
        resetCharArray(currPatronInfo[row][col].firstName, LNAME_SIZE);
        resetCharArray(currPatronInfo[row][col].phoneNum, DIGITS);
        resetCharArray(seats[row][col].IDS, ID_SIZE);
        seats[row][col].sold=false;
    }
}

//***********************************************
//  function to get user choice at main menu    *
//***********************************************
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
	// Assign choice to chosen value once validated
	choice = userInput[0];

    // Most user choices will return to main() to call functions based on input
    // User choice H and F will stay in this block to confirm input
    // Since these are actions that do not want to be accidentally activated

	// if user chose choice H
	if (choice == 'h' || choice == 'H')
	{
		cin.clear();
		fflush(stdin);
		// reset variables for validating
		userInput = "";
		flag = true;
		while (flag){
           cout << "\n       Are you sure that you want to exit the program?\n\n       "
           << "If you are sure enter Y or y.\n\n       Enter any other letter to go back to the main menu: ";
           cin >> userInput;
           flag = validate_Y_input(userInput);
		}

		// Assign choice to user input once validated to be a character
		// Returning the same letter, in this case H, confirms the action
		// Returning an X aborts action and returns to main() for new input
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
            cout << "\n       Are you sure you want to delete all ticket and patron information?\n\n       This cannot be undone.\n\n"
            << "       Enter Y or y if you are sure\n\n       Enter any other letter to cancel deletion: ";
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

//***************************************************
//  Checks if the seat has been sold.               *
//  Returns false if it has not been sold.          *
//  Returns true if it has been sold.               *
//***************************************************
bool soldCheck(int row, int col, SeatInfo seats[ROWS][COLS])
{
    if(seats[row][col].sold==false)
        return false;
    if(seats[row][col].sold==true)
        return true;
}
//-----------------------------------Reign's code above-----------------------------------


//-----------------------------------Jeel's code below-----------------------------------
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

void credit()
{
   system("CLS");
   SetColor(6);
   cout<<setw(10)<<"Designed and Programmed for HUNTIGTON PLAYHOUSE by Jedi Coders"<<endl<<endl;
   cout<<"Team Master:"<<endl;
   cout<<"Reign Durrent"<<endl;
   cout<<"ReignDurrent@gmail.com"<<endl<<endl;
   cout<<"Team Members:"<<endl;
   cout<<"Nicholas Bishop"<<endl;
   cout<<"Nicholasbishop27@ymail.com"<<endl<<endl;
   cout<<"Jeel Salvi"<<endl;
   cout<<"Jeel12393@gmail.com"<<endl<<endl;
   cout<<"Technical Support:"<<endl;
   cout<<"(361)742-6400"<<endl<<endl;
   cout<<"Technical Support Hours: 24/7"<<endl<<endl;
   system("PAUSE");
}
//-----------------------------------Jeel's code above-----------------------------------


//-----------------------------------Nick's code below-----------------------------------
//***********************************************************************
//    Definition of Function saveSeatFile                               *
//                                                                      *
//       This function will write each struct element                   *
//       to binary file seatsInfoFile.da                               *
//       Parameters: seats array of type SeatInfo (passed by reference) *
//                   seatFile of type fstream (passed by reference)     *
//***********************************************************************

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
//                                                         *
//        This function will reset the information of the  *
//        seats array and curPatronInfo array. This Func-  *
//        tion will also call the resetCharArray to assign *
//        x to each element in a char array.               *
//        Parameters: seats array (by reference)           *
//                    currPatronInfo array (by reference)  *
//**********************************************************

void emptySeatInfo(SeatInfo seats[ROWS][COLS], PatronInfo currPatronInfo[ROWS][COLS]) {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			// empty XO char array
			SChart[i][j] = 'O';
			// reset info in seats array
			seats[i][j].sold = false;
			resetCharArray(seats[i][j].IDS, ID_SIZE);
			// reset info in patrons array
			resetCharArray(currPatronInfo[i][j].id, (ID_SIZE - 1));
			resetCharArray(currPatronInfo[i][j].firstName, (FNAME_SIZE - 1));
			resetCharArray(currPatronInfo[i][j].lastName, (LNAME_SIZE- 1));
			resetCharArray(currPatronInfo[i][j].phoneNum, (DIGITS - 1));
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
//       This function will validate whether the name entered*
//       by the user contains letters only and is of proper  *
//       length. If not, function will return false.         *
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
	    SetColor(12);
		cout << setw(7) << " " << "ERROR: Invalid Input." << endl;
		cout << setw(7) << " " << errorNotLetter;
		SetColor(13);
		return flag;
	}
	catch (string errorTooLong) {
	    SetColor(12);
	    cout << setw(7) << " " << "ERROR: Invalid Input." << endl;
		cout << setw(7) << " " << errorTooLong;
		SetColor(13);
		return flag;
	}
	return flag = false;
}

//************************************************************
//     Definition of copyTemptoFirstName                     *
//                                                           *
//       This function is used with validateName, it is used *
//       to copy the validated name to a char array in the   *
//       PatronInfo array. specifically the First Name       *
//************************************************************

void copyTempToFirstName(string temp, PatronInfo currPatronInfo[ROWS][COLS], int row, int col) {

    for (int i = 0; i < FNAME_SIZE - 1; i++) {
        currPatronInfo[row][col].firstName[i] = temp[i];
    }
}

//************************************************************
//     Definition of copyTemptoLastName                      *
//                                                           *
//       This function is used with validateName, it is used *
//       to copy the validated name to a char array in the   *
//       PatronInfo array. Specifically for the last name.   *
//************************************************************

void copyTempToLastName(string temp, PatronInfo currPatronInfo[ROWS][COLS], int row, int col) {
    // copy each element value from temp string to lastname meber function
    for (int i = 0; i < FNAME_SIZE - 1; i++) {
        currPatronInfo[row][col].lastName[i] = temp[i];
    }
}

//*************************************************
//    Definition of menuChoiceValidate            *
//                                                *
//      This function will validate whether the   *
//      user entered a correct menu option, if not*
//      the function will return true             *
//      Parameters: input of type string          *
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
	    SetColor(12);
	    cout << setw(7) << " " << "ERROR: Invalid Input." << endl;
	    cout << setw(7) << " " << "Choice must be a letter A - F" << endl;
	    SetColor(13);
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
        SetColor(12);
        cout << setw(7) << "Error: Invalid Input." << endl;
        cout << setw(7) << errorMsgTooLong << endl;
        SetColor(13);
        return flag;
    }
    catch (string errorMsgNotDigits) {
        SetColor(12);
        cout << setw(7) << "Error: Invalid Input." << endl;
        cout << setw(7) << errorMsgNotDigits << endl;
        SetColor(13);
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
    // assign each element value in temp string to phoneNum char array in
    // requested patron array element
    for (int i = 0; i < DIGITS - 1; i++) {
        currPatronInfo[row][col].phoneNum[i] = temp[i];
    }
}

//************************************************************
//     Definition of validate_Y_input                        *
//                                                           *
//        this function will validate whether the user       *
//        entered a y or Y. If input is not valid, function  *
//        return true.                                       *
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
        SetColor(12);
        cout << setw(7) << " " << errorMsg << endl;
        SetColor(13);
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
    if(!soldCheck(row, col, seats))
	{
	    cout << "\n       This seat has not been sold.\n\n";
	    system("pause");
        return;
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
