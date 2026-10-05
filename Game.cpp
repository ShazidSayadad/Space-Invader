#include "Game.h"

Game::Game()
{
    playerBulletTexture = LoadTexture("assets/projectiles/projectile2.png");
    enemy_bullet_texture = LoadTexture("assets/projectiles/projectile3.png");
    lower_background_texture = LoadTexture("assets/background1.png");
    upper_background_texture = LoadTexture("assets/background4.png");
    back_music = LoadMusicStream("assets/videoplayback.wav");
    Collision_sound = LoadSound("assets/shoot.flac");
    health_bar = LoadTexture("assets/healthBar.png");
    health = LoadTexture("assets/health.png");
    gameover_Screen = LoadTexture("assets/Gameover.png");
    enemyTexture = LoadTexture("assets/spaceShip/ships/brown.png");
    boss_texture = LoadTexture("assets/spaceShip/ships/green.png");
    boss_bullet_texture = LoadTexture("assets/projectiles/projectile4.png");

    enemyTextures.push_back(enemyTexture);
    enemyTextures.push_back(LoadTexture("assets/spaceShip/ships/Blue.png"));
    enemyTextures.push_back(LoadTexture("assets/spaceShip/ships/Gray1.png"));
    enemyTextures.push_back(LoadTexture("assets/spaceShip/ships/purple.png"));

    custom_font = LoadFontEx("assets/CommodorePixeled.ttf", 30, NULL, 0);
    pause_font = LoadFontEx("assets/Game Paused DEMO.ttf", 48, NULL, 0);

    background = Background(lower_background_texture, upper_background_texture);

    source_health = { 0, 0, (float)health.width, (float)health.height };
    dest_health = { (float)(GetScreenWidth() - health_bar.width * 2.5f - 10), 10, (float)health.width * 2.5f, (float)health.height * 2.2f };

    boss_fire_rate = 0.35f;
    current_boss_fire_time = 0;
    enemySpawnrate = 2.5f;
    bullet_fire_rate = 0.14f;
    current_fire_time = 0;
    currentTime = (float)GetTime();
    score = 0;
    next_boss_score = 10;
    screenShake = 0.0f;
    state = GameState::TITLE;

    boss = Boss({ (float)GetScreenWidth() / 2.0f, -2000.0f }, boss_texture, boss_bullet_texture);

    high_score = loadHighScore();
    PlayMusicStream(back_music);
}

int Game::loadHighScore()
{
    high_score = 0;
    ifstream infile("highscore.txt");
    if (infile.is_open())
    {
        infile >> high_score;
        infile.close();
    }
    return high_score;
}

void Game::saveHighScore()
{
    if (score > high_score)
    {
        high_score = score;
    }
    ofstream file("highscore.txt");
    if (file.is_open())
    {
        file << high_score;
        file.close();
    }
}

int Game::check_HighScore()
{
    return high_score;
}

void Game::reset()
{
    _player.reset();
    bullets.clear();
    bossBullets.clear();
    enemies.clear();
    particles.clear();
    boss.reset();
    score = 0;
    next_boss_score = 10;
    enemySpawnrate = 2.5f;
    currentTime = (float)GetTime();
    current_fire_time = (float)GetTime();
    current_boss_fire_time = (float)GetTime();
    screenShake = 0.0f;
    state = GameState::PLAYING;

    // Spawn initial wave of 2 enemies
    spawnEnemy();
    spawnEnemy();
}

void Game::spawnEnemy()
{
    float angle = (float)GetRandomValue(0, 360) * (PI / 180.0f);
    float distance = (float)GetRandomValue(600, 750);
    Vector2 pPos = _player.getPosition();
    int x = (int)(pPos.x + cosf(angle) * distance);
    int y = (int)(pPos.y + sinf(angle) * distance);

    Texture2D tex = enemyTexture;
    if (!enemyTextures.empty())
    {
        int idx = GetRandomValue(0, (int)enemyTextures.size() - 1);
        tex = enemyTextures[idx];
    }
    enemies.push_back(Enemy(tex, x, y, enemy_bullet_texture));
}

