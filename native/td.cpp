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
            Path * path = new Path(NUM_ROWS, NUM_COLS);
            std::vector<Path *> paths {path} ;
            map = new TDMap(width, height, paths);
            Tower * tower = new Tower(Coordinate(3,4));
            tower->set_image_string("tower.png");
            tower->set_attack_image_string("arrow.png");
            map->add_tower(tower);
            Sprite * sprite = new Sprite(path);
            map->add_sprite(sprite);
            sprite->image_string = "sprite.png";
            gui = new GUI(NUM_ROWS, NUM_COLS, path, map);
           	gui->Update();
           	sprite->move_to_next_position();
           	gui->Update();
           	sprite->move_to_next_position();
           	tower->set_attacking(Coordinate(2, 5));
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
