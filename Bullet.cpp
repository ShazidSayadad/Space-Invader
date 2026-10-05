#include "Bullet.h"

Bullet::Bullet()
{
    _position = { 0, 0 };
    _angle = 0;
    _texture = { 0 };
    source = { 0, 0, 0, 0 };
    _width = 0;
    _height = 0;
    _des = { 0, 0, 0, 0 };
    _xSpeed = 0;
    _ySpeed = 0;
}

Bullet::Bullet(Vector2 pos, float angle, Texture2D texture) 
{
    _position = pos;
    _angle = angle - 90.0f;
    _texture = texture;
    _width = (float)_texture.width;
    _height = (float)_texture.height;
    source = { 0, 0, _width, _height };
    _des = { _position.x, _position.y, _width * _scale, _height * _scale };
    Vector2 direction = { cosf(_angle * PI / 180.0f), sinf(_angle * PI / 180.0f) };
    _xSpeed = direction.x * _speed;
    _ySpeed = direction.y * _speed;
}

void Bullet::draw()
{
    float w = _width * _scale;
    float h = _height * _scale;
    Rectangle dest = { _position.x, _position.y, w, h };
    Vector2 origin = { w / 2.0f, h / 2.0f };
    DrawTexturePro(_texture, source, dest, origin, _angle + 90.0f, WHITE);
}

void Bullet::update()
{
    _position.x += _xSpeed;
    _position.y += _ySpeed;
}

float Bullet::getWidth()
{
    return _width;
}

float Bullet::getHeight()
{
    return _height;
}

Vector2 Bullet::getPosition()
{
    return _position;
}

void Bullet::MoveVertical(float speed)
{
    _position.y += speed;
}

void Bullet::MoveHorizontal(float speed)
{
    _position.x += speed;
}

void Bullet::moveWithBackground(float xSpeed, float ySpeed)
{
    _position.x += xSpeed;
    _position.y += ySpeed;
}

Rectangle Bullet::get_rect_bullet() 
{
    float w = _width * _scale;
    float h = _height * _scale;
    return { _position.x - w / 2.0f, _position.y - h / 2.0f, w, h };
}

Bullet::~Bullet()
{
}

