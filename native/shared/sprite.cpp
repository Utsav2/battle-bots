#include "sprite.hpp"
#include <iostream>

Coordinate Sprite::get_coordinate(int index = -1)
{
    if(index == -1)
        index = pos_index;

    return this->path.get_coordinate(index);
}

void Sprite::move_to_next_position()
{
    if(dead)
        return;
    this->pos_index++;
}

Coordinate Sprite::get_previous_position()
{
    if (this->pos_index == 0)
    	return get_coordinate(0);
    else
    	return get_coordinate(this->pos_index-1);
}


Sprite::Sprite(Path path, Spritesheet * spritesheet, std::vector<Coordinate>& cycles, std::vector<Coordinate>& dead_cycles, int health = 100) : path(path), spritesheet(spritesheet), cycles(cycles), dead_cycles(dead_cycles) , health(health), random_pos(Coordinate(0, 0))
{
    attacked = false;
    dead = false;
    this->pos_index = 0;

}

Spritesheet * Sprite::get_spritesheet()
{
	return this->spritesheet;
}

bool Sprite::is_out_of_map()
{
    return this->pos_index >= this->path.size();
}

std::vector<Coordinate>& Sprite::get_sscords()
{
    if(dead)
        return dead_cycles;
    else
        return cycles;
}

void Sprite::set_attacked()
{
    attacked = true;
}
void Sprite::set_not_attacked()
{
    attacked = false;
}
bool Sprite::is_attacked()
{
    return attacked;
}

void Sprite::die()
{
    dead = true;
}

void Sprite::add_damage(int damage)
{
    health -= damage;
    if(health <= 0)
        die();
}

void Sprite::set_random_position(Coordinate coord)
{
    random_pos.x = coord.x;
    random_pos.y = coord.y;
}

Coordinate Sprite::get_offset()
{
    return random_pos;
}