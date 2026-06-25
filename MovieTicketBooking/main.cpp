#include "raylib.h"
#include <vector>
#include <string>
#include <fstream>
#include <cctype>

using namespace std;

const int W = 1000;
const int H = 700;
const int ROWS = 5;
const int COLS = 8;
const int MAX_SHOWS = 20;

enum Screen
{
    MENU,
    LOGIN,
    REGISTER,
    MOVIES,
    DETAILS,
    BOOKING,
    PAYMENT,
    CONFIRMATION,
    ADMIN,
    ADMIN_ADD_MOVIE,
    ADMIN_CHOOSE_EDIT_MOVIE,
    ADMIN_EDIT_MOVIE,
    ADMIN_DELETE_MOVIE,
    NOTIFICATIONS
};

struct User
{
    string username;
    string password;
};

struct Movie
{
    string title;
    string language;
    string genre;
    string releaseDate;
    string description;
    string posterPath;
    string actors;
    string duration;
    string ageRating;
    float rating;
    bool favorite;
};

struct Show
{
    int movieIndex;
    string cinema;
    string hall;
    string time;
};

struct Notification
{
    string text;
};

Texture2D menuBg;
Texture2D logo;
Texture2D buttonImg;
Texture2D buttonHover;
Texture2D pawCursor;

vector<User> users;
vector<Movie> movies;
vector<Show> shows;
vector<Texture2D> posters;
vector<Notification> notifications;

bool booked[MAX_SHOWS][ROWS][COLS] = {};
bool selected[ROWS][COLS] = {};

Screen screen = MENU;

int selectedMovie = -1;
int selectedShow = -1;
int editMovieIndex = -1;
int activeInput = 0;
int lastTotal = 0;

string currentUser = "";
string loginUser = "";
string loginPass = "";
string regUser = "";
string regPass = "";
string searchText = "";
string cardName = "";
string cardNumber = "";
string message = "";

string formTitle = "";
string formLanguage = "";
string formGenre = "";
string formDate = "";
string formDescription = "";
string formPoster = "";
string formActors = "";
string formDuration = "";
string formAge = "";
string formRating = "";

Color darkBg = { 38, 12, 23, 255 };
Color panel = { 82, 28, 45, 235 };
Color panelSoft = { 105, 42, 62, 230 };
Color red = { 153, 27, 30, 255 };
Color redHover = { 185, 55, 78, 255 };
Color cream = { 255, 242, 229, 255 };
Color softPink = { 255, 220, 228, 255 };

string Lower(string s)
{
    for (int i = 0; i < (int)s.size(); i++)
    {
        s[i] = (char)tolower((unsigned char)s[i]);
    }

    return s;
}

float ToFloat(string s)
{
    try
    {
        return stof(s);
    }
    catch (...)
    {
        return 4.0f;
    }
}

bool IsImageFile(string path)
{
    string p = Lower(path);

    return p.find(".png") != string::npos ||
        p.find(".jpg") != string::npos ||
        p.find(".jpeg") != string::npos;
}

void DrawWrappedText(string text, int x, int y, int maxWidth, int fontSize, Color color)
{
    string line = "";
    string word = "";
    int lineY = y;

    for (int i = 0; i <= (int)text.size(); i++)
    {
        char ch = i < (int)text.size() ? text[i] : ' ';

        if (ch == ' ')
        {
            string testLine = line.empty() ? word : line + " " + word;

            if (MeasureText(testLine.c_str(), fontSize) > maxWidth && !line.empty())
            {
                DrawText(line.c_str(), x, lineY, fontSize, color);
                lineY += fontSize + 6;
                line = word;
            }
            else
            {
                line = testLine;
            }

            word = "";
        }
        else
        {
            word += ch;
        }
    }

    if (!line.empty())
    {
        DrawText(line.c_str(), x, lineY, fontSize, color);
    }
}

