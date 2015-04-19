#ifndef SPRITE_H
#define SPRITE_H
#include "coordinate.hpp"
#include "path_creator.hpp"
#include <iostream>
#include "spritesheet.hpp"
#include <vector>


class Sprite
{
    private:
        size_t pos_index;
        Path * path;
        Spritesheet * spritesheet;
        std::vector<Coordinate> cycles;
        bool attacked;
        bool dead;
    public:
        Sprite(Path * path, Spritesheet * spritesheet, std::vector<Coordinate>& cycles);
        Coordinate get_coordinate();
        void move_to_next_position();
        bool is_out_of_map();
        Coordinate get_previous_position();
        Spritesheet * get_spritesheet();
        std::vector<Coordinate>& get_sscords();
        void set_not_attacked();
        void set_attacked();
        bool is_attacked();
        void set_new_cycles(std::vector<Coordinate>);
        void die();


};

#include "sprite.cpp"
#endif
