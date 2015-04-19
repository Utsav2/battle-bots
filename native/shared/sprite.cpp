#include "sprite.hpp"
#include <iostream>

Coordinate Sprite::get_coordinate()
{
    return this->path->get_coordinate(this->pos_index);
}

void Sprite::move_to_next_position()
{
    if(dead)
        return;
    this->pos_index++;
}

Coordinate Sprite::get_previous_position()
{
    if (this->pos_index == 0 || dead)
    	return get_coordinate();
    else
    	return this->path->get_coordinate(this->pos_index-1);
}


Sprite::Sprite(Path * path, Spritesheet * spritesheet, std::vector<Coordinate>& cycles) : spritesheet(spritesheet), cycles(cycles)
{
    attacked = false;
    dead = false;
    this->path = path;
    this->pos_index = 0;

}

Spritesheet * Sprite::get_spritesheet()
{
	return this->spritesheet;
}

bool Sprite::is_out_of_map()
{
    return this->pos_index >= this->path->size();
}

std::vector<Coordinate>& Sprite::get_sscords()
{
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

void Sprite::set_new_cycles(std::vector<Coordinate> cycles)
{
    this->cycles = cycles;
}

void Sprite::die()
{
    dead = true;
}