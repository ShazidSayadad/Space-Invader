#include "Boss.h"

Boss::Boss() : Enemy()
{
    isAlive = false;
    isKilled = false;
    _health = 0;
    _maxHealth = 0;
}

Boss::Boss(Vector2 pos, Texture2D texture, Texture2D bullet)
{
    _texture = texture;
    _Bullettexture = bullet;
    _position = pos;
    distance = 0;
    scale = 3.0f;
    speed = 2.0f;
    width = (float)texture.width * scale;
    height = (float)texture.height * scale;
    source = { 0, 0, (float)texture.width, (float)texture.height };
    destination = { _position.x, _position.y, width, height };
    origin = { width / 2.0f, height / 2.0f };
    fireRate = 0.4f;
    _health = 30;
    _maxHealth = 30;
    isAlive = false;
    isKilled = false;
    _distance = 180.0f;
}

void Boss::spawn(Vector2 pos, int maxHp)
{
    _position = pos;
    _maxHealth = maxHp;
    _health = maxHp;
    isAlive = true;
    isKilled = false;
}

void Boss::Update(Vector2 playerPosition)
{
    if (!isAlive) return;

    direction = Vector2Subtract(_position, playerPosition);
    Vector2 dir = Vector2Normalize(direction);
    angle = (atan2f(_position.y - playerPosition.y, _position.x - playerPosition.x) * 180.0f) / PI;
    float dist = Vector2Distance(_position, playerPosition);
    if (dist > _distance)
    {
        _position.x -= dir.x * speed;
        _position.y -= dir.y * speed;
    }
}

void Boss::Draw()
{
    if (!isAlive) return;
    destination = { _position.x, _position.y, width, height };
    DrawTexturePro(_texture, source, destination, origin, angle + 90.0f, WHITE);
}

int Boss::getHealth()
{
    return _health;
}

int Boss::getMaxHealth()
{
    return _maxHealth;
}

void Boss::getDamage(int damage)
{
    _health -= damage;
    if (_health <= 0)
    {
        _health = 0;
        isKilled = true;
        isAlive = false;
    }
}

bool Boss::isBossAlive()
{
    return isAlive;
}

void Boss::setAlive()
{
    isAlive = true;
    _health = _maxHealth > 0 ? _maxHealth : 30;
    _maxHealth = _health;
}

bool Boss::isBossKilled()
{
    return isKilled;
}

void Boss::setBossKilled()
{
    isKilled = false;
}

void Boss::reset()
{
    isAlive = false;
    isKilled = false;
    _health = 0;
    _position = { 0, -2000 };
}

Rectangle Boss::boss_get_rect()
{
    if (!isAlive)
        return Rectangle{ -1000, -1000, 0, 0 };
    return { _position.x - origin.x, _position.y - origin.y, width, height };
}

vector<Bullet>& Boss::getBossBullets()
{
    return bullets;
}

Vector2 Boss::getPosition()
{
    return _position;
}

float Boss::getAngle()
{
    return angle;
}

