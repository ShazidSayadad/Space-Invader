#include "Background.h"

Background::Background() 
{
    mainTexturePosition = { 0, 0 };
    upperTexturePosition = { 0, 0 };
    width = 0;
    height = 0;
    parallexStrength = 0.3f;
    scale = 3.0f;
}

Background::Background(Texture2D mainTexture, Texture2D upperTexture)
{
    _mainTexture = mainTexture;
    _upperTexture = upperTexture;
    mainTexturePosition = { 0, 0 };
    upperTexturePosition = { 0, 0 };
    width = (float)_mainTexture.width;
    height = (float)_mainTexture.height;
    parallexStrength = 0.3f;
    scale = 3.0f;
}

void Background::draw()
{
    float tileSize = width * scale;
    if (tileSize <= 0) return;

    int tilesX = (int)(GetScreenWidth() / tileSize) + 3;
    int tilesY = (int)(GetScreenHeight() / tileSize) + 3;

    float mainOffsetX = fmodf(mainTexturePosition.x, tileSize);
    if (mainOffsetX > 0) mainOffsetX -= tileSize;
    float mainOffsetY = fmodf(mainTexturePosition.y, tileSize);
    if (mainOffsetY > 0) mainOffsetY -= tileSize;

    float upperOffsetX = fmodf(upperTexturePosition.x, tileSize);
    if (upperOffsetX > 0) upperOffsetX -= tileSize;
    float upperOffsetY = fmodf(upperTexturePosition.y, tileSize);
    if (upperOffsetY > 0) upperOffsetY -= tileSize;

    for (int i = 0; i < tilesY; i++)
    {
        for (int j = 0; j < tilesX; j++)
        {
            DrawTextureEx(_mainTexture, { mainOffsetX + j * tileSize, mainOffsetY + i * tileSize }, 0, scale, WHITE);
            DrawTextureEx(_upperTexture, { upperOffsetX + j * tileSize, upperOffsetY + i * tileSize }, 0, scale, ColorAlpha(WHITE, 0.75f));
        }
    }
}

void Background::xUpdateMain(float speed)
{
    mainTexturePosition.x += speed;
    upperTexturePosition.x += speed * (1.0f - parallexStrength);
}

void Background::yUpdateMain(float speed)
{
    mainTexturePosition.y += speed;
    upperTexturePosition.y += speed * (1.0f - parallexStrength);
}

void Background::check_bound()
{
}

void Background::xUpdateUpper(float speed)
{
    upperTexturePosition.x += speed;
}
   
void Background::yUpdateUpper(float speed)
{
    upperTexturePosition.y += speed;
}

Background::~Background()
{
}
