#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#define NOMINMAX
#include <windows.h>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <limits>

using namespace std;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

void pink() { SetConsoleTextAttribute(h, 13); }
void darkPink() { SetConsoleTextAttribute(h, 5); }
void yellow() { SetConsoleTextAttribute(h, 14); }
void whiteColor() { SetConsoleTextAttribute(h, 15); }
void redColor() { SetConsoleTextAttribute(h, 12); }
void greenColor() { SetConsoleTextAttribute(h, 10); }
void cyanColor() { SetConsoleTextAttribute(h, 11); }

void clearScreen()
{
    system("cls");
}

void moveCursor(short x, short y)
{
    COORD p = { x, y };
    SetConsoleCursorPosition(h, p);
}

void pauseConsole()
{
    whiteColor();
    cout << "\nPress Enter to continue...";
    cin.get();
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
        int x = rand() % ((width > 0) ? width : 1);
        int y = rand() % ((height > 0) ? height : 1);
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

void printHeader(const string& subtitle = "")
{
    clearScreen();
    pink();
    drawStars(100);
    moveCursor(0, 0);

    for (const auto& line : titleArt())
        cout << line << "\n";

    if (!subtitle.empty())
    {
        cout << "\n";
        cyanColor();
        cout << subtitle << "\n";
        pink();
    }

    cout << "\n";
}

int selectMenu(const vector<string>& items, const string& subtitle = "")
{
    int selected = 0;

    while (true)
    {
        clearScreen();
        pink();
        drawStars(120);
        moveCursor(0, 0);

        for (const auto& line : titleArt())
            cout << line << "\n";

        if (!subtitle.empty())
        {
            cout << "\n";
            cyanColor();
            cout << subtitle << "\n";
            pink();
        }

        cout << "\n";

        for (int i = 0; i < (int)items.size(); i++)
        {
            if (i == selected)
            {
                greenColor();
                cout << " > " << items[i] << "\n";
                pink();
            }
            else
            {
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
                selected = (selected - 1 + (int)items.size()) % (int)items.size();
            else if (key == 80)
                selected = (selected + 1) % (int)items.size();
        }
        else if (key == 13)
        {
            return selected;
        }
    }
}

void showMessage(const string& msg, int delay = 900)
{
    printHeader("System message");
    whiteColor();
    cout << msg << "\n";
    Sleep(delay);
}

string toLowerCase(string s)
{
    transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return (char)tolower(c); });
    return s;
}

bool containsIgnoreCase(const string& text, const string& part)
{
    return toLowerCase(text).find(toLowerCase(part)) != string::npos;
}

string readLine(const string& prompt)
{
    whiteColor();
    cout << prompt;
    string value;
    getline(cin, value);
    return value;
}

int readInt(const string& prompt, int minValue, int maxValue)
{
    while (true)
    {
        whiteColor();
        cout << prompt;
        int value;
        cin >> value;

        if (!cin.fail() && value >= minValue && value <= maxValue)
        {
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            return value;
        }

        cin.clear();
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
        redColor();
        cout << "Invalid input. Try again.\n";
    }
}

double readDouble(const string& prompt, double minValue)
{
    while (true)
    {
        whiteColor();
        cout << prompt;
        double value;
        cin >> value;

        if (!cin.fail() && value >= minValue)
        {
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            return value;
        }

        cin.clear();
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
        redColor();
        cout << "Invalid input. Try again.\n";
    }
}

struct Movie
{
    int id;
    string title;
    string language;
    string genre;
    string releaseDate;
    int durationMinutes;
};

struct Hall
{
    int id;
    string name;
};

struct Cinema
{
    int id;
    string name;
    string city;
    vector<Hall> halls;
};

enum class SeatType
{
    Silver,
    Gold,
    Platinum
};

string seatTypeToString(SeatType type)
{
    switch (type)
    {
    case SeatType::Silver: return "Silver";
    case SeatType::Gold: return "Gold";
    case SeatType::Platinum: return "Platinum";
    default: return "Unknown";
    }
}