void Game::spawnBoss()
{
    Vector2 pPos = _player.getPosition();
    int bossHp = 30 + (score / 10) * 10;
    boss.spawn({ pPos.x, pPos.y - 700.0f }, bossHp);
    triggerScreenShake(6.0f);
}

void Game::triggerScreenShake(float intensity)
{
    screenShake = intensity;
}

void Game::addExplosion(Vector2 pos, Color color, int count)
{
    for (int i = 0; i < count; i++)
    {
        float angle = (float)GetRandomValue(0, 360) * (PI / 180.0f);
        float spd = (float)GetRandomValue(40, 220) / 60.0f;
        Particle p;
        p.position = pos;
        p.velocity = { cosf(angle) * spd, sinf(angle) * spd };
        p.color = color;
        p.alpha = 1.0f;
        p.size = (float)GetRandomValue(3, 7);
        p.maxLife = (float)GetRandomValue(15, 35) / 60.0f;
        p.life = p.maxLife;
        particles.push_back(p);
    }
}

void Game::updateParticles()
{
    for (auto it = particles.begin(); it != particles.end(); )
    {
        it->position.x += it->velocity.x;
        it->position.y += it->velocity.y;
        it->velocity.x *= 0.94f;
        it->velocity.y *= 0.94f;
        it->life -= 1.0f / 60.0f;
        it->alpha = it->life / it->maxLife;
        if (it->life <= 0.0f)
        {
            it = particles.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void Game::drawParticles()
{
    for (auto& p : particles)
    {
        DrawCircleV(p.position, p.size * p.alpha, ColorAlpha(p.color, p.alpha));
    }
}

void Game::handleInput()
{
    if (state == GameState::TITLE)
    {
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
        {
            reset();
        }
        return;
    }

    if (state == GameState::PAUSED)
    {
        if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE))
        {
            state = GameState::PLAYING;
        }
        if (IsKeyPressed(KEY_R))
        {
            reset();
        }
        return;
    }

    if (state == GameState::GAMEOVER)
    {
        if (IsKeyPressed(KEY_R))
        {
            reset();
        }
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
        {
            state = GameState::TITLE;
        }
        return;
    }

    // PLAYING State Input
    if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE))
    {
        state = GameState::PAUSED;
        return;
    }

    // Continuous Autofire on Left Mouse Button hold
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        if ((float)GetTime() - current_fire_time >= bullet_fire_rate)
        {
            bullets.push_back(Bullet(_player.getPosition(), (float)_player.getAngle(), playerBulletTexture));
            current_fire_time = (float)GetTime();
            SetSoundPitch(Collision_sound, 1.25f);
            PlaySound(Collision_sound);
        }
    }

    // Normalized WASD World Movement
    Vector2 moveDir = { 0, 0 };
    if (IsKeyDown(KEY_A)) moveDir.x += 1.0f;
    if (IsKeyDown(KEY_D)) moveDir.x -= 1.0f;
    if (IsKeyDown(KEY_W)) moveDir.y += 1.0f;
    if (IsKeyDown(KEY_S)) moveDir.y -= 1.0f;

    if (Vector2Length(moveDir) > 0.0f)
    {
        moveDir = Vector2Normalize(moveDir);
        float dx = moveDir.x * backgroundSpeed;
        float dy = moveDir.y * backgroundSpeed;

        background.xUpdateMain(dx);
        background.yUpdateMain(dy);

        for (auto& bullet : bullets)
            bullet.moveWithBackground(dx, dy);

        for (auto& enemy : enemies)
        {
            enemy.MoveHorizontal(dx);
            enemy.MoveVertical(dy);
            for (auto& eb : enemy.getEnemyBullets())
                eb.moveWithBackground(dx, dy);
        }

        if (boss.isBossAlive())
        {
            boss.MoveHorizontal(dx);
            boss.MoveVertical(dy);
        }

        for (auto& bb : bossBullets)
            bb.moveWithBackground(dx, dy);

        for (auto& p : particles)
        {
            p.position.x += dx;
            p.position.y += dy;
        }
    }
}

