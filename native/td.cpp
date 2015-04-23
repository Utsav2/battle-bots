#include <boost/python.hpp>
#include <boost/multi_array.hpp>
#include <boost/foreach.hpp>
#include <thread> 
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
#include <stdlib.h>    
#include <time.h> 
#include "shared/spritesheet.hpp" 
#include <boost/thread.hpp>
#include <pthread.h>


class TDGamecore
{
    private:
        int money;
        TDMap * map;
        GUI * gui; 
        Spritesheet zombie;
        Spritesheet projectile;
        std::vector<Path *> paths;
        std::vector<Coordinate> tower_cycles;
        std::vector<Coordinate> cycles;
        std::vector<Coordinate> death_cycles;
        Range simple_range;

        void set_up()
        {
            for(int i = 10; i < 30; i++)
                death_cycles.push_back(Coordinate(i, 5));
            
            tower_cycles = {Coordinate(1, 1), Coordinate(0, 1), Coordinate(2, 0), Coordinate(3, 2), Coordinate(0, 3), Coordinate(1, 3)};
            for(int i = 5; i < 13; i++)
                cycles.push_back(Coordinate(i, 5));
        }

        Tower *  tower_generator(Coordinate location)
        {
            Tower * tower = new Tower(simple_range, location, "tower.png", &projectile, tower_cycles);
            return tower;
        }

        Sprite * sprite_generator(int health = 100)
        {
            Sprite * sprite = new Sprite(paths.at(0) , &zombie, cycles, death_cycles, health);
            return sprite;
        }


  	public:

        int get_money()
        {
            return money;
        }

  		bool make_tower_request(int x, int y)
  		{
            Tower * tower = tower_generator(Coordinate(x, y));
            if(tower->get_cost() > money)
            {   
                delete tower;
                return false;
            }
            else
            {
               money -= tower->get_cost(); 
  			   return map->add_tower(tower);
            }
  		}

        TDGamecore(int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT) : money(1000),
                zombie("zombie.png", Coordinate(128, 128), 8), projectile ("projectile.png", Coordinate(64, 64), 2), simple_range(6)

        {
            srand(time(NULL));

            Path * path = new Path(NUM_ROWS, NUM_COLS);
            paths.push_back(path);
            map = new TDMap(width, height, paths);
            set_up();
            Sprite * sprite = sprite_generator();
            map->add_sprite(sprite);
            gui = new GUI(NUM_ROWS, NUM_COLS, paths, map);
        }

        void reduce_player_score()
        {

        }

        void increase_player_score()
        {
            money += 50;
        }

        void update_sprites()
        {
                BOOST_FOREACH(Sprite * sprite, map->get_sprites())
                {
                    if(!sprite->is_dead())
                    {
                        sprite->move_to_next_position();
                        if(sprite->is_out_of_map())
                        {
                            map->remove_sprite(sprite);
                            reduce_player_score();
                        }
                    }
                    else
                    {
                        gui->Update();
                        map->remove_sprite(sprite);
                        map->add_sprite(sprite_generator(rand() % 200));
                        increase_player_score();
                    }
                }
        }

        void game_loop(int number_of_times = 1)
        {

            while(--number_of_times >= 0)
            {
                BOOST_FOREACH(Sprite * sprite, map->get_sprites())
                {
                    bool sprite_attacked_flag = false;
                    BOOST_FOREACH(Tower * tower, map->get_towers())
                    {
                        if(tower->can_attack(sprite->get_coordinate()))
                        {
                            tower->set_attacking(sprite);
                            sprite_attacked_flag = true;
                        }
                    }
                    if(!sprite_attacked_flag)
                        sprite->set_not_attacked();
                }

                gui->Update();
                map->update_towers();
                update_sprites();
            }
        }


};

using namespace boost::python;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(loop_overloads, game_loop, 0, 1);

BOOST_PYTHON_MODULE(libtd)
{
    class_<TDGamecore>("Core", init<>())
    .def("loop", &TDGamecore::game_loop, loop_overloads(args("number_of_times")))
    .def("tower", &TDGamecore::make_tower_request)
    .def_readonly("money", &TDGamecore::get_money);
}
