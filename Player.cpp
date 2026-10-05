#include "Player.h"

void Player::calculateAngle()
{
    Vector2 mouse = GetMousePosition();
    _angle = ((atan2f(mouse.y - _position.y, mouse.x - _position.x)) * 180.0f) / PI;
    _angle += 90.0f;
}

Player::Player()
{
    _angle = 0;
    _position = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f };
    _ship = LoadTexture("assets/spaceShip/ships/purple.png");
    _width = (float)_ship.width;
    _height = (float)_ship.height;
    _source = { 0, 0, _width, _height };
    _destination = { _position.x, _position.y, _width * _scale, _height * _scale };
    _origin = { (_width * _scale) / 2.0f, (_height * _scale) / 2.0f };
    health = 100.0f;
}

void Player::draw()
{
    DrawTexturePro(_ship, _source, _destination, _origin, (float)_angle, WHITE);
}

void Player::update()
{
    calculateAngle();
}

double Player::getAngle()
{
    return _angle;
}

float Player::getHealth()
{
    return health;
}

void Player::takeDamage(float damage)
{
    health -= damage;
    if (health < 0) health = 0;
}

void Player::reset()
{
    health = 100.0f;
    _position = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f };
    _angle = 0;
}

Vector2 Player::getPosition() 
{
    return _position; 
}

Rectangle Player::get_rect_player() 
{
    float w = _width * _scale;
    float h = _height * _scale;
    return { _position.x - w / 2.0f, _position.y - h / 2.0f, w, h };
}

Player::~Player()
{
    UnloadTexture(_ship);
}