#include <iostream>

using namespace std;

struct SeatInfo
{
    int row, col, price;
    bool sold;
    string IDS;
};

void initSeat(SeatInfo tempseats[10][16]);

int main()
{
    SeatInfo seats[10][16];
    initSeat(seats);
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

void sellSeats(SeatInfo seatstemp[10][16], )
