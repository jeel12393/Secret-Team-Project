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

using namespace std;

// Constants
// Array sizes
const int ID_SIZE = 7, FNAME_SIZE = 25, LNAME_SIZE = 25, DIGITS = 10,
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

// Function Prototype
void showMenu();
void errorCheckin(char);
void showSeatingchar();
void SetColor(int);
//jeel's above
void initSeat(SeatInfo tempseats[ROWS][COLS]);
void updateInfoSingle(SeatInfo seats[ROWS][COLS], int rownum, int column);
// fucntion prototypes
void emptySeatChart(); // maybe delete
// Nick's functions
void saveSeatInfo(SeatInfo seats[ROWS][COLS], fstream &);
void getSeatInfo(SeatInfo seats[ROWS][COLS], fstream &);
void savePatronInfo(PatronInfo currentPatronInfo[ROWS][COLS], fstream &);
void getPatronInfo(PatronInfo currentPatronInfo[ROWS][COLS], fstream&);
void emptySeatInfo(SeatInfo seats[ROWS][COLS], PatronInfo currPatronInfo[ROWS][COLS]);
void resetCharArray(char[], int);
void sellSeat(SeatInfo seatstemp[ROWS][COLS], PatronInfo currentPatronInfo[ROWS][COLS]);

static char SChart[10][16];

int main()
{
    // file objects
    fstream seatFile;
    fstream patronFile;
    // 2-d arrays
    SeatInfo seats[ROWS][COLS];
    PatronInfo currPatronInfo[ROWS][COLS]; // new code

    initSeat(seats);
    emptySeatChart();
    // Make some dummy filled seats for testing
  //  SChart[0][0]='X';
  //  SChart[9][15]='X';
  //  SChart[6][6]='X';
    // Dummy seats ^^^^
    for(int i=0; i<10; i++)
    {
    showSeatingchar();
    sellSeat(seats, currPatronInfo);
    }
   // initSeat(seats);


    return 0;
}

void initSeat(SeatInfo tempseats[10][16])
{
    for(int count=0;count<10;count++)
    {
        for(int i=0;i<16;i++)
        {
            if(count<=3)
                tempseats[count][i].price=50;
            if(count>=4&&count<=7)
                tempseats[count][i].price=35;
            if(count>=8)
                tempseats[count][i].price=25;
            tempseats[count][i].col=i+1;
            tempseats[count][i].row=count+1;
            tempseats[count][i].sold=false;
            resetCharArray(tempseats[count][i].IDS, ID_SIZE);
          //  cout << tempseats[count][i].row << "  " << tempseats[count][i].col << "  " << tempseats[count][i].price << "  " << tempseats[count][i].sold << endl;
        }
    }
}

void sellSeat(SeatInfo seatstemp[10][16], PatronInfo currentPatronInfo[10][16])
{
	int row, column;
	cout << "Please enter the row for the seat that the patron is buying.\n";
	cin >> row;
	row=row-1;
	cout << "Please enter the column for the seat that the patron is buying.\n";
	cin >> column;
	column=column-1;
	cout << "Enter first name";
	cin >> currentPatronInfo[row][column].firstName;
	cout << "Last name\n";
	cin >> currentPatronInfo[row][column].lastName;
	cout << "Phone # in format nnnnnnnnnn";
	cin >> currentPatronInfo[row][column].phoneNum;
	updateInfoSingle(seatstemp, row, column);
}


void emptySeatChart()
{
    // empyt char array
    for(int i=0; i<10; i++)
    {
        for(int j=0; j<16; j++)
        {
            SChart[i][j]='O';
            // reset/empty seats array
            // set seats.sold = 0;
        }
    }
}


void updateInfoSingle(SeatInfo seats[10][16], int rownum, int column)
{
    seats[rownum][column].sold=true;
    SChart[rownum][column]='X';
}

//jeel's code