bool Button(Rectangle rect, string text)
{
    Vector2 mouse = GetMousePosition();
    bool hover = CheckCollisionPointRec(mouse, rect);

    DrawRectangleRounded(rect, 0.25f, 12, hover ? redHover : red);

    int tw = MeasureText(text.c_str(), 21);

    DrawText(
        text.c_str(),
        (int)(rect.x + rect.width / 2 - tw / 2),
        (int)(rect.y + rect.height / 2 - 10),
        21,
        WHITE
    );

    return hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool ImageButton(Rectangle rect, string text)
{
    Vector2 mouse = GetMousePosition();
    bool hover = CheckCollisionPointRec(mouse, rect);

    Texture2D img = hover ? buttonHover : buttonImg;

    DrawTexturePro(
        img,
        { 0, 0, (float)img.width, (float)img.height },
        rect,
        { 0, 0 },
        0,
        WHITE
    );

    int tw = MeasureText(text.c_str(), 24);

    DrawText(
        text.c_str(),
        (int)(rect.x + rect.width / 2 - tw / 2 + 18),
        (int)(rect.y + 22),
        24,
        red
    );

    return hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void UpdateText(string& text, int limit)
{
    int key = GetCharPressed();

    while (key > 0)
    {
        if (key >= 32 && key <= 125 && (int)text.size() < limit)
        {
            text += (char)key;
        }

        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !text.empty())
    {
        text.pop_back();
    }
}

void InputBox(Rectangle rect, string label, string& value, int id, int limit = 40, bool password = false)
{
    DrawText(label.c_str(), (int)rect.x, (int)rect.y - 23, 18, cream);

    bool active = activeInput == id;
    bool hover = CheckCollisionPointRec(GetMousePosition(), rect);

    DrawRectangleRounded(rect, 0.12f, 8, active ? cream : softPink);
    DrawRectangleLinesEx(rect, 2, active || hover ? red : panelSoft);

    string shown = value;

    if (password)
    {
        shown = "";
        for (int i = 0; i < (int)value.size(); i++)
        {
            shown += "*";
        }
    }

    DrawText(shown.c_str(), (int)rect.x + 10, (int)rect.y + 11, 20, red);

    if (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        activeInput = id;
    }

    if (active)
    {
        UpdateText(value, limit);
    }
}

void DrawMenuBackground()
{
    DrawTexturePro(
        menuBg,
        { 0, 0, (float)menuBg.width, (float)menuBg.height },
        { 0, 0, W, H },
        { 0, 0 },
        0,
        WHITE
    );

    DrawRectangle(0, 0, W, H, Fade(BLACK, 0.18f));
}

void DrawCleanBackground()
{
    ClearBackground(darkBg);

    DrawRectangleGradientV(
        0,
        0,
        W,
        H,
        Color{ 94, 31, 52, 255 },
        Color{ 35, 10, 22, 255 }
    );

    DrawCircle(110, 120, 130, Fade(softPink, 0.08f));
    DrawCircle(865, 570, 145, Fade(softPink, 0.07f));

    DrawRectangleRounded(
        { 58, 50, 884, 600 },
        0.035f,
        12,
        Fade(WHITE, 0.055f)
    );
}

void DrawTitle(string text)
{
    int tw = MeasureText(text.c_str(), 38);
    DrawText(text.c_str(), W / 2 - tw / 2, 75, 38, cream);
}

void DrawStars(float rating, int x, int y)
{
    int fullStars = (int)rating;

    for (int i = 0; i < 5; i++)
    {
        Color c = i < fullStars ? Color{ 255, 205, 80, 255 } : Color{ 125, 80, 90, 255 };
        DrawText("*", x + i * 25, y, 30, c);
    }

    DrawText(TextFormat("%.1f/5", rating), x + 140, y + 7, 18, cream);
}

void DrawCat(float x, float y, Color color)
{
    DrawCircle((int)x + 25, (int)y + 28, 22, color);
    DrawTriangle({ x + 8, y + 15 }, { x + 18, y - 5 }, { x + 28, y + 15 }, color);
    DrawTriangle({ x + 22, y + 15 }, { x + 32, y - 5 }, { x + 42, y + 15 }, color);
    DrawCircle((int)x + 17, (int)y + 26, 3, BLACK);
    DrawCircle((int)x + 33, (int)y + 26, 3, BLACK);
    DrawCircle((int)x + 25, (int)y + 34, 2, BLACK);
}

Color SeatColor(int row)
{
    if (row < 2) return Color{ 190, 190, 190, 255 };
    if (row < 4) return Color{ 255, 205, 80, 255 };
    return Color{ 190, 120, 255, 255 };
}

int SeatPrice(int row)
{
    if (row < 2) return 7;
    if (row < 4) return 10;
    return 14;
}

void DrawFallbackPoster(Rectangle rect, string title)
{
    DrawRectangleRounded(rect, 0.06f, 10, Color{ 255, 235, 220, 255 });
    DrawRectangleLinesEx(rect, 3, red);

    DrawCircle(
        (int)(rect.x + rect.width / 2),
        (int)(rect.y + 80),
        45,
        Color{ 210, 120, 85, 255 }
    );

    DrawTriangle(
        { rect.x + rect.width / 2 - 35, rect.y + 52 },
        { rect.x + rect.width / 2 - 18, rect.y + 15 },
        { rect.x + rect.width / 2, rect.y + 52 },
        Color{ 210, 120, 85, 255 }
    );

    DrawTriangle(
        { rect.x + rect.width / 2, rect.y + 52 },
        { rect.x + rect.width / 2 + 18, rect.y + 15 },
        { rect.x + rect.width / 2 + 35, rect.y + 52 },
        Color{ 210, 120, 85, 255 }
    );

    DrawCircle((int)(rect.x + rect.width / 2 - 15), (int)(rect.y + 78), 4, BLACK);
    DrawCircle((int)(rect.x + rect.width / 2 + 15), (int)(rect.y + 78), 4, BLACK);

    DrawWrappedText(
        title,
        (int)rect.x + 18,
        (int)rect.y + (int)rect.height - 95,
        (int)rect.width - 36,
        22,
        red
    );

    DrawText("No poster yet", (int)rect.x + 45, (int)rect.y + (int)rect.height - 35, 18, Color{ 120, 35, 45, 255 });
}

void DrawPoster(Texture2D tex, Rectangle rect, string title)
{
    if (tex.id > 0)
    {
        DrawTexturePro(
            tex,
            { 0, 0, (float)tex.width, (float)tex.height },
            rect,
            { 0, 0 },
            0,
            WHITE
        );
    }
    else
    {
        DrawFallbackPoster(rect, title);
    }
}

Texture2D SafeLoadTexture(string path)
{
    Texture2D empty = {};

    if (path.empty())
    {
        return empty;
    }

    if (!FileExists(path.c_str()))
    {
        return empty;
    }

    return LoadTexture(path.c_str());
}

void LoadPosterForMovie(string path)
{
    posters.push_back(SafeLoadTexture(path));
}

void ReplacePoster(int index, string path)
{
    if (index < 0 || index >= (int)posters.size())
    {
        return;
    }

    if (posters[index].id > 0)
    {
        UnloadTexture(posters[index]);
    }

    posters[index] = SafeLoadTexture(path);
}

void AddNotification(string text)
{
    notifications.push_back({ text });
}

void SaveUsers()
{
    ofstream file("users.txt");

    for (int i = 0; i < (int)users.size(); i++)
    {
        file << users[i].username << " " << users[i].password << endl;
    }
}

void LoadUsers()
{
    ifstream file("users.txt");
    User u;

    while (file >> u.username >> u.password)
    {
        users.push_back(u);
    }
}

bool UserExists(string username)
{
    for (int i = 0; i < (int)users.size(); i++)
    {
        if (users[i].username == username)
        {
            return true;
        }
    }

    return false;
}

bool LoginCorrect(string username, string password)
{
    for (int i = 0; i < (int)users.size(); i++)
    {
        if (users[i].username == username && users[i].password == password)
        {
            return true;
        }
    }

    return false;
}

void ClearSelectedSeats()
{
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            selected[r][c] = false;
        }
    }
}