double seatPrice(SeatType type)
{
    switch (type)
    {
    case SeatType::Silver: return 10.00;
    case SeatType::Gold: return 15.00;
    case SeatType::Platinum: return 20.00;
    default: return 0.00;
    }
}

struct Seat
{
    string code;
    SeatType type;
    bool booked;
};

struct Show
{
    int id;
    int movieId;
    int cinemaId;
    int hallId;
    string showDate;
    string showTime;
    vector<Seat> seats;
};

enum class BookingChannel
{
    Online,
    WalkIn
};

enum class PaymentMethod
{
    CreditCard,
    Cash
};

string bookingChannelToString(BookingChannel channel)
{
    return (channel == BookingChannel::Online) ? "Online" : "Walk-in";
}

string paymentMethodToString(PaymentMethod payment)
{
    return (payment == PaymentMethod::CreditCard) ? "Credit Card" : "Cash";
}

struct Booking
{
    int id;
    string customerName;
    int showId;
    vector<string> seatCodes;
    BookingChannel channel;
    PaymentMethod payment;
    double totalAmount;
    bool active;
};

struct Notification
{
    string message;
};

vector<Movie> movies;
vector<Cinema> cinemas;
vector<Show> shows;
vector<Booking> bookings;
vector<Notification> notifications;

int nextMovieId = 1;
int nextShowId = 1;
int nextBookingId = 1;

Movie* findMovieById(int id)
{
    for (auto& movie : movies)
        if (movie.id == id)
            return &movie;
    return nullptr;
}

Cinema* findCinemaById(int id)
{
    for (auto& cinema : cinemas)
        if (cinema.id == id)
            return &cinema;
    return nullptr;
}

Hall* findHallByIds(int cinemaId, int hallId)
{
    Cinema* cinema = findCinemaById(cinemaId);
    if (!cinema) return nullptr;

    for (auto& hall : cinema->halls)
        if (hall.id == hallId)
            return &hall;

    return nullptr;
}

Show* findShowById(int id)
{
    for (auto& show : shows)
        if (show.id == id)
            return &show;
    return nullptr;
}

Booking* findBookingById(int id)
{
    for (auto& booking : bookings)
        if (booking.id == id)
            return &booking;
    return nullptr;
}

vector<Seat> createDefaultSeats()
{
    vector<Seat> seats;
    vector<char> rows = { 'A', 'B', 'C', 'D', 'E' };

    for (int r = 0; r < (int)rows.size(); r++)
    {
        for (int c = 1; c <= 8; c++)
        {
            SeatType type;
            if (r <= 1) type = SeatType::Silver;
            else if (r <= 3) type = SeatType::Gold;
            else type = SeatType::Platinum;

            Seat seat;
            seat.code = string(1, rows[r]) + to_string(c);
            seat.type = type;
            seat.booked = false;
            seats.push_back(seat);
        }
    }

    return seats;
}

void addNotification(const string& text)
{
    notifications.push_back({ text });
}

void seedData()
{
    cinemas.push_back({ 1, "Cinema City Center", "Burgas", {{1, "Hall 1"}, {2, "Hall 2"}} });
    cinemas.push_back({ 2, "Grand Mall Cinema", "Varna", {{1, "Hall 1"}, {2, "Hall 2"}, {3, "Hall 3"}} });

    movies.push_back({ nextMovieId++, "Interstellar", "English", "Sci-Fi", "2014-11-07", 169 });
    movies.push_back({ nextMovieId++, "Frozen", "English", "Animation", "2013-11-27", 102 });
    movies.push_back({ nextMovieId++, "Parasite", "Korean", "Thriller", "2019-05-30", 132 });
    movies.push_back({ nextMovieId++, "Dune", "English", "Sci-Fi", "2021-10-22", 155 });

    shows.push_back({ nextShowId++, 1, 1, 1, "2026-03-12", "18:00", createDefaultSeats() });
    shows.push_back({ nextShowId++, 1, 1, 2, "2026-03-12", "21:00", createDefaultSeats() });
    shows.push_back({ nextShowId++, 2, 2, 1, "2026-03-12", "16:00", createDefaultSeats() });
    shows.push_back({ nextShowId++, 3, 2, 2, "2026-03-13", "20:30", createDefaultSeats() });
    shows.push_back({ nextShowId++, 4, 1, 1, "2026-03-13", "19:30", createDefaultSeats() });
}

