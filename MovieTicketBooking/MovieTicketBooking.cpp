#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <windows.h>
#include <ctime>

using namespace std;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

void pink() { SetConsoleTextAttribute(h, 13); 
}
void darkPink() { SetConsoleTextAttribute(h, 5); 
}
void yellow() { SetConsoleTextAttribute(h, 14); 
}

void clearScreen() 
{
    system("cls");
}

void moveCursor(short x, short y) 
{
    COORD p = { x, y };
    SetConsoleCursorPosition(h, p);
}

void drawStars(int count) 
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(h, &csbi);
    int width = csbi.dwSize.X;
    int height = csbi.dwSize.Y;

    yellow();
    for (int i = 0; i < count; i++) 
    {
        int x = rand() % width;
        int y = rand() % height;
        moveCursor(x, y);
        cout << "*";
    }
    pink();
}

vector<string> titleArt() 
{
    return {
"_________ .___ _______  ___________   _____      _____   ",
"\\_   ___ \\|   |\\      \\ \\_   _____/  /     \\    /  _  \\  ",
"/    \\  \\/|   |/   |   \\ |    __)_  /  \\ /  \\  /  /_\\  \\ ",
"\\     \\___|   /    |    \\|        \\/    Y    \\/    |    \\",
" \\______  /___\\____|__  /_______  /\\____|__  /\\____|__  /",
"        \\/            \\/        \\/         \\/         \\/ "
    };
}

int selectMenu(const vector<string>& items) 
{
    int selected = 0;

    while (true) 
    {
        clearScreen();
        pink();

        drawStars(120);

        moveCursor(0, 0);

        for (auto& line : titleArt())
            cout << line << "\n";

        cout << "\n\n";

        for (int i = 0; i < items.size(); i++) 
        {
            if (i == selected) {
                cout << " > " << items[i] << "\n";
            }
            else {
                darkPink();
                cout << "   " << items[i] << "\n";
                pink();
            }
        }

        int key = _getch();

        if (key == 224) 
        {
            key = _getch();
            if (key == 72)
                selected = (selected - 1 + items.size()) % items.size();
            else if (key == 80)
                selected = (selected + 1) % items.size();
        }
        else if (key == 13) 
        {
            return selected;
        }
    }
}

void showMessage(const string& msg) 
{
    clearScreen();
    pink();
    drawStars(80);
    moveCursor(0, 0);
    cout << msg << "\n";
    Sleep(700);
}

int main() 
{

    srand((unsigned)time(nullptr));
    SetConsoleOutputCP(65001);

    vector<string> mainMenu = 
    {
        "Search movies",
        "View showtimes",
        "Make booking",
        "Admin menu",
        "Exit"
    };

    vector<string> adminMenu = 
    {
        "Add show",
        "Delete show",
        "Update show",
        "Add movie",
        "Delete movie",
        "Back"
    };

    while (true) 
    {
        int choice = selectMenu(mainMenu);

        if (choice == 0) showMessage("Accessing movie database...");
        else if (choice == 1) showMessage("Loading showtimes...");
        else if (choice == 2) showMessage("Opening booking system...");
        else if (choice == 3) 
        {
            while (true) 
            {
                int a = selectMenu(adminMenu);
                if (a == adminMenu.size() - 1) break;
                showMessage("Executing command...");
            }
        }
        else if (choice == mainMenu.size() - 1) 
        {
            clearScreen();
            pink();
            drawStars(100);
            moveCursor(0, 0);
            cout << "System shutdown...\n";
            break;
        }
    }
}