void ClearMovieForm()
{
    formTitle = "";
    formLanguage = "";
    formGenre = "";
    formDate = "";
    formDescription = "";
    formPoster = "";
    formActors = "";
    formDuration = "";
    formAge = "";
    formRating = "";
    message = "";
    activeInput = 0;
}

void LoadMovieToForm(int index)
{
    if (index < 0 || index >= (int)movies.size())
    {
        return;
    }

    formTitle = movies[index].title;
    formLanguage = movies[index].language;
    formGenre = movies[index].genre;
    formDate = movies[index].releaseDate;
    formDescription = movies[index].description;
    formPoster = movies[index].posterPath;
    formActors = movies[index].actors;
    formDuration = movies[index].duration;
    formAge = movies[index].ageRating;
    formRating = TextFormat("%.1f", movies[index].rating);

    message = "Drop a new poster image on the poster box.";
}

void LoadData()
{
    movies.push_back({ "The Aristocats", "English", "Animation", "1970", "Elegant cats go on a musical adventure through Paris.", "assets/aristocats.png", "Phil Harris, Eva Gabor", "1h 18m", "G", 4.4f, false });
    movies.push_back({ "Puss in Boots", "English", "Adventure", "2011", "A brave and charming cat goes on a heroic journey.", "assets/puss_in_boots.png", "Antonio Banderas, Salma Hayek", "1h 30m", "PG", 4.6f, false });
    movies.push_back({ "Garfield", "English", "Comedy", "2004", "A lazy orange cat who loves lasagna causes chaos.", "assets/garfield.png", "Bill Murray, Breckin Meyer", "1h 20m", "PG", 4.1f, false });
    movies.push_back({ "A Street Cat Named Bob", "English", "Drama", "2016", "A heartwarming story about a man and a street cat.", "assets/bob.png", "Luke Treadaway, Ruta Gedmintas", "1h 43m", "PG-13", 4.7f, false });

    shows.push_back({ 0, "CatCinema Burgas", "Hall 1", "12:00" });
    shows.push_back({ 0, "CatCinema Burgas", "Hall 2", "16:30" });
    shows.push_back({ 1, "CatCinema Burgas", "Hall 1", "18:00" });
    shows.push_back({ 2, "CatCinema Burgas", "Hall 3", "20:30" });
    shows.push_back({ 3, "CatCinema Burgas", "Hall 2", "21:00" });

    for (int i = 0; i < (int)movies.size(); i++)
    {
        LoadPosterForMovie(movies[i].posterPath);
    }

    booked[0][0][2] = true;
    booked[0][2][4] = true;
    booked[1][1][1] = true;
    booked[2][3][5] = true;
    booked[3][4][6] = true;

    AddNotification("New movie released: The Aristocats");
    AddNotification("New movie released: Puss in Boots");
}

