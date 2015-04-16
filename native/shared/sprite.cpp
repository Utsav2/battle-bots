#include "sprite.hpp"
#include <iostream>

Coordinate Sprite::get_coordinate()
{
    return this->path->get_coordinate(this->position);
}

void Sprite::move_to_next_position()
{
    this->position
}

Sprite::Sprite(Path * path)
{
    this->path = path;
    this->position = 0;
}

bool Sprite::is_out_of_map()
{
    return this->position >= this->path.size();
}