void Game::update()
{
    if (state != GameState::PLAYING)
    {
        return;
    }

    _player.update();

    if (_player.getHealth() <= 0.0f)
    {
        state = GameState::GAMEOVER;
        saveHighScore();
        addExplosion(_player.getPosition(), RED, 35);
        triggerScreenShake(12.0f);
        return;
    }

    // Update Player Bullets
    for (auto it = bullets.begin(); it != bullets.end(); )
    {
        it->update();
        if (Vector2Distance(it->getPosition(), _player.getPosition()) > 900)
            it = bullets.erase(it);
        else
            it++;
    }

    // Update Enemies & Enemy Bullets
    for (auto& enemy : enemies)
    {
        enemy.Update(_player.getPosition());
        for (auto it = enemy.getEnemyBullets().begin(); it != enemy.getEnemyBullets().end(); )
        {
            it->update();
            if (Vector2Distance(it->getPosition(), _player.getPosition()) > 900)
                it = enemy.getEnemyBullets().erase(it);
            else
                it++;
        }
    }

    // Update Boss & Boss Bullets
    if (boss.isBossAlive())
    {
        boss.Update(_player.getPosition());

        for (auto it = bossBullets.begin(); it != bossBullets.end(); )
        {
            it->update();
            if (Vector2Distance(it->getPosition(), _player.getPosition()) > 950)
                it = bossBullets.erase(it);
            else
                it++;
        }

        if ((float)GetTime() - current_boss_fire_time > boss_fire_rate)
        {
            bossBullets.push_back(Bullet(boss.getPosition(), boss.getAngle(), boss_bullet_texture));
            current_boss_fire_time = (float)GetTime();
        }
    }

    // Dynamic Enemy Spawning
    float dynamicRate = fmaxf(1.0f, enemySpawnrate - (score * 0.04f));
    if ((float)GetTime() - currentTime > dynamicRate)
    {
        if (enemies.size() < 10)
        {
            spawnEnemy();
        }
        currentTime = (float)GetTime();
    }

    // Screen Shake decay
    if (screenShake > 0.0f)
    {
        screenShake -= 0.35f;
        if (screenShake < 0.0f) screenShake = 0.0f;
    }

    updateParticles();
    check_collision();
}