bool MovieMatches(Movie movie)
{
    if (searchText.empty())
    {
        return true;
    }

    string all = movie.title + " " + movie.language + " " + movie.genre + " " + movie.releaseDate;

    return Lower(all).find(Lower(searchText)) != string::npos;
}

void HandlePosterDrop()
{
    if (!IsFileDropped())
    {
        return;
    }

    FilePathList droppedFiles = LoadDroppedFiles();

    if (droppedFiles.count > 0)
    {
        string path = droppedFiles.paths[0];

        if (IsImageFile(path))
        {
            formPoster = path;
            message = "Poster selected.";
        }
        else
        {
            message = "Please drop PNG, JPG or JPEG image.";
        }
    }

    UnloadDroppedFiles(droppedFiles);
}

void DrawPosterDropBox()
{
    Rectangle box = { 95, 145, 250, 290 };
    bool hover = CheckCollisionPointRec(GetMousePosition(), box);

    DrawRectangleRounded(box, 0.06f, 10, hover ? panelSoft : panel);
    DrawRectangleLinesEx(box, 3, hover ? cream : red);

    Texture2D preview = SafeLoadTexture(formPoster);

    if (preview.id > 0)
    {
        DrawTexturePro(
            preview,
            { 0, 0, (float)preview.width, (float)preview.height },
            { 115, 165, 210, 220 },
            { 0, 0 },
            0,
            WHITE
        );

        DrawText("Drop to replace", 130, 398, 18, cream);
        UnloadTexture(preview);
    }
    else
    {
        DrawText("Drag & drop", 140, 245, 24, cream);
        DrawText("poster here", 145, 275, 24, cream);
        DrawText(".png / .jpg / .jpeg", 135, 315, 18, softPink);
    }

    HandlePosterDrop();
}

void DrawMenu()
{
    DrawMenuBackground();

    DrawTexturePro(
        logo,
        { 0, 0, (float)logo.width, (float)logo.height },
        { 280, 25, 440, 260 },
        { 0, 0 },
        0,
        WHITE
    );

    if (currentUser.empty())
    {
        if (ImageButton({ 360, 360, 280, 70 }, "Login"))
        {
            screen = LOGIN;
        }
    }
    else
    {
        if (ImageButton({ 360, 285, 280, 70 }, "View Movies"))
        {
            screen = MOVIES;
        }

        if (ImageButton({ 360, 375, 280, 70 }, "Admin Panel"))
        {
            screen = ADMIN;
        }

        if (ImageButton({ 360, 465, 280, 70 }, "Notifications"))
        {
            screen = NOTIFICATIONS;
        }
    }

    if (ImageButton({ 360, 555, 280, 70 }, "Exit"))
    {
        CloseWindow();
    }

    if (!currentUser.empty())
    {
        DrawText(("Logged in as: " + currentUser).c_str(), 30, 30, 22, WHITE);
    }
}

