#include <boost/python.hpp>
#include <boost/multi_array.hpp>
#include <boost/foreach.hpp>
#include <vector>
#include <iostream>
#include <array>
#include <iostream>
#include <string>
#include "shared/path_creator.hpp"
#include "shared/tdmap.hpp"
#include "gui.cpp"
#include "shared/sizes.h"
#include "shared/sprite.hpp"
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include "shared/spritesheet.hpp" 


class TDGamecore
{
    private:
        std::vector<Player> players;
        TDMap * map;
        GUI * gui; 

  	public:
        TDGamecore(int number_of_players)
        {
            TDGamecore(number_of_players, DEFAULT_WIDTH, DEFAULT_HEIGHT);
        }
        TDGamecore(int number_of_players, int width, int height)
        {
            srand(time(NULL));
            Path * path = new Path(NUM_ROWS, NUM_COLS);
            std::vector<Path *> paths {path} ;
            map = new TDMap(width, height, paths);
            Tower * tower = new Tower(Coordinate(2,4));
            tower->set_image_string("tower.png");
            tower->set_attack_image_string("arrow.png");
            map->add_tower(tower);
            Spritesheet zombie ("zombie.png", Coordinate(128, 128));
            std::vector<Coordinate> cycles;
            for(int i = 5; i < 13; i++)
                cycles.push_back(Coordinate(i, 5));
            Sprite * sprite = new Sprite(path, &zombie, cycles);
            map->add_sprite(sprite);
            gui = new GUI(NUM_ROWS, NUM_COLS, paths, map);
           	gui->Update();
           	sprite->move_to_next_position();
           	gui->Update();
           	sprite->move_to_next_position();
            sprite->set_attacked();
           	tower->set_attacking(Coordinate(1, 5));
            gui->Update();
            tower->remove_attack_tile(Coordinate(1, 5));
            sprite->move_to_next_position();
            tower->set_attacking(Coordinate(2, 5));
            gui->Update();  
            tower->remove_attack_tile(Coordinate(2, 5));
            sprite->move_to_next_position();
            tower->set_attacking(Coordinate(3, 5));
            gui->Update();  
            tower->remove_attack_tile(Coordinate(3, 5));
            sprite->die();
            std::vector<Coordinate> v;
            for(int i = 10; i < 30; i++)
                v.push_back(Coordinate(i, 5));
            sprite->set_new_cycles(v);
            gui->Update();  
            map->remove_sprite(sprite);
            gui->Update();  
        }

        void generate_new_player()
        {
            players.push_back(*(new Player()));
        }
};

using namespace boost::python;

BOOST_PYTHON_MODULE(libtd)
{
    class_<TDGamecore>("TDGamecore", init<int>());
}
