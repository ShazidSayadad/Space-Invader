#pragma once
#include "raylib.h"
#include "raymath.h"
using namespace std;

class Background
{
private:
    Vector2 mainTexturePosition;
    Vector2 upperTexturePosition;
    Texture2D _mainTexture;
    Texture2D _upperTexture;
    float scale;
    float parallexStrength;
    float width;
    float height;
public:
    Background();
    Background(Texture2D, Texture2D);
    ~Background();
    void draw();
    void xUpdateMain(float);
    void yUpdateMain(float);
    void check_bound();
    void xUpdateUpper(float);
    void yUpdateUpper(float);
};

