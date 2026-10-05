#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include "raylib.h"
#include "raymath.h"
#include "Player.h"
#include "Background.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Boss.h"

enum class GameState {
    TITLE,
    PLAYING,
    PAUSED,
    GAMEOVER
};

struct Particle {
    Vector2 position;
    Vector2 velocity;
    Color color;
    float alpha;
    float size;
    float life;
    float maxLife;
};

class Game
{
private:
    GameState state;
    Player _player;
    vector<Bullet> bullets;
    vector<Bullet> bossBullets;
    vector<Enemy> enemies;
    vector<Texture2D> enemyTextures;
    vector<Particle> particles;
    Boss boss;

    Texture2D playerBulletTexture;
    Texture2D enemy_bullet_texture;
    Texture2D lower_background_texture;
    Texture2D upper_background_texture;
    Texture2D enemyTexture;
    Texture2D boss_texture;
    Texture2D boss_bullet_texture;
    Texture2D health_bar;
    Texture2D health;
    Texture2D gameover_Screen;

    Rectangle source_health;
    Rectangle dest_health;
    Background background;

    Font custom_font;
    Font pause_font;
    Sound Collision_sound;

    float current_boss_fire_time;
    float boss_fire_rate;
    const float backgroundSpeed = 7.0f;
    float enemySpawnrate;
    float currentTime;
    float bullet_fire_rate;
    float current_fire_time;
    int score;
    int high_score;
    int next_boss_score;
    float screenShake;

    void spawnEnemy();
    void spawnBoss();
    void updateParticles();
    void drawParticles();
    void drawHealthBar();
    void drawBossHealthBar();
    void drawTitleScreen();
    void drawPausedScreen();
    void drawGameOverScreen();

public:
    Music back_music;

    Game();
    ~Game();
    void reset();
    void update();
    void draw();
    void handleInput();
    void check_collision();
    Vector2 getPlayerPosition();
    Vector2 getPlayerPOstion(); // Legacy alias
    bool get_running_stat();
    int check_HighScore();
    int loadHighScore();
    void saveHighScore();
    void addExplosion(Vector2 pos, Color color, int count = 12);
    void triggerScreenShake(float intensity);
};

