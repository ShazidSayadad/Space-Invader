#include "Enemy.h"

Enemy::Enemy(Texture2D texture, int x, int y, Texture2D bullet)
{
    _texture = texture;
    _Bullettexture = bullet;
    _position.x = (float)x;
    _position.y = (float)y;
    distance = 0;
    scale = 4.0f;
    speed = 3.5f;
    width = (float)texture.width * scale;
    height = (float)texture.height * scale;
    source = { 0, 0, (float)texture.width, (float)texture.height };
    destination = { _position.x, _position.y, width, height };
    origin = { width / 2.0f, height / 2.0f };
    fireRate = 0.8f;
    _distance = (float)GetRandomValue(150, 450);
}

void Enemy::Update(Vector2 playerPosition)
{
    direction = Vector2Subtract(_position, playerPosition);
    Vector2 dir = Vector2Normalize(direction);
    angle = (atan2f(_position.y - playerPosition.y, _position.x - playerPosition.x) * 180.0f) / PI;
    float dist = Vector2Distance(_position, playerPosition);
    if (dist > _distance)
    {
        _position.x -= dir.x * speed;
        _position.y -= dir.y * speed;
    }
    if ((float)GetTime() - current > fireRate && bullets.size() < 4)
    {
        current = (float)GetTime();
        bullets.push_back(Bullet({ _position.x, _position.y }, angle - 90.0f, _Bullettexture));
    }
}

void Enemy::Draw()
{
    destination = { _position.x, _position.y, width, height };
    DrawTexturePro(_texture, source, destination, origin, angle + 90.0f, WHITE);
}

void Enemy::MoveHorizontal(float s)
{
    _position.x += s;
}

void Enemy::MoveVertical(float s)
{
    _position.y += s;
}

Vector2 Enemy::getPosition()
{
    return _position;
}

float Enemy::getAngle()
{
    return angle;
}

float Enemy::getWidth()
{
    return width;
}

float Enemy::getHeight()
{
    return height;
}

vector<Bullet>& Enemy::getEnemyBullets()
{
    return bullets;
}

Rectangle Enemy::get_rect_enemy()
{
    return { _position.x - origin.x, _position.y - origin.y, width, height };
}