void DrawLogin()
{
    DrawCleanBackground();
    DrawTitle("Login");

    InputBox({ 310, 200, 380, 52 }, "Username", loginUser, 1);
    InputBox({ 310, 300, 380, 52 }, "Password", loginPass, 2, 30, true);

    if (Button({ 370, 410, 260, 55 }, "Login"))
    {
        if (LoginCorrect(loginUser, loginPass))
        {
            currentUser = loginUser;
            loginUser = "";
            loginPass = "";
            message = "";
            screen = MOVIES;
        }
        else
        {
            message = "Wrong username or password.";
        }
    }

    DrawText(message.c_str(), 330, 560, 22, cream);

    if (Button({ 30, 610, 180, 55 }, "Back"))
    {
        screen = MENU;
    }
}

void DrawRegister()
{
    DrawCleanBackground();
    DrawTitle("Create Account");

    InputBox({ 310, 200, 380, 52 }, "New username", regUser, 3);
    InputBox({ 310, 300, 380, 52 }, "New password", regPass, 4, 30, true);

    if (Button({ 370, 410, 260, 55 }, "Register"))
    {
        if (regUser.empty() || regPass.empty())
        {
            message = "Fill all fields.";
        }
        else if (UserExists(regUser))
        {
            message = "Username already exists.";
        }
        else
        {
            users.push_back({ regUser, regPass });
            SaveUsers();

            currentUser = regUser;
            regUser = "";
            regPass = "";
            message = "";

            screen = MOVIES;
        }
    }

    DrawText(message.c_str(), 330, 510, 22, cream);

    if (Button({ 30, 610, 180, 55 }, "Back"))
    {
        screen = MENU;
    }
}

void DrawMovies()
{
    DrawCleanBackground();
    DrawTitle("Cat Movies");

    InputBox({ 260, 130, 480, 50 }, "Search by title, language, genre or date", searchText, 5);

    int x = 105;
    int y = 215;

    for (int i = 0; i < (int)movies.size(); i++)
    {
        if (!MovieMatches(movies[i]))
        {
            continue;
        }

        Rectangle card = { (float)x, (float)y, 185, 300 };
        bool hover = CheckCollisionPointRec(GetMousePosition(), card);

        DrawRectangleRounded(card, 0.08f, 10, hover ? panelSoft : panel);
        DrawRectangleLinesEx(card, 2, hover ? cream : red);

        Rectangle posterRect = { (float)x + 20, (float)y + 20, 145, 170 };

        if (i < (int)posters.size())
        {
            DrawPoster(posters[i], posterRect, movies[i].title);
        }

        DrawWrappedText(movies[i].title, x + 18, y + 205, 150, 20, cream);
        DrawStars(movies[i].rating, x + 18, y + 250);

        if (movies[i].favorite)
        {
            DrawText("?", x + 145, y + 205, 28, Color{ 255, 120, 150, 255 });
        }

        if (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            selectedMovie = i;
            screen = DETAILS;
        }

        x += 210;
    }

    if (Button({ 30, 610, 180, 55 }, "Back"))
    {
        screen = MENU;
    }
}

void DrawDetails()
{
    DrawCleanBackground();

    if (selectedMovie < 0 || selectedMovie >= (int)movies.size())
    {
        return;
    }

    Movie& m = movies[selectedMovie];

    DrawText(m.title.c_str(), 330, 72, 38, cream);

    Rectangle posterRect = { 105, 135, 270, 390 };

    if (selectedMovie < (int)posters.size())
    {
        DrawPoster(posters[selectedMovie], posterRect, m.title);
    }

    DrawRectangleRounded({ 410, 135, 470, 390 }, 0.06f, 10, panel);

    DrawStars(m.rating, 440, 160);

    DrawText(("Language: " + m.language).c_str(), 440, 220, 21, cream);
    DrawText(("Genre: " + m.genre).c_str(), 440, 250, 21, cream);
    DrawText(("Release: " + m.releaseDate).c_str(), 440, 280, 21, cream);
    DrawText(("Duration: " + m.duration).c_str(), 440, 310, 21, cream);
    DrawText(("Age rating: " + m.ageRating).c_str(), 440, 340, 21, cream);

    DrawText("Actors:", 440, 382, 21, softPink);
    DrawWrappedText(m.actors, 440, 412, 390, 18, cream);

    DrawText("Description:", 440, 465, 21, softPink);
    DrawWrappedText(m.description, 440, 493, 390, 18, cream);

    if (Button({ 105, 545, 170, 50 }, m.favorite ? "Unfavorite" : "Favorite"))
    {
        m.favorite = !m.favorite;
    }

    DrawText("Showtimes:", 675, 548, 22, cream);

    int sx = 675;
    int sy = 585;

    for (int i = 0; i < (int)shows.size(); i++)
    {
        if (shows[i].movieIndex == selectedMovie)
        {
            string showText = shows[i].hall + " | " + shows[i].time;

            if (Button({ (float)sx, (float)sy, 220, 45 }, showText))
            {
                selectedShow = i;
                ClearSelectedSeats();
                screen = BOOKING;
            }

            sy += 55;
        }
    }

    if (Button({ 30, 610, 180, 55 }, "Back"))
    {
        screen = MOVIES;
    }
}

