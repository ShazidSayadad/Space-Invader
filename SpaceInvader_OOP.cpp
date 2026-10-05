#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <vector>
#include "Game.h"
using namespace std;

const int windowWidth = 800;
const int windowHeight = 800;

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(windowWidth, windowHeight, "Space Invaders: Galactic Defender");
    InitAudioDevice();
    SetTargetFPS(60);

    {
        Game game;
        while (!WindowShouldClose())
        {
            UpdateMusicStream(game.back_music);

            game.handleInput();
            game.update();

            BeginDrawing();
            ClearBackground(BLACK);
            game.draw();
            EndDrawing();
        }
    } // game destructor runs here safely before raylib context is closed

    CloseAudioDevice();
    CloseWindow();

    return 0;
}