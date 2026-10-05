#pragma once
#include "raylib.h"
#include "Bullet.h"
#include "Enemy.h"
#include <vector>

class Boss : public Enemy
{
private:
    int _health;
    int _maxHealth;
    bool isAlive;
    bool isKilled;

public:
    Boss();
    Boss(Vector2 pos, Texture2D texture, Texture2D bullet);
    void Update(Vector2 playerPosition) override;
    void Draw() override;
    int getHealth();
    int getMaxHealth();
    void getDamage(int damage);
    bool isBossAlive();
    void setAlive();
    void spawn(Vector2 pos, int maxHp = 30);
    bool isBossKilled();
    void setBossKilled();
    void reset();
    Rectangle boss_get_rect();
    vector<Bullet>& getBossBullets();
    Vector2 getPosition();
    float getAngle();
};

