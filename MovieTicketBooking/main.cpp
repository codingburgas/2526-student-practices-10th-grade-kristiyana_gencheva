#include "raylib.h"

enum Screen
{
    MENU,
    MOVIES,
    ADMIN
};

bool Button(Rectangle rect, const char* text)
{
    Vector2 mouse = GetMousePosition();
    bool hover = CheckCollisionPointRec(mouse, rect);

    DrawRectangleRec(rect, hover ? LIGHTGRAY : GRAY);
    DrawRectangleLinesEx(rect, 2, DARKGRAY);

    DrawText(text, rect.x + 45, rect.y + 13, 20, BLACK);

    return hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

int main()
{
    InitWindow(1000, 700, "CatCinema");
    SetTargetFPS(60);

    Screen currentScreen = MENU;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (currentScreen == MENU)
        {
            DrawText("CatCinema", 370, 120, 50, DARKPURPLE);
            DrawText("Movie Ticket Booking System", 330, 180, 22, DARKGRAY);

            if (Button({ 380, 280, 240, 50 }, "View Movies"))
            {
                currentScreen = MOVIES;
            }

            if (Button({ 380, 350, 240, 50 }, "Admin Panel"))
            {
                currentScreen = ADMIN;
            }

            if (Button({ 380, 420, 240, 50 }, "Exit"))
            {
                CloseWindow();
                return 0;
            }
        }
        else if (currentScreen == MOVIES)
        {
            DrawText("Movies screen", 390, 250, 35, BLACK);

            if (Button({ 30, 620, 140, 45 }, "Back"))
            {
                currentScreen = MENU;
            }
        }
        else if (currentScreen == ADMIN)
        {
            DrawText("Admin screen", 400, 250, 35, BLACK);

            if (Button({ 30, 620, 140, 45 }, "Back"))
            {
                currentScreen = MENU;
            }
        }

        EndDrawing();
    }

    CloseWindow();
 
}