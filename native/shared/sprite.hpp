#ifndef SPRITE_H
#define SPRITE_H
#include "coordinate.hpp"
#include "path_creator.hpp"

class Sprite{
    private:
        int position;
        Path * path;
    public:
        Sprite(Path * path);
        Coordinate get_coordinate();
        void move_to_next_position();
        bool is_out_of_map();
}

#endif
