#ifndef SPRITE_H
#define SPRITE_H
#include "coordinate.hpp"
#include "path_creator.hpp"
#include <iostream>
#include "spritesheet.hpp"


class Sprite
{
    private:
        size_t pos_index;
        Path * path;
        Spritesheet * spritesheet;
        Coordinate ss_coords;
    public:
        Sprite(Path * path, Spritesheet * spritesheet, Coordinate ss_coords);
        Coordinate get_coordinate();
        void move_to_next_position();
        bool is_out_of_map();
        Coordinate get_previous_position();
        Spritesheet * get_spritesheet();
        Coordinate get_sscords();

};

#include "sprite.cpp"
#endif