void DrawBooking()
{
    DrawCleanBackground();

    if (selectedMovie < 0 || selectedShow < 0)
    {
        return;
    }

    DrawTitle("Choose Seats");

    DrawText("Silver - 7 EUR", 90, 145, 21, cream);
    DrawText("Gold - 10 EUR", 90, 175, 21, cream);
    DrawText("Platinum - 14 EUR", 90, 205, 21, cream);

    DrawText("Booked seats have cats.", 690, 145, 21, cream);

    DrawRectangleRounded({ 260, 165, 480, 35 }, 0.4f, 10, Color{ 210, 175, 160, 255 });
    DrawText("SCREEN", 460, 173, 20, red);

    int startX = 240;
    int startY = 245;
    int total = 0;

    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            Rectangle seat = { (float)(startX + c * 65), (float)(startY + r * 60), 48, 40 };

            DrawRectangleRounded(seat, 0.30f, 8, SeatColor(r));
            DrawRectangleLinesEx(seat, 2, Color{ 90, 25, 38, 255 });
            DrawRectangleRounded({ seat.x + 5, seat.y + 28, seat.width - 10, 14 }, 0.35f, 8, Fade(BLACK, 0.15f));

            if (booked[selectedShow][r][c])
            {
                DrawCat(seat.x, seat.y - 15, SeatColor(r));
            }
            else if (selected[r][c])
            {
                DrawCircle((int)seat.x + 24, (int)seat.y + 20, 14, cream);
                total += SeatPrice(r);
            }

            if (!booked[selectedShow][r][c] &&
                CheckCollisionPointRec(GetMousePosition(), seat) &&
                IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                selected[r][c] = !selected[r][c];
            }
        }
    }

    DrawText(("Total: " + to_string(total) + " EUR").c_str(), 410, 570, 30, cream);

    if (Button({ 365, 615, 270, 55 }, "Continue Payment"))
    {
        bool hasSeat = false;

        for (int r = 0; r < ROWS; r++)
        {
            for (int c = 0; c < COLS; c++)
            {
                if (selected[r][c])
                {
                    hasSeat = true;
                }
            }
        }

        if (hasSeat)
        {
            lastTotal = total;
            screen = PAYMENT;
        }
    }

    if (Button({ 30, 615, 180, 55 }, "Back"))
    {
        screen = DETAILS;
    }
}

void DrawPayment()
{
    DrawCleanBackground();
    DrawTitle("Payment");

    DrawRectangleRounded({ 280, 150, 440, 400 }, 0.06f, 10, panel);

    DrawText("Payment method: Credit Card", 330, 190, 23, cream);

    InputBox({ 330, 270, 340, 50 }, "Card holder", cardName, 6);
    InputBox({ 330, 370, 340, 50 }, "Card number", cardNumber, 7);

    DrawText(("Total: " + to_string(lastTotal) + " EUR").c_str(), 390, 455, 28, cream);

    if (Button({ 365, 580, 270, 55 }, "Pay Now"))
    {
        for (int r = 0; r < ROWS; r++)
        {
            for (int c = 0; c < COLS; c++)
            {
                if (selected[r][c])
                {
                    booked[selectedShow][r][c] = true;
                }
            }
        }

        AddNotification("Booking made for: " + movies[selectedMovie].title);

        cardName = "";
        cardNumber = "";

        screen = CONFIRMATION;
    }

    if (Button({ 30, 610, 180, 55 }, "Back"))
    {
        screen = BOOKING;
    }
}

