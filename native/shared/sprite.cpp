#include "sprite.hpp"
#include <iostream>

Coordinate Sprite::get_coordinate()
{
    return this->path->get_coordinate(this->pos_index);
}

void Sprite::move_to_next_position()
{
    this->pos_index++;
}

Coordinate Sprite::get_previous_position()
{
    if (this->pos_index == 0)
    	return get_coordinate();
    else
    	return this->path->get_coordinate(this->pos_index-1);
}


Sprite::Sprite(Path * path, Spritesheet * spritesheet, Coordinate ss_coords) : spritesheet(spritesheet), ss_coords(ss_coords)
{
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

Coordinate Sprite::get_sscords()
{
	return ss_coords;
}