void Game::check_collision()
{
    // 1. Player Bullets vs Boss
    if (boss.isBossAlive())
    {
        for (auto it = bullets.begin(); it != bullets.end(); )
        {
            if (CheckCollisionRecs(it->get_rect_bullet(), boss.boss_get_rect()))
            {
                addExplosion(it->getPosition(), ORANGE, 6);
                boss.getDamage(2);
                SetSoundPitch(Collision_sound, 0.7f);
                PlaySound(Collision_sound);
                it = bullets.erase(it);

                if (boss.isBossKilled())
                {
                    addExplosion(boss.getPosition(), GOLD, 35);
                    score += 15;
                    if (score > high_score) high_score = score;
                    next_boss_score = score + 25;
                    boss.setBossKilled();
                    triggerScreenShake(9.0f);
                }
            }
            else
            {
                it++;
            }
        }
    }

    // 2. Boss Bullets vs Player
    if (boss.isBossAlive())
    {
        for (auto it = bossBullets.begin(); it != bossBullets.end(); )
        {
            if (CheckCollisionRecs(it->get_rect_bullet(), _player.get_rect_player()))
            {
                addExplosion(it->getPosition(), RED, 8);
                _player.takeDamage(10.0f);
                triggerScreenShake(5.0f);
                SetSoundPitch(Collision_sound, 0.5f);
                PlaySound(Collision_sound);
                it = bossBullets.erase(it);
            }
            else
            {
                it++;
            }
        }
    }

    // 3. Player vs Boss Body Collision
    if (boss.isBossAlive() && CheckCollisionRecs(_player.get_rect_player(), boss.boss_get_rect()))
    {
        _player.takeDamage(0.4f);
        triggerScreenShake(2.5f);
    }

    // 4. Player Bullets vs Enemies
    for (auto bIt = bullets.begin(); bIt != bullets.end(); )
    {
        bool bulletHit = false;
        for (auto eIt = enemies.begin(); eIt != enemies.end(); )
        {
            if (CheckCollisionRecs(bIt->get_rect_bullet(), eIt->get_rect_enemy()))
            {
                addExplosion(eIt->getPosition(), SKYBLUE, 14);
                SetSoundPitch(Collision_sound, 0.8f);
                PlaySound(Collision_sound);
                score++;
                if (score > high_score) high_score = score;

                if (score >= next_boss_score && !boss.isBossAlive())
                {
                    spawnBoss();
                }

                eIt = enemies.erase(eIt);
                bulletHit = true;
                break;
            }
            else
            {
                eIt++;
            }
        }

        if (bulletHit)
        {
            bIt = bullets.erase(bIt);
        }
        else
        {
            bIt++;
        }
    }

    // 5. Enemy Bullets vs Player
    for (auto& enemy : enemies)
    {
        for (auto it = enemy.getEnemyBullets().begin(); it != enemy.getEnemyBullets().end(); )
        {
            if (CheckCollisionRecs(it->get_rect_bullet(), _player.get_rect_player()))
            {
                addExplosion(it->getPosition(), RED, 6);
                _player.takeDamage(5.0f);
                triggerScreenShake(3.5f);
                SetSoundPitch(Collision_sound, 0.6f);
                PlaySound(Collision_sound);
                it = enemy.getEnemyBullets().erase(it);
            }
            else
            {
                it++;
            }
        }
    }

    // 6. Enemies vs Player Body Collision
    for (auto it = enemies.begin(); it != enemies.end(); )
    {
        if (CheckCollisionRecs(it->get_rect_enemy(), _player.get_rect_player()))
        {
            addExplosion(it->getPosition(), RED, 16);
            _player.takeDamage(12.0f);
            triggerScreenShake(6.0f);
            it = enemies.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void Game::drawHealthBar()
{
    float healthPercent = _player.getHealth() / 100.0f;
    if (healthPercent < 0.0f) healthPercent = 0.0f;
    if (healthPercent > 1.0f) healthPercent = 1.0f;

    float barX = (float)(GetScreenWidth() - health_bar.width * 2.5f - 15);
    float barY = 15;

    DrawTextureEx(health_bar, { barX, barY }, 0, 2.5f, WHITE);

    dest_health = { barX + 2.0f, barY + 1.0f, healthPercent * health.width * 2.5f, (float)health.height * 2.2f };
    DrawTexturePro(health, source_health, dest_health, { 0, 0 }, 0, WHITE);
}

void Game::drawBossHealthBar()
{
    int maxHp = boss.getMaxHealth();
    if (maxHp <= 0) maxHp = 30;
    float hpPercent = (float)boss.getHealth() / (float)maxHp;
    if (hpPercent < 0.0f) hpPercent = 0.0f;

    float barW = 320.0f;
    float barH = 18.0f;
    float barX = (float)GetScreenWidth() / 2.0f - barW / 2.0f;
    float barY = 20.0f;

    DrawRectangle((int)barX - 2, (int)barY - 2, (int)barW + 4, (int)barH + 4, DARKGRAY);
    DrawRectangle((int)barX, (int)barY, (int)(barW * hpPercent), (int)barH, RED);
    DrawRectangleLines((int)barX - 2, (int)barY - 2, (int)barW + 4, (int)barH + 4, WHITE);

    const char* bossText = "BOSS";
    DrawTextEx(custom_font, bossText, { (float)GetScreenWidth() / 2.0f - 30, barY + barH + 4 }, 16, 2, RED);
}

void Game::drawTitleScreen()
{
    float centerX = (float)GetScreenWidth() / 2.0f;

    const char* title1 = "SPACE INVADERS";
    const char* title2 = "GALACTIC DEFENDER";
    Vector2 t1Size = MeasureTextEx(custom_font, title1, custom_font.baseSize * 1.8f, 2);
    Vector2 t2Size = MeasureTextEx(custom_font, title2, custom_font.baseSize * 0.9f, 2);

    DrawTextEx(custom_font, title1, { centerX - t1Size.x / 2.0f, 180 }, custom_font.baseSize * 1.8f, 2, SKYBLUE);
    DrawTextEx(custom_font, title2, { centerX - t2Size.x / 2.0f, 250 }, custom_font.baseSize * 0.9f, 2, GOLD);

    if ((int)(GetTime() * 2.0f) % 2 == 0)
    {
        const char* prompt = "PRESS [ENTER] OR [SPACE] TO PLAY";
        Vector2 pSize = MeasureTextEx(custom_font, prompt, custom_font.baseSize * 0.7f, 2);
        DrawTextEx(custom_font, prompt, { centerX - pSize.x / 2.0f, 380 }, custom_font.baseSize * 0.7f, 2, RAYWHITE);
    }

    // High Score
    const char* hsText = TextFormat("BEST SCORE: %d", high_score);
    Vector2 hsSize = MeasureTextEx(custom_font, hsText, custom_font.baseSize * 0.8f, 2);
    DrawTextEx(custom_font, hsText, { centerX - hsSize.x / 2.0f, 440 }, custom_font.baseSize * 0.8f, 2, YELLOW);

    // Controls Guide Box
    int boxW = 500;
    int boxH = 140;
    int boxX = (GetScreenWidth() - boxW) / 2;
    int boxY = 540;
    DrawRectangle(boxX, boxY, boxW, boxH, ColorAlpha(BLACK, 0.75f));
    DrawRectangleLines(boxX, boxY, boxW, boxH, DARKGRAY);

    DrawTextEx(custom_font, "CONTROLS", { (float)boxX + 20, (float)boxY + 12 }, 16, 2, SKYBLUE);
    DrawTextEx(custom_font, "- WASD : Fly / Navigate Ship", { (float)boxX + 20, (float)boxY + 40 }, 14, 2, LIGHTGRAY);
    DrawTextEx(custom_font, "- MOUSE : Aim & Shoot (Hold LMB for autofire)", { (float)boxX + 20, (float)boxY + 65 }, 14, 2, LIGHTGRAY);
    DrawTextEx(custom_font, "- P / ESC : Pause Game", { (float)boxX + 20, (float)boxY + 90 }, 14, 2, LIGHTGRAY);
    DrawTextEx(custom_font, "- R : Quick Restart", { (float)boxX + 20, (float)boxY + 115 }, 14, 2, LIGHTGRAY);
}

void Game::drawPausedScreen()
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorAlpha(BLACK, 0.65f));

    float centerX = (float)GetScreenWidth() / 2.0f;
    const char* paused = "PAUSED";
    Vector2 pSize = MeasureTextEx(pause_font, paused, 56, 3);
    DrawTextEx(pause_font, paused, { centerX - pSize.x / 2.0f, 300 }, 56, 3, RAYWHITE);

    const char* resumeText = "Press [P] or [ESC] to Resume";
    Vector2 rSize = MeasureTextEx(custom_font, resumeText, 18, 2);
    DrawTextEx(custom_font, resumeText, { centerX - rSize.x / 2.0f, 400 }, 18, 2, LIGHTGRAY);

    const char* restartText = "Press [R] to Restart";
    Vector2 reSize = MeasureTextEx(custom_font, restartText, 16, 2);
    DrawTextEx(custom_font, restartText, { centerX - reSize.x / 2.0f, 440 }, 16, 2, GOLD);
}

void Game::drawGameOverScreen()
{
    DrawTextureEx(gameover_Screen, { 0, 0 }, 0.0f, 1.0f, WHITE);
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorAlpha(BLACK, 0.45f));

    float centerX = (float)GetScreenWidth() / 2.0f;

    const char* goText = "GAME OVER";
    Vector2 goSize = MeasureTextEx(custom_font, goText, custom_font.baseSize * 1.8f, 2);
    DrawTextEx(custom_font, goText, { centerX - goSize.x / 2.0f, 220 }, custom_font.baseSize * 1.8f, 2, RED);

    const char* sText = TextFormat("YOUR SCORE: %d", score);
    Vector2 sSize = MeasureTextEx(custom_font, sText, (float)custom_font.baseSize, 2);
    DrawTextEx(custom_font, sText, { centerX - sSize.x / 2.0f, 320 }, (float)custom_font.baseSize, 2, WHITE);

    const char* hsText = TextFormat("BEST SCORE: %d", high_score);
    Vector2 hsSize = MeasureTextEx(custom_font, hsText, custom_font.baseSize * 0.9f, 2);
    DrawTextEx(custom_font, hsText, { centerX - hsSize.x / 2.0f, 370 }, custom_font.baseSize * 0.9f, 2, GOLD);

    if (score >= high_score && score > 0)
    {
        const char* newBest = "*** NEW HIGH SCORE! ***";
        Vector2 nbSize = MeasureTextEx(custom_font, newBest, custom_font.baseSize * 0.8f, 2);
        DrawTextEx(custom_font, newBest, { centerX - nbSize.x / 2.0f, 420 }, custom_font.baseSize * 0.8f, 2, GREEN);
    }

    if ((int)(GetTime() * 2.5f) % 2 == 0)
    {
        const char* restartPrompt = "PRESS [R] TO PLAY AGAIN";
        Vector2 rpSize = MeasureTextEx(custom_font, restartPrompt, custom_font.baseSize * 0.8f, 2);
        DrawTextEx(custom_font, restartPrompt, { centerX - rpSize.x / 2.0f, 500 }, custom_font.baseSize * 0.8f, 2, RAYWHITE);
    }

    const char* exitPrompt = "PRESS [ESC] FOR MAIN MENU";
    Vector2 epSize = MeasureTextEx(custom_font, exitPrompt, custom_font.baseSize * 0.6f, 2);
    DrawTextEx(custom_font, exitPrompt, { centerX - epSize.x / 2.0f, 550 }, custom_font.baseSize * 0.6f, 2, LIGHTGRAY);
}