void DrawConfirmation()
{
    DrawCleanBackground();
    DrawTitle("Booking Confirmed");

    DrawRectangleRounded({ 255, 165, 490, 350 }, 0.06f, 10, cream);

    DrawText("Email notification sent!", 340, 210, 30, red);
    DrawText(("To: " + currentUser + "@catcinema.com").c_str(), 310, 275, 22, Color{ 80, 30, 45, 255 });
    DrawText(("Movie: " + movies[selectedMovie].title).c_str(), 310, 315, 22, Color{ 80, 30, 45, 255 });
    DrawText(("Cinema: " + shows[selectedShow].cinema).c_str(), 310, 355, 22, Color{ 80, 30, 45, 255 });
    DrawText(("Hall: " + shows[selectedShow].hall).c_str(), 310, 395, 22, Color{ 80, 30, 45, 255 });
    DrawText(("Time: " + shows[selectedShow].time).c_str(), 310, 435, 22, Color{ 80, 30, 45, 255 });
    DrawText(("Paid: " + to_string(lastTotal) + " EUR").c_str(), 310, 475, 22, red);

    if (Button({ 365, 570, 270, 55 }, "Back to Movies"))
    {
        ClearSelectedSeats();
        screen = MOVIES;
    }
}

void DrawAdmin()
{
    DrawCleanBackground();
    DrawTitle("Admin Panel");

    DrawRectangleRounded({ 245, 145, 510, 340 }, 0.06f, 10, panel);

    if (Button({ 360, 190, 280, 55 }, "Add Movie"))
    {
        ClearMovieForm();
        screen = ADMIN_ADD_MOVIE;
    }

    if (Button({ 360, 270, 280, 55 }, "Edit Movie"))
    {
        editMovieIndex = -1;
        screen = ADMIN_CHOOSE_EDIT_MOVIE;
    }

    if (Button({ 360, 350, 280, 55 }, "Delete Movie"))
    {
        screen = ADMIN_DELETE_MOVIE;
    }

    DrawText("Admin panel manages movies only.", 325, 430, 21, cream);

    if (Button({ 30, 610, 180, 55 }, "Back"))
    {
        screen = MENU;
    }
}

void DrawMovieForm(bool editMode)
{
    DrawPosterDropBox();

    InputBox({ 395, 145, 220, 42 }, "Title", formTitle, 10);
    InputBox({ 395, 215, 220, 42 }, "Language", formLanguage, 11);
    InputBox({ 395, 285, 220, 42 }, "Genre", formGenre, 12);
    InputBox({ 395, 355, 220, 42 }, "Release date", formDate, 13);
    InputBox({ 395, 425, 220, 42 }, "Duration", formDuration, 14);
    InputBox({ 395, 495, 220, 42 }, "Age rating", formAge, 15);

    InputBox({ 650, 145, 250, 42 }, "Actors", formActors, 18, 100);
    InputBox({ 650, 215, 250, 42 }, "Description", formDescription, 19, 120);
    InputBox({ 650, 285, 120, 42 }, "Rating", formRating, 20, 5);

    DrawWrappedText("Poster: " + formPoster, 650, 355, 260, 16, cream);

    string buttonText = editMode ? "Save Changes" : "Save Movie";

    if (Button({ 650, 515, 210, 50 }, buttonText))
    {
        if (formTitle.empty())
        {
            message = "Title is required.";
            return;
        }

        if (formRating.empty())
        {
            formRating = "4.0";
        }

        if (editMode && editMovieIndex >= 0 && editMovieIndex < (int)movies.size())
        {
            bool oldFavorite = movies[editMovieIndex].favorite;

            movies[editMovieIndex] = {
                formTitle,
                formLanguage,
                formGenre,
                formDate,
                formDescription,
                formPoster,
                formActors,
                formDuration,
                formAge,
                ToFloat(formRating),
                oldFavorite
            };

            ReplacePoster(editMovieIndex, formPoster);
            AddNotification("Movie edited: " + formTitle);
        }
        else
        {
            movies.push_back({
                formTitle,
                formLanguage,
                formGenre,
                formDate,
                formDescription,
                formPoster,
                formActors,
                formDuration,
                formAge,
                ToFloat(formRating),
                false
                });

            LoadPosterForMovie(formPoster);
            AddNotification("New movie released: " + formTitle);
        }

        ClearMovieForm();
        editMovieIndex = -1;
        screen = ADMIN;
    }

    DrawText(message.c_str(), 395, 585, 18, cream);

    if (Button({ 30, 610, 180, 55 }, "Back"))
    {
        ClearMovieForm();
        editMovieIndex = -1;
        screen = ADMIN;
    }
}

void DrawAdminAddMovie()
{
    DrawCleanBackground();
    DrawTitle("Add Movie");
    DrawMovieForm(false);
}

