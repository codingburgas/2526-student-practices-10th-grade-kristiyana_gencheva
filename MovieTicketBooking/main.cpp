#include "raylib.h"

enum Screen
{
    MENU,
    MOVIES,
    ADMIN
};

Texture2D background;
Texture2D logo;
Texture2D button;
Texture2D buttonHover;
Texture2D pawCursor;

bool ImageButton(Rectangle rect, const char* text)
{
    Vector2 mouse = GetMousePosition();
    bool hover = CheckCollisionPointRec(mouse, rect);

    Texture2D currentButton = hover ? buttonHover : button;

    DrawTexturePro(
        currentButton,
        { 0, 0, (float)currentButton.width, (float)currentButton.height },
        rect,
        { 0, 0 },
        0,
        WHITE
    );

    int textWidth = MeasureText(text, 24);

    DrawText(
        text,
        rect.x + rect.width / 2 - textWidth / 2 + 18,
        rect.y + 22,
        24,
        Color{ 153, 27, 30, 255 }
    );

    return hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

int main()
{
    InitWindow(1000, 700, "CatCinema");
    SetTargetFPS(60);

    background = LoadTexture("assets/menu_background.png");
    logo = LoadTexture("assets/logo.png");
    button = LoadTexture("assets/button.png");
    buttonHover = LoadTexture("assets/button_hover.png");
    pawCursor = LoadTexture("assets/paw_cursor.png");

    HideCursor();

    Screen currentScreen = MENU;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        DrawTexturePro(
            background,
            { 0, 0, (float)background.width, (float)background.height },
            { 0, 0, 1000, 700 },
            { 0, 0 },
            0,
            WHITE
        );

        DrawRectangle(0, 0, 1000, 700, Fade(BLACK, 0.18f));

        if (currentScreen == MENU)
        {
            DrawTexturePro(
                logo,
                { 0, 0, (float)logo.width, (float)logo.height },
                { 280, 25, 440, 260 },
                { 0, 0 },
                0,
                WHITE
            );

            if (ImageButton({ 360, 315, 280, 70 }, "View Movies"))
            {
                currentScreen = MOVIES;
            }

            if (ImageButton({ 360, 405, 280, 70 }, "Admin Panel"))
            {
                currentScreen = ADMIN;
            }

            if (ImageButton({ 360, 495, 280, 70 }, "Exit"))
            {
                break;
            }
        }
        else if (currentScreen == MOVIES)
        {
            DrawText("Movies screen coming soon...", 270, 320, 32, WHITE);

            if (ImageButton({ 30, 610, 220, 60 }, "Back"))
            {
                currentScreen = MENU;
            }
        }
        else if (currentScreen == ADMIN)
        {
            DrawText("Admin panel coming soon...", 290, 320, 32, WHITE);

            if (ImageButton({ 30, 610, 220, 60 }, "Back"))
            {
                currentScreen = MENU;
            }
        }

        Vector2 mousePos = GetMousePosition();

        DrawTextureEx(
            pawCursor,
            mousePos,
            0,
            0.03f,
            WHITE
        );

        EndDrawing();
    }

    UnloadTexture(background);
    UnloadTexture(logo);
    UnloadTexture(button);
    UnloadTexture(buttonHover);
    UnloadTexture(pawCursor);

    CloseWindow();

}