void printMovie(const Movie& movie)
{
    whiteColor();
    cout << "ID: " << movie.id
        << " | Title: " << movie.title
        << " | Language: " << movie.language
        << " | Genre: " << movie.genre
        << " | Release date: " << movie.releaseDate
        << " | Duration: " << movie.durationMinutes << " min\n";
}

void printShow(const Show& show)
{
    Movie* movie = findMovieById(show.movieId);
    Cinema* cinema = findCinemaById(show.cinemaId);
    Hall* hall = findHallByIds(show.cinemaId, show.hallId);

    whiteColor();
    cout << "Show ID: " << show.id
        << " | Movie: " << (movie ? movie->title : "Unknown")
        << " | Cinema: " << (cinema ? cinema->name : "Unknown")
        << " | City: " << (cinema ? cinema->city : "Unknown")
        << " | Hall: " << (hall ? hall->name : "Unknown")
        << " | Date: " << show.showDate
        << " | Time: " << show.showTime << "\n";
}

void printAllMovies()
{
    if (movies.empty())
    {
        redColor();
        cout << "No movies available.\n";
        return;
    }

    for (const auto& movie : movies)
        printMovie(movie);
}

void printAllShows()
{
    if (shows.empty())
    {
        redColor();
        cout << "No shows available.\n";
        return;
    }

    for (const auto& show : shows)
        printShow(show);
}

void searchMovies()
{
    vector<string> searchMenu =
    {
        "Search by title",
        "Search by language",
        "Search by genre",
        "Search by release date",
        "Back"
    };

    while (true)
    {
        int choice = selectMenu(searchMenu, "Movie search");

        if (choice == 4) return;

        printHeader("Search results");

        string query;
        if (choice == 0) query = readLine("Enter title: ");
        else if (choice == 1) query = readLine("Enter language: ");
        else if (choice == 2) query = readLine("Enter genre: ");
        else if (choice == 3) query = readLine("Enter release date (YYYY-MM-DD): ");

        bool found = false;

        for (const auto& movie : movies)
        {
            bool match = false;

            if (choice == 0 && containsIgnoreCase(movie.title, query)) match = true;
            if (choice == 1 && containsIgnoreCase(movie.language, query)) match = true;
            if (choice == 2 && containsIgnoreCase(movie.genre, query)) match = true;
            if (choice == 3 && containsIgnoreCase(movie.releaseDate, query)) match = true;

            if (match)
            {
                printMovie(movie);
                found = true;
            }
        }

        if (!found)
        {
            redColor();
            cout << "No movies found.\n";
        }

        pauseConsole();
    }
}

void viewShowtimes()
{
    printHeader("Available showtimes");
    printAllShows();
    pauseConsole();
}

void printSeatLegend()
{
    cout << "\nLegend:\n";
    yellow();
    cout << "[A] Available  ";
    redColor();
    cout << "[X] Booked\n";
    whiteColor();
    cout << "Rows A-B = Silver ($" << seatPrice(SeatType::Silver) << ")\n";
    cout << "Rows C-D = Gold ($" << seatPrice(SeatType::Gold) << ")\n";
    cout << "Row E    = Platinum ($" << seatPrice(SeatType::Platinum) << ")\n";
}

