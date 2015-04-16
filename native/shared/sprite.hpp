#ifndef SPRITE_H
#define SPRITE_H
#include "coordinate.hpp"
#include "path_creator.hpp"
#include <iostream>

class Sprite
{
    private:
        size_t position;
        Path * path;
    public:
        Sprite(Path * path);
        Coordinate get_coordinate();
        void move_to_next_position();
        bool is_out_of_map();
        Coordinate get_previous_position();
        std::string image_string;
};

#include "sprite.cpp"
#endif
