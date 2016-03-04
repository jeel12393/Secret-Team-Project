#include <iostream>

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

void initSeat(SeatInfo tempseats[10][16]);
// fucntion prototypes 
void getSeatingInfo(PatronInfo currentPatronInfo[10][16], fstream&);

int main()
{
    SeatInfo seats[10][16];
    initSeat(seats);
    
    // Nick's code
    fstream dataFile;
	PatronInfo currentPatronInfo[10][16];
	// grab current patron information from file
	// and store data into passed array
	// will be seats array
	getSeatingInfo(currentPatronInfo, /*seats,*/ dataFile)
	// nicks code end
    
    
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

// nick's code
// search 
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


}
// end of nick's code 