void displaySeats(const Show& show)
{
    whiteColor();
    cout << "\nSeating layout:\n\n";

    for (int i = 0; i < (int)show.seats.size(); i++)
    {
        const Seat& seat = show.seats[i];

        if (i % 8 == 0)
            cout << seat.code[0] << "  ";

        if (seat.booked)
        {
            redColor();
            cout << setw(4) << "[X]";
        }
        else
        {
            greenColor();
            cout << setw(4) << "[A]";
        }

        if (i % 8 == 7)
        {
            whiteColor();
            cout << "\n";
        }
    }

    whiteColor();
    cout << "\n    ";
    for (int c = 1; c <= 8; c++)
        cout << setw(4) << c;
    cout << "\n";

    printSeatLegend();
}

Seat* findSeatByCode(Show& show, const string& code)
{
    for (auto& seat : show.seats)
        if (toLowerCase(seat.code) == toLowerCase(code))
            return &seat;
    return nullptr;
}

bool hallIsBusyAtTime(int cinemaId, int hallId, const string& date, const string& time, int ignoredShowId = -1)
{
    for (const auto& show : shows)
    {
        if (show.id != ignoredShowId &&
            show.cinemaId == cinemaId &&
            show.hallId == hallId &&
            show.showDate == date &&
            show.showTime == time)
        {
            return true;
        }
    }
    return false;
}

void makeBooking()
{
    printHeader("Make booking");

    if (shows.empty())
    {
        redColor();
        cout << "There are no shows available.\n";
        pauseConsole();
        return;
    }

    printAllShows();
    cout << "\n";
    int showId = readInt("Enter Show ID: ", 1, 1000000);

    Show* show = findShowById(showId);
    if (!show)
    {
        redColor();
        cout << "Show not found.\n";
        pauseConsole();
        return;
    }

    Movie* movie = findMovieById(show->movieId);
    Cinema* cinema = findCinemaById(show->cinemaId);
    Hall* hall = findHallByIds(show->cinemaId, show->hallId);

    printHeader("Booking details");
    whiteColor();
    cout << "Movie: " << (movie ? movie->title : "Unknown") << "\n";
    cout << "Cinema: " << (cinema ? cinema->name : "Unknown") << "\n";
    cout << "Hall: " << (hall ? hall->name : "Unknown") << "\n";
    cout << "Date: " << show->showDate << "\n";
    cout << "Time: " << show->showTime << "\n";

    displaySeats(*show);

    string customerName = readLine("\nEnter customer name: ");

    vector<string> channelMenu = { "Online customer", "Walk-in customer" };
    int channelChoice = selectMenu(channelMenu, "Booking channel");
    BookingChannel channel = (channelChoice == 0) ? BookingChannel::Online : BookingChannel::WalkIn;

    PaymentMethod payment;
    if (channel == BookingChannel::Online)
    {
        payment = PaymentMethod::CreditCard;
        showMessage("Online customers can pay only with credit card.");
    }
    else
    {
        vector<string> paymentMenu = { "Credit Card", "Cash" };
        int paymentChoice = selectMenu(paymentMenu, "Select payment method");
        payment = (paymentChoice == 0) ? PaymentMethod::CreditCard : PaymentMethod::Cash;
    }

    int seatCount = readInt("How many seats do you want to book? ", 1, 10);

    vector<string> selectedSeats;
    double total = 0.0;

    for (int i = 0; i < seatCount; i++)
    {
        while (true)
        {
            string seatCode = readLine("Enter seat code #" + to_string(i + 1) + " (example: A1): ");
            Seat* seat = findSeatByCode(*show, seatCode);

            if (!seat)
            {
                redColor();
                cout << "Seat does not exist.\n";
                continue;
            }

            if (seat->booked)
            {
                redColor();
                cout << "Seat already booked.\n";
                continue;
            }

            bool alreadyChosen = false;
            for (const auto& s : selectedSeats)
            {
                if (toLowerCase(s) == toLowerCase(seatCode))
                {
                    alreadyChosen = true;
                    break;
                }
            }

            if (alreadyChosen)
            {
                redColor();
                cout << "You already selected that seat.\n";
                continue;
            }

            selectedSeats.push_back(seat->code);
            total += seatPrice(seat->type);
            greenColor();
            cout << "Seat " << seat->code << " added (" << seatTypeToString(seat->type)
                << " - $" << fixed << setprecision(2) << seatPrice(seat->type) << ")\n";
            break;
        }
    }

    whiteColor();
    cout << "\nBooking summary:\n";
    cout << "Customer: " << customerName << "\n";
    cout << "Channel: " << bookingChannelToString(channel) << "\n";
    cout << "Payment: " << paymentMethodToString(payment) << "\n";
    cout << "Seats: ";
    for (const auto& s : selectedSeats) cout << s << " ";
    cout << "\nTotal: $" << fixed << setprecision(2) << total << "\n";

    string confirm = readLine("Confirm booking? (yes/no): ");
    if (toLowerCase(confirm) != "yes")
    {
        redColor();
        cout << "Booking canceled before payment.\n";
        pauseConsole();
        return;
    }

    for (auto& code : selectedSeats)
    {
        Seat* seat = findSeatByCode(*show, code);
        if (seat) seat->booked = true;
    }

    Booking booking;
    booking.id = nextBookingId++;
    booking.customerName = customerName;
    booking.showId = show->id;
    booking.seatCodes = selectedSeats;
    booking.channel = channel;
    booking.payment = payment;
    booking.totalAmount = total;
    booking.active = true;

    bookings.push_back(booking);

    addNotification("Booking made: Booking ID " + to_string(booking.id) +
        " for " + customerName + " (" + (movie ? movie->title : "Unknown movie") + ").");

    showMessage("Payment successful. Booking confirmed!");

    printHeader("Booking confirmation");
    greenColor();
    cout << "Booking ID: " << booking.id << "\n";
    whiteColor();
    cout << "Customer: " << booking.customerName << "\n";
    cout << "Movie: " << (movie ? movie->title : "Unknown") << "\n";
    cout << "Cinema: " << (cinema ? cinema->name : "Unknown") << "\n";
    cout << "Hall: " << (hall ? hall->name : "Unknown") << "\n";
    cout << "Date: " << show->showDate << "\n";
    cout << "Time: " << show->showTime << "\n";
    cout << "Seats: ";
    for (const auto& s : booking.seatCodes) cout << s << " ";
    cout << "\nPayment: " << paymentMethodToString(booking.payment) << "\n";
    cout << "Total: $" << fixed << setprecision(2) << booking.totalAmount << "\n";
    cout << "Email notification: SENT (simulated)\n";

    pauseConsole();
}

