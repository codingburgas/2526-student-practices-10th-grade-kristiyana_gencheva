#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <windows.h>

using namespace std;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);


void color(int c) { SetConsoleTextAttribute(h, c); }
void pink() { color(13); }
void green() { color(10); }
void red() { color(12); }
void white() { color(15); }
void cyan() { color(11); }


void clear() { system("cls"); }


void header(string title)
{
    clear();
    pink();
    cout << "==== MOVIE SYSTEM ====\n\n";
    cyan();
    cout << title << "\n\n";
    white();
}


void pause()
{
    cout << "\nPress Enter...";
    cin.ignore();
    cin.get();
}


int menu(vector<string> items, string title)
{
    int selected = 0;

    while (true)
    {
        header(title);

        for (int i = 0; i < items.size(); i++)
        {
            if (i == selected)
            {
                green();
                cout << "> " << items[i] << "\n";
            }
            else
            {
                white();
                cout << "  " << items[i] << "\n";
            }
        }

        int key = _getch();

        if (key == 224)
        {
            key = _getch();
            if (key == 72) selected--;
            if (key == 80) selected++;
        }

        if (selected < 0) selected = items.size() - 1;
        if (selected >= items.size()) selected = 0;

        if (key == 13) return selected;
    }
}


struct Movie
{
    int id;
    string title;
};

struct Show
{
    int id;
    int movieId;
    string time;
};

struct Booking
{
    string name;
    int showId;
};


vector<Movie> movies;
vector<Show> shows;
vector<Booking> bookings;


void seed()
{
    movies.push_back({ 1, "Interstellar" });
    movies.push_back({ 2, "Frozen" });
    movies.push_back({ 3, "Parasite" });

    shows.push_back({ 1, 1, "18:00" });
    shows.push_back({ 2, 2, "20:00" });
}


void showMovies()
{
    header("Movies");

    for (auto m : movies)
    {
        cout << m.id << ". " << m.title << "\n";
    }

    pause();
}


void showShows()
{
    header("Shows");

    for (auto s : shows)
    {
        string movieName = "Unknown";

        for (auto m : movies)
            if (m.id == s.movieId)
                movieName = m.title;

        cout << "Show " << s.id << " | " << movieName << " | " << s.time << "\n";
    }

    pause();
}

void makeBooking()
{
    header("Make booking");

    showShows();

    int showId;
    cout << "\nEnter show ID: ";
    cin >> showId;

    cin.ignore();

    string name;
    cout << "Your name: ";
    getline(cin, name);

    bookings.push_back({ name, showId });

    green();
    cout << "\nBooking successful!\n";
    white();

    pause();
}


void showBookings()
{
    header("Bookings");

    if (bookings.empty())
    {
        red();
        cout << "No bookings.\n";
    }
    else
    {
        for (auto b : bookings)
        {
            cout << b.name << " -> Show " << b.showId << "\n";
        }
    }

    pause();
}


int main()
{
    seed();

    vector<string> mainMenu =
    {
        "Movies",
        "Shows",
        "Make booking",
        "View bookings",
        "Exit"
    };

    while (true)
    {
        int choice = menu(mainMenu, "Main Menu");

        if (choice == 0) showMovies();
        else if (choice == 1) showShows();
        else if (choice == 2) makeBooking();
        else if (choice == 3) showBookings();
        else break;
    }

}