void Game::draw()
{
    // Apply subtle screen shake
    Vector2 shake = { 0, 0 };
    if (screenShake > 0.0f)
    {
        shake.x = (float)GetRandomValue(-(int)screenShake, (int)screenShake);
        shake.y = (float)GetRandomValue(-(int)screenShake, (int)screenShake);
    }

    background.draw();

    if (state == GameState::TITLE)
    {
        drawTitleScreen();
        return;
    }

    // Draw Bullets
    for (auto& bullet : bullets)
    {
        bullet.draw();
    }

    // Draw Enemies & Enemy Bullets
    for (auto& enemy : enemies)
    {
        enemy.Draw();
        for (auto& eb : enemy.getEnemyBullets())
        {
            eb.draw();
        }
    }

    // Draw Boss & Boss Bullets
    if (boss.isBossAlive())
    {
        boss.Draw();
        for (auto& bb : bossBullets)
        {
            bb.draw();
        }
    }

    drawParticles();

    if (state != GameState::GAMEOVER)
    {
        _player.draw();
    }

    // HUD: Score & Health
    char score_text[40];
    snprintf(score_text, sizeof(score_text), "SCORE: %d", score);
    DrawTextEx(custom_font, score_text, { 15, 15 }, custom_font.baseSize * 0.85f, 2, WHITE);

    char high_text[40];
    snprintf(high_text, sizeof(high_text), "BEST:  %d", high_score);
    DrawTextEx(custom_font, high_text, { 15, 45 }, custom_font.baseSize * 0.65f, 2, GOLD);

    drawHealthBar();

    if (boss.isBossAlive())
    {
        drawBossHealthBar();
    }

    if (state == GameState::PAUSED)
    {
        drawPausedScreen();
    }
    else if (state == GameState::GAMEOVER)
    {
        drawGameOverScreen();
    }
}

Vector2 Game::getPlayerPosition()
{
    return _player.getPosition();
}

Vector2 Game::getPlayerPOstion()
{
    return _player.getPosition();
}

bool Game::get_running_stat()
{
    return state == GameState::PLAYING;
}

Game::~Game()
{
    UnloadTexture(playerBulletTexture);
    UnloadTexture(enemy_bullet_texture);
    UnloadTexture(lower_background_texture);
    UnloadTexture(upper_background_texture);
    UnloadTexture(enemyTexture);
    UnloadTexture(health_bar);
    UnloadTexture(health);
    UnloadTexture(gameover_Screen);
    UnloadTexture(boss_texture);
    UnloadTexture(boss_bullet_texture);

    for (auto& tex : enemyTextures)
    {
        if (tex.id != enemyTexture.id)
        {
            UnloadTexture(tex);
        }
    }

    UnloadFont(custom_font);
    UnloadFont(pause_font);

    UnloadMusicStream(back_music);
    UnloadSound(Collision_sound);
}