void cancelBooking()
{
    printHeader("Cancel booking");

    if (bookings.empty())
    {
        redColor();
        cout << "There are no bookings yet.\n";
        pauseConsole();
        return;
    }

    whiteColor();
    cout << "Existing bookings:\n\n";
    for (const auto& booking : bookings)
    {
        if (!booking.active) continue;

        Show* show = findShowById(booking.showId);
        Movie* movie = show ? findMovieById(show->movieId) : nullptr;

        cout << "Booking ID: " << booking.id
            << " | Customer: " << booking.customerName
            << " | Movie: " << (movie ? movie->title : "Unknown")
            << " | Seats: ";

        for (const auto& seat : booking.seatCodes) cout << seat << " ";
        cout << "| Total: $" << fixed << setprecision(2) << booking.totalAmount << "\n";
    }

    cout << "\n";
    int bookingId = readInt("Enter Booking ID to cancel: ", 1, 1000000);
    Booking* booking = findBookingById(bookingId);

    if (!booking || !booking->active)
    {
        redColor();
        cout << "Active booking not found.\n";
        pauseConsole();
        return;
    }

    Show* show = findShowById(booking->showId);
    Movie* movie = show ? findMovieById(show->movieId) : nullptr;

    if (show)
    {
        for (const auto& seatCode : booking->seatCodes)
        {
            Seat* seat = findSeatByCode(*show, seatCode);
            if (seat) seat->booked = false;
        }
    }

    booking->active = false;

    addNotification("Booking canceled: Booking ID " + to_string(booking->id) +
        " for " + booking->customerName + " (" + (movie ? movie->title : "Unknown movie") + ").");

    greenColor();
    cout << "Booking canceled successfully.\n";
    pauseConsole();
}

