#pragma once
#include "raylib.h"
#include "raymath.h"
using namespace std;

class Player
{
private:
    float health;
    Vector2 _position;
    Vector2 _origin;
    Texture2D _ship;
    double _angle;
    float _width;
    float _height;
    Rectangle _source;
    Rectangle _destination;
    const float _scale = 4;
    void calculateAngle();

public:
    Player();
    ~Player();
    void draw();
    void update();
    double getAngle();
    Vector2 getPosition();
    Rectangle get_rect_player();
    float getHealth();
    void takeDamage(float);
    void reset();

};