void showSeatingchar()
{
    const char TAKEN = 'X';//seats taken
	const char EMPTY = 'O';//seats free
	const int row=10;
	const int col=16;
	SetColor(14);
	cout<<endl<<endl<<endl<<endl;
    cout<<setw(48)<<"HUNTINGTON PLAYHOUSE"<<endl;
	cout<<setw(48)<<"SEATING CHART\n\n\n";
	cout<<setw(42)<<"Rear\n\n\n";
    cout <<setw(10)<< "            Seats " <<endl;
	cout<<setw(10)<<"Row"<<setw(2)<<" ";
	for(int k =1; k <=col;k++)
	{
		cout<<setw(1)<<k<<" " ;
		if(k<9)
        {
            cout<<" ";
        }
        if(k==8)
        {
            cout<< "     ";
        }
	}
    cout<<endl;
	cout<<setw(40)<< "Aisle"<<endl;
    for(int i=9;i>-1;i--)
	{
        cout << endl;
        cout<<setw(10)<< i+1<<setw(1)<<"  ";
		if(i<10)
        {
            cout<<"";
        }
        for(int j=0;j<col;j++)
		{
		 //   cout<<setw(1)<< EMPTY<<" ";
		 cout << setw(1) << SChart[i][j] << " ";
            if(j<9)
            {
                cout<<" ";
            }
            if(j>8)
            {
                cout<<" ";
            }
            if(j==7)
            {
                cout<<"     ";
            }
        }
	}
	cout <<endl;
	cout<<endl;
	cout<<setw(40)<<"Front"<<endl;
	string line1,line2;
    line1 =line1.assign(49,'-');
    line2 =line2.assign(30,'-');
    cout<<endl;
    cout<<setw(13)<<"+"<<line1<<"+"<<endl;
    cout<<setw(13)<<"|"<<setw(50)<<"|"<<endl;
    cout<<setw(13)<<"|"<<setw(27)<<"STAGE"<<setw(23)<<"|"<<endl;
    cout<<setw(13)<<"|"<<setw(50)<<"|"<<endl;
    cout<<setw(13)<<"+"<<line1<<"+"<<endl;
    {
        SetColor(13);
        cout<<"+"<<line2<<"+"<<endl;
        cout<<"|"<<"Row 1-4 Cost $50.00 Each"<<setw(7)<<"|"<<endl;
        cout<<"|"<<"Row 5-8 Cost $35.00 Each"<<setw(7)<<"|"<<endl;
        cout<<"|"<<"Row 9-10 Cost $25.00 Each"<<setw(6)<<"|"<<endl;
        cout<<"+"<<line2<<"+"<<endl;
    }

}

void SetColor(int value){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  value);
}
void showMenu()
{
    cout<<" A) Seating chart"<<endl;
    cout<<" B) Selling ticket"<<endl;
    cout<<" C) Search patron information"<<endl;
    cout<<" D) Current total of revenue"<<endl;
    cout<<" E) information about program"<<endl;
    cout<<" F) Quit Program"<<endl;

}
void errorCheckin(char choice)
{
    while(choice <'A'||choice>'F'&choice<'a'||choice>'f')
    {
        cout<<"Opps Enter Wrong Choice"<<endl;
        cout<<"Please Enter Correct choice"<<endl;
        cin>>choice;
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

	// fill some elements with info
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			seats[i][j].row = i;
			seats[i][j].col = j;
			if (i >= 0 && i <= 1) {
				seats[i][j].price = 25;
			}
			else if (i >= 2 && i <= 5) {
				seats[i][j].price = 35;
			}
			else if (i >= 6 && i <= 9) {
				seats[i][j].price = 50;
			}
			seats[i][j].sold = 0;
		}
	}
	// fill a few seat elements with id info
	bool status;

	for (int rows = 0; rows < 1; rows++) {
		for (int cols = 0; cols < 3; cols++) {
			cout << "Row " << rows << " seat " << cols << " status?" << endl;
			cin >> status;
			cin.ignore();
			cout << endl;
			cout << "what is the row " << rows << " seat " << cols << endl;
			cout << "ID? " << endl;
			cin.getline(seats[rows][cols].IDS, ID_SIZE);
			cout << endl << endl;
		}
	}

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
	while (!seatFile.eof()) {
		for (int i = 0; i < ROWS; i++) {
			for (int j = 0; j < COLS; j++) {
				seatFile.read(reinterpret_cast<char *>(&seats[i][j]),
					sizeof(seats[i][j]));
			}
		}
	}
	//close file
	seatFile.close();

	// test read data
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			cout << seats[i][j].row << endl;
			cout << seats[i][j].col << endl;
			cout << seats[i][j].price << endl;
			cout << seats[i][j].sold << endl;
			cout << seats[i][j].IDS << endl;
			cin.get();
			cout << endl << endl << endl;
		}
	}
	system("pause");
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
	while (!patronFile.eof()) {
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
			SChart[i][j]='O';
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