void viewNotifications()
{
    printHeader("Notifications");

    if (notifications.empty())
    {
        redColor();
        cout << "No notifications available.\n";
        pauseConsole();
        return;
    }

    whiteColor();
    for (int i = 0; i < (int)notifications.size(); i++)
    {
        cout << i + 1 << ". " << notifications[i].message << "\n";
    }

    pauseConsole();
}

void addMovie()
{
    printHeader("Admin - Add movie");

    Movie movie;
    movie.id = nextMovieId++;
    movie.title = readLine("Title: ");
    movie.language = readLine("Language: ");
    movie.genre = readLine("Genre: ");
    movie.releaseDate = readLine("Release date (YYYY-MM-DD): ");
    movie.durationMinutes = readInt("Duration in minutes: ", 1, 500);

    movies.push_back(movie);

    addNotification("New movie released: " + movie.title + ".");
    greenColor();
    cout << "\nMovie added successfully.\n";
    pauseConsole();
}

void deleteMovie()
{
    printHeader("Admin - Delete movie");
    printAllMovies();
    cout << "\n";

    int movieId = readInt("Enter Movie ID to delete: ", 1, 1000000);

    auto it = remove_if(movies.begin(), movies.end(),
        [movieId](const Movie& m) { return m.id == movieId; });

    if (it == movies.end())
    {
        redColor();
        cout << "Movie not found.\n";
        pauseConsole();
        return;
    }

    shows.erase(remove_if(shows.begin(), shows.end(),
        [movieId](const Show& s) { return s.movieId == movieId; }),
        shows.end());

    movies.erase(it, movies.end());

    greenColor();
    cout << "Movie deleted successfully. Related shows were also removed.\n";
    pauseConsole();
}

void addShow()
{
    printHeader("Admin - Add show");

    if (movies.empty() || cinemas.empty())
    {
        redColor();
        cout << "Movies or cinemas are missing.\n";
        pauseConsole();
        return;
    }

    cout << "Movies:\n";
    printAllMovies();
    cout << "\n";

    int movieId = readInt("Enter Movie ID: ", 1, 1000000);
    if (!findMovieById(movieId))
    {
        redColor();
        cout << "Movie not found.\n";
        pauseConsole();
        return;
    }

    cout << "\nCinemas and halls:\n";
    for (const auto& cinema : cinemas)
    {
        whiteColor();
        cout << "Cinema ID: " << cinema.id << " | " << cinema.name << " | " << cinema.city << "\n";
        for (const auto& hall : cinema.halls)
        {
            cout << "   Hall ID: " << hall.id << " | " << hall.name << "\n";
        }
    }
    cout << "\n";

    int cinemaId = readInt("Enter Cinema ID: ", 1, 1000000);
    Cinema* cinema = findCinemaById(cinemaId);
    if (!cinema)
    {
        redColor();
        cout << "Cinema not found.\n";
        pauseConsole();
        return;
    }

    int hallId = readInt("Enter Hall ID: ", 1, 1000000);
    Hall* hall = findHallByIds(cinemaId, hallId);
    if (!hall)
    {
        redColor();
        cout << "Hall not found.\n";
        pauseConsole();
        return;
    }

    string date = readLine("Show date (YYYY-MM-DD): ");
    string time = readLine("Show time (HH:MM): ");

    if (hallIsBusyAtTime(cinemaId, hallId, date, time))
    {
        redColor();
        cout << "This hall already has a show at the selected date and time.\n";
        pauseConsole();
        return;
    }

    Show show;
    show.id = nextShowId++;
    show.movieId = movieId;
    show.cinemaId = cinemaId;
    show.hallId = hallId;
    show.showDate = date;
    show.showTime = time;
    show.seats = createDefaultSeats();

    shows.push_back(show);

    greenColor();
    cout << "Show added successfully.\n";
    pauseConsole();
}

