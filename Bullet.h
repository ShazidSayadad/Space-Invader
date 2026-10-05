#pragma once
#include "raylib.h"
#include "raymath.h"
using namespace std;

class Bullet
{
private:
    Vector2 _position;
    Texture2D _texture;
    Rectangle source;
    Rectangle _des;
    float _scale = 0.05f;
    float _width;
    float _height;
    float _angle;
    float _speed = 15.0f;
    float _xSpeed;
    float _ySpeed;

public:
    Bullet();
    Bullet(Vector2, float, Texture2D);
    ~Bullet();
    void draw();
    void update();
    float getWidth();
    float getHeight();
    Vector2 getPosition();
    void MoveVertical(float);
    void MoveHorizontal(float);
    Rectangle get_rect_bullet();
    void moveWithBackground(float,float);
};