void DrawAdminChooseEditMovie()
{
    DrawCleanBackground();
    DrawTitle("Choose Movie To Edit");

    int y = 145;

    for (int i = 0; i < (int)movies.size(); i++)
    {
        DrawText((to_string(i) + ". " + movies[i].title).c_str(), 150, y + 12, 22, cream);

        if (Button({ 650, (float)y, 160, 45 }, "Edit"))
        {
            editMovieIndex = i;
            LoadMovieToForm(i);
            screen = ADMIN_EDIT_MOVIE;
        }

        y += 60;
    }

    if (Button({ 30, 610, 180, 55 }, "Back"))
    {
        screen = ADMIN;
    }
}

void DrawAdminEditMovie()
{
    DrawCleanBackground();
    DrawTitle("Edit Movie");
    DrawMovieForm(true);
}

void DrawAdminDeleteMovie()
{
    DrawCleanBackground();
    DrawTitle("Delete Movie");

    int y = 150;

    for (int i = 0; i < (int)movies.size(); i++)
    {
        DrawText((to_string(i) + ". " + movies[i].title).c_str(), 160, y + 12, 22, cream);

        if (Button({ 650, (float)y, 160, 45 }, "Delete"))
        {
            AddNotification("Movie deleted: " + movies[i].title);

            movies.erase(movies.begin() + i);

            if (i < (int)posters.size())
            {
                if (posters[i].id > 0)
                {
                    UnloadTexture(posters[i]);
                }

                posters.erase(posters.begin() + i);
            }

            for (int s = 0; s < (int)shows.size(); s++)
            {
                if (shows[s].movieIndex == i)
                {
                    shows.erase(shows.begin() + s);
                    s--;
                }
                else if (shows[s].movieIndex > i)
                {
                    shows[s].movieIndex--;
                }
            }

            break;
        }

        y += 60;
    }

    if (Button({ 30, 610, 180, 55 }, "Back"))
    {
        screen = ADMIN;
    }
}

void DrawNotifications()
{
    DrawCleanBackground();
    DrawTitle("Notifications");

    int y = 150;

    for (int i = (int)notifications.size() - 1; i >= 0; i--)
    {
        DrawRectangleRounded({ 170, (float)y, 660, 50 }, 0.15f, 8, panel);
        DrawText(notifications[i].text.c_str(), 195, y + 15, 20, cream);

        y += 65;

        if (y > 550)
        {
            break;
        }
    }

    if (Button({ 30, 610, 180, 55 }, "Back"))
    {
        screen = MENU;
    }
}

int main()
{
    InitWindow(W, H, "CatCinema");
    SetTargetFPS(60);
    HideCursor();

    menuBg = LoadTexture("assets/menu_background.png");
    logo = LoadTexture("assets/logo.png");
    buttonImg = LoadTexture("assets/button.png");
    buttonHover = LoadTexture("assets/button_hover.png");
    pawCursor = LoadTexture("assets/paw_cursor.png");

    LoadUsers();
    LoadData();

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        if (screen == MENU) DrawMenu();
        else if (screen == LOGIN) DrawLogin();
        else if (screen == REGISTER) DrawRegister();
        else if (screen == MOVIES) DrawMovies();
        else if (screen == DETAILS) DrawDetails();
        else if (screen == BOOKING) DrawBooking();
        else if (screen == PAYMENT) DrawPayment();
        else if (screen == CONFIRMATION) DrawConfirmation();
        else if (screen == ADMIN) DrawAdmin();
        else if (screen == ADMIN_ADD_MOVIE) DrawAdminAddMovie();
        else if (screen == ADMIN_CHOOSE_EDIT_MOVIE) DrawAdminChooseEditMovie();
        else if (screen == ADMIN_EDIT_MOVIE) DrawAdminEditMovie();
        else if (screen == ADMIN_DELETE_MOVIE) DrawAdminDeleteMovie();
        else if (screen == NOTIFICATIONS) DrawNotifications();

        Vector2 mouse = GetMousePosition();

        DrawTextureEx(
            pawCursor,
            mouse,
            0,
            0.03f,
            WHITE
        );

        EndDrawing();
    }

    for (int i = 0; i < (int)posters.size(); i++)
    {
        if (posters[i].id > 0)
        {
            UnloadTexture(posters[i]);
        }
    }

    UnloadTexture(menuBg);
    UnloadTexture(logo);
    UnloadTexture(buttonImg);
    UnloadTexture(buttonHover);
    UnloadTexture(pawCursor);

    CloseWindow();

    return 0;
}