void deleteShow()
{
    printHeader("Admin - Delete show");
    printAllShows();
    cout << "\n";

    int showId = readInt("Enter Show ID to delete: ", 1, 1000000);

    auto it = remove_if(shows.begin(), shows.end(),
        [showId](const Show& s) { return s.id == showId; });

    if (it == shows.end())
    {
        redColor();
        cout << "Show not found.\n";
        pauseConsole();
        return;
    }

    shows.erase(it, shows.end());

    greenColor();
    cout << "Show deleted successfully.\n";
    pauseConsole();
}

void updateShow()
{
    printHeader("Admin - Update show");
    printAllShows();
    cout << "\n";

    int showId = readInt("Enter Show ID to update: ", 1, 1000000);
    Show* show = findShowById(showId);

    if (!show)
    {
        redColor();
        cout << "Show not found.\n";
        pauseConsole();
        return;
    }

    string newDate = readLine("New date (current: " + show->showDate + "): ");
    string newTime = readLine("New time (current: " + show->showTime + "): ");

    if (hallIsBusyAtTime(show->cinemaId, show->hallId, newDate, newTime, show->id))
    {
        redColor();
        cout << "Conflict detected. Another show already exists in the same hall at that time.\n";
        pauseConsole();
        return;
    }

    show->showDate = newDate;
    show->showTime = newTime;

    greenColor();
    cout << "Show updated successfully.\n";
    pauseConsole();
}

void adminMenu()
{
    vector<string> adminOptions =
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
        int choice = selectMenu(adminOptions, "Admin menu");

        if (choice == 0) addShow();
        else if (choice == 1) deleteShow();
        else if (choice == 2) updateShow();
        else if (choice == 3) addMovie();
        else if (choice == 4) deleteMovie();
        else break;
    }
}

void listBookings()
{
    printHeader("All bookings");

    if (bookings.empty())
    {
        redColor();
        cout << "No bookings found.\n";
        pauseConsole();
        return;
    }

    for (const auto& booking : bookings)
    {
        Show* show = findShowById(booking.showId);
        Movie* movie = show ? findMovieById(show->movieId) : nullptr;

        whiteColor();
        cout << "Booking ID: " << booking.id
            << " | Customer: " << booking.customerName
            << " | Movie: " << (movie ? movie->title : "Unknown")
            << " | Seats: ";

        for (const auto& seat : booking.seatCodes)
            cout << seat << " ";

        cout << "| Channel: " << bookingChannelToString(booking.channel)
            << " | Payment: " << paymentMethodToString(booking.payment)
            << " | Total: $" << fixed << setprecision(2) << booking.totalAmount
            << " | Status: " << (booking.active ? "Active" : "Canceled") << "\n";
    }

    pauseConsole();
}

int main()
{
    srand((unsigned)time(nullptr));
    SetConsoleOutputCP(65001);
    seedData();

    vector<string> mainMenu =
    {
        "Search movies",
        "View showtimes",
        "Make booking",
        "Cancel booking",
        "View bookings",
        "View notifications",
        "Admin menu",
        "Exit"
    };

    while (true)
    {
        int choice = selectMenu(mainMenu, "Movie Ticket Booking System");

        if (choice == 0) searchMovies();
        else if (choice == 1) viewShowtimes();
        else if (choice == 2) makeBooking();
        else if (choice == 3) cancelBooking();
        else if (choice == 4) listBookings();
        else if (choice == 5) viewNotifications();
        else if (choice == 6) adminMenu();
        else if (choice == 7)
        {
            printHeader("System shutdown");
            whiteColor();
            cout << "Thank you for using Movie Ticket Booking System.\n";
            Sleep(1000);
            break;
        }
    }

}