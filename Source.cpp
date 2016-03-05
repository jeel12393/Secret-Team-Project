#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <conio.h>
#include <windows.h>

using namespace std;

struct SeatInfo
{
    int row, col, price;
    bool sold;
    string IDS;
};
// nick's code
struct PatronInfo {
	string id;
	string firstName;
	string lastName;
	int phoneNum;
};
// end of nick's code
void showMenu();
void errorCheckin(char);
void showSeatingchar();
void SetColor(int);
//jeel's above
void initSeat(SeatInfo tempseats[10][16]);
void updateInfoSingle(int rownum, int column);
// fucntion prototypes
void getSeatingInfo(PatronInfo currentPatronInfo[10][16], fstream&);
void emptySeatChart();

static char SChart[10][16];

int main()
{



    SeatInfo seats[10][16];
    PatronInfo currentPatronInfo[10][16];
    initSeat(seats);
    emptySeatChart();
    // Make some dummy filled seats for testing
    SChart[0][0]='X';
    SChart[10][16]='X';
    SChart[6][6]='X';
    // Dummy seats ^^^^
    showSeatingchar();
   /* initSeat(seats);

    // Nick's code
    fstream dataFile;
	PatronInfo currentPatronInfo[10][16];
	// grab current patron information from file
	// and store data into passed array
	// will be seats array
	getSeatingInfo(currentPatronInfo, /*seats,*/// dataFile)
	// nicks code end */


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
            tempseats[count][i].IDS="testID";
            cout << tempseats[count][i].row << "  " << tempseats[count][i].col << "  " << tempseats[count][i].price << "  " << tempseats[count][i].sold << endl;
        }
    }
}

void sellSeat(SeatInfo seatstemp[10][16], PatronInfo currentPatronInfo[10][16])
{
	int row, column;
	cout << "Please enter the row for the seat that the patron is buying.\n";
	cin >> row;
	cout << "Please enter the column for the seat that the patron is buying.\n";
	cin >> column;
	cout << "Enter first name";
	cin >> currentPatronInfo[row][column].firstName;
	cout << "Last name\n";
	cin >> currentPatronInfo[row][column].lastName;
	cout << "Phone # in format nnnnnnnnnn";
	cin >> currentPatronInfo[row][column].phoneNum;
}

void emptySeatChart()
{
    for(int i=0; i<10; i++)
    {
        for(int j=0; j<16; j++)
        {
            SChart[i][j]='O';
        }
    }
}

void updateInfoSingle(SeatInfo seats[10][16], int rownum, int column)
{
    seats[rownum][column].sold=true;
    SChart[rownum][column]='X';
}

// nick's code
// search
/*
void getSeatingInfo(PatronInfo currentPatronInfo[10][16], fstream &inputFile) {
	// open Patron file
	inputFile.open("PatronInfo", ios::in);
	if (inputFile.fail()) {
		cout << "Error opening file" << endl;
		inputFile.close();
	}
	else
		while (inputFile.eof()) {
			// get col and row from seatfile
			// use to search id strings in patron
			// file

		}
	}*/
// end of nick's code
//jeel's code

void showSeatingchar()
{
    const char TAKEN = 'X';//seats taken
	const char EMPTY = 'O';//seats free
	const int row=11;
	const int col=17;
	SetColor(14);
	cout<<endl<<endl<<endl<<endl;
    cout<<setw(48)<<"HUNTINGTON PLAYHOUSE"<<endl;
	cout<<setw(48)<<"SEATING CHART\n\n\n";
	cout<<setw(42)<<"Rear\n\n\n";
    cout <<setw(10)<< "            Seats " <<endl;
	cout<<setw(10)<<"Row"<<setw(2)<<" ";
	for(int k =1; k <col;k++)
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
        cout<<setw(10)<< i<<setw(1)<<"  ";
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
            if(j==8)
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


