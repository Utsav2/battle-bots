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
        Path path;
        Spritesheet * spritesheet;
        std::vector<Coordinate> cycles;
        std::vector<Coordinate> dead_cycles;
        bool attacked;
        bool dead;
        int health;
        Coordinate random_pos;

    public:
        Sprite(Path path, Spritesheet * spritesheet, std::vector<Coordinate>& cycles, std::vector<Coordinate>& dead_cycles, int health);
        Coordinate get_coordinate(int);
        void move_to_next_position();
        bool is_out_of_map();
        Coordinate get_previous_position();
        Spritesheet * get_spritesheet();
        std::vector<Coordinate>& get_sscords();
        void set_not_attacked();
        void set_attacked();
        void add_damage(int);
        bool is_attacked();
        void set_new_cycles(std::vector<Coordinate>);
        void die();
        bool is_dead(){return dead;}
        void set_random_position(Coordinate coord);
        Coordinate get_offset();


};

#include "sprite.cpp"
#endif
