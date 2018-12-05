#include <iostream>
#include <algorithm>
#include <ctime>
#include <windows.h>
using namespace std;
struct coord
{
    int X;
    int Y;
    int number;
    bool selected;
    coord(){selected = false;}
};
class keno
{
public:
    //class default constructor
    keno();
    //game functions
    void display_board();
    void display_numbers();
    void get_picks();
    void computer_draw();
    void calc_winnings();
    void reset();
    //console enviornment
    void set_console_size(int x, int y);
    void gotoxy(int x, int y);
    void clear_screen();
private:
    int turn_counter;
    int picks;
    int money;
    int hits;
    int random_draws[20];
    int user_picks[10];
    int computer_picks[20];
    coord num_coords[80];
};
int main()
{
    keno mykeno;
    //Super-Complex Game Engine
    while(true)
    {
        mykeno.display_board();
        mykeno.display_numbers();
        mykeno.get_picks();
        mykeno.computer_draw();
        mykeno.calc_winnings();
        mykeno.reset();
    }
    return 0;
}
keno::keno()
{
    turn_counter  = 1;
    picks = 0;
    money = 500;
    hits = 0;
    srand(time(NULL));
    SetConsoleTitle("Keno!");
    set_console_size(58,37);
    //Generate X,Y ASCII grid coordinates for displaying numbers
    for(int i=0, x=8, y=10; i<80; i++, x+=5)
    {
        num_coords[i].X = x;
        num_coords[i].Y = y;
        num_coords[i].number = i+1;
        if(x>=53)
        {
            x=3;
            y+=3;
        }
    }
}
void keno::display_board()
{
    cout << "  ---------------------------------------------------\n"
    << " |\\                                                   \\\n"
    << " |  \\                                                   \\\n"
    << " |   |---------------------------------------------------|\n";
    for(int i=0; i<31; i++)
    {
        cout << " |   |                                                   |\n";
    }
    cout << "  \\  |                                                   |\n"
    << "    \\ --------------------------------------------------- \n";
    gotoxy(8,5);   cout << "Game #"   << turn_counter;
    gotoxy(40,5);  cout << "Total: $" << money;
    gotoxy(40,35); cout << "$20 per play.";
}
void keno::display_numbers()
{
    int randoms[80];
    int snooze = 30;
    int r=0;
    //populate an array of numbers
    for(int i=0; i<80; i++)
    {
        randoms[i] = i;
    }
    //randomly shuffle the array
    for(int i=0; i<160; i++)
    {
        swap(randoms[rand()%80], randoms[rand()%80]);
    }
    //move to the appropriate position on the board that corrosponds to the random number
    //and display it.
    for(int i=0; i<80; i++)
    {
        r = randoms[i];
        gotoxy(num_coords[r].X, num_coords[r].Y);
        cout << num_coords[r].number;
        Sleep(snooze);
    }
}
void keno::get_picks()
{
    int number = 0;
    int i=0;
    while(picks<10)
    {
        //previous entry clear
        gotoxy(8,35); cout << "                 ";
        gotoxy(8,35); cout << "Enter number: ";
        cin >> number;
        number--;
        user_picks[i] = number;
        //put [  ] box around user selected number
        gotoxy(num_coords[number].X-1, num_coords[number].Y); cout << '[';
        if(number<9) //handle 1 digit numbers
        {
            gotoxy(num_coords[number].X+1, num_coords[number].Y);
        }
        else  //handle 2 digit numbers
        {
            gotoxy(num_coords[number].X+2, num_coords[number].Y);
        }
        num_coords[number].selected = true;
        cout << ']';
        picks++;
        gotoxy(40,35);  cout << "Pick " << picks << " of 10 ";
    }
    //Clear to prevent further user entry
    gotoxy(8,35); cout << "                                                 ";
}
void keno::computer_draw()
{
    int randoms[80];
    int snooze = 30;
    int cp = 0;
    //$20 to play
    money -= 20;
    gotoxy(8,7);  cout << "Draw: ";
    gotoxy(40,7); cout << "Hits: ";
    //generate a list of numbers
    for(int i=0; i<80; i++)
    {
        randoms[i] = i;
    }
    //randomly shuffle the numbers
    for(int i=0; i<160; i++)
    {
        swap(randoms[rand()%80], randoms[rand()%80]);
    }
    //get a list of 20 'computer picked' randomly drawn numbers
    for(int i=0; i<20; i++)
    {
        computer_picks[i] = randoms[i];
    }
    for(int i=0; i<20; i++)
    {
        cp = computer_picks[i];
        //handle 'Hits'
        if(num_coords[cp].selected)
        {
            hits++;
            gotoxy(num_coords[cp].X-1, num_coords[cp].Y); cout << "HIT!";
            gotoxy(18,7); cout << "HIT!";
            gotoxy(47,7); cout << hits;
        }
        else //handle 'Miss'; display *'s around computer selected numbers.
        {
            gotoxy(num_coords[cp].X-1, num_coords[cp].Y); cout << '*';
            if(cp<9) //handle 1 digit numbers
            {
                gotoxy(num_coords[cp].X+1, num_coords[cp].Y); cout << '*';
            }
            else //handle 2 digit numbers
            {
                gotoxy(num_coords[cp].X+2, num_coords[cp].Y); cout << '*';
            }
        }
        gotoxy(14,7); cout << cp+1;
        Sleep(1000);
        //number clear
        gotoxy(18,7); cout << "    ";
    }
    //final clear
    gotoxy(8,7); cout << "                  ";
    turn_counter++;
}
void keno::calc_winnings()
{
    gotoxy(20,6); cout << "You Win ";
    switch(hits)
    {
        case 1:  money +=  10;    cout << "$10!";     break;
        case 2:  money +=  20;    cout << "$20!";     break;
        case 3:  money +=  50;    cout << "$50!";     break;
        case 4:  money +=  100;   cout << "$100!";    break;
        case 5:  money +=  200;   cout << "$200!";    break;
        case 6:  money +=  500;   cout << "$500!";    break;
        case 7:  money +=  1000;  cout << "$1,000!";  break;
        case 8:  money +=  2000;  cout << "$2,000!";  break;
        case 9:  money +=  5000;  cout << "$5,000!";  break;
        case 10: money +=  10000; cout << "$10,000!"; break;
        default: cout << "Nothing!";
    }
    //cin.ignore();
    //cin.get();
    Sleep(3000);
}
void keno::reset()
{
    picks = 0;
    hits = 0;
    clear_screen();
    for(int i=0; i<80; i++)
    {
        num_coords[i].selected = false;
    }
}
//Win32 API functions <windows.h>
void keno::gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void keno::clear_screen()
{
    DWORD n;
    DWORD size;
    COORD coord = {0};
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
    GetConsoleScreenBufferInfo ( h, &csbi );
    size = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter ( h, TEXT ( ' ' ), size, coord, &n );
    GetConsoleScreenBufferInfo ( h, &csbi );
    FillConsoleOutputAttribute ( h, csbi.wAttributes, size, coord, &n );
    SetConsoleCursorPosition ( h, coord );
}
void keno::set_console_size(int x, int y)
{
    HANDLE hOut;
    SMALL_RECT DisplayArea = {0, 0, 0, 0};
    COORD c = {0,0};
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DisplayArea.Right  = x;
    DisplayArea.Bottom = y;
    c.X = x+1;
    c.Y = y+1;
    SetConsoleWindowInfo(hOut, TRUE, &DisplayArea);
    SetConsoleScreenBufferSize(hOut, c);
}
