#pragma once
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "Bullet.h"
#include <vector>
using namespace std;

class Enemy
{
protected:
    Vector2 _position;
    Texture2D _texture;
    Texture2D _Bullettexture;
    float speed;
    Vector2 direction;
    Rectangle source;
    Rectangle destination;
    float angle;
    float distance;
    float width;
    float height;
    float scale;
    float fireRate;
    Vector2 origin;
    float current = 0;
    vector <Bullet> bullets;
    float _distance;

public:
    Enemy() {}
    Enemy(Texture2D, int, int, Texture2D);
    virtual ~Enemy() {}
    virtual void Update(Vector2);
    virtual void Draw();
    void MoveHorizontal(float);
    void MoveVertical(float);
    Vector2 getPosition();
    float getAngle();
    float getWidth();
    float getHeight();
    vector<Bullet>& getEnemyBullets();
    Rectangle get_rect_enemy();
};

