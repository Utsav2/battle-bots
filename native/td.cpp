#include <boost/python.hpp>
#include <boost/multi_array.hpp>
#include <boost/foreach.hpp>
#include <thread> 
#include <vector>
#include <iostream>
#include <array>
#include <string>
#include "shared/path_creator.hpp"
#include "shared/tdmap.hpp"
#include "gui.cpp"
#include "shared/sizes.h"
#include "shared/sprite.hpp"
#include <stdlib.h>    
#include <time.h> 
#include "shared/spritesheet.hpp" 
#include "shared/player.hpp"
#include <pthread.h>

class TDGamecore
{
    private:
        Player player;
        int level_number;
        TDMap * map;
        GUI * gui; 
        Spritesheet zombie;
        Spritesheet projectile;
        std::vector<Path> paths;
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

        Tower * tower_generator(Coordinate location)
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
            return player.money;
        }

        int get_kills()
        {
            return player.kills;
        }

        int get_score()
        {
            return player.score;
        }

  		bool make_tower_request(int x, int y)
  		{
            Tower * tower = tower_generator(Coordinate(x, y));
            if(tower->get_cost() > player.money)
            {   
                delete tower;
                return false;
            }
            else
            {
  			   bool retval = map->add_tower(tower);
               if(retval)
                   player.money -= tower->get_cost(); 
               return retval;
            }
  		}

        TDGamecore(int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT) :
                zombie("zombie.png", Coordinate(128, 128), 8), projectile ("projectile.png", Coordinate(64, 64), 2), simple_range(6)
        {
            srand(time(NULL));

            Path path(NUM_ROWS, NUM_COLS);
            paths.push_back(path);
            map = new TDMap(NUM_ROWS, NUM_COLS, paths);
            set_up();
            Sprite * sprite = sprite_generator();
            map->add_sprite(sprite);
            gui = new GUI(NUM_ROWS, NUM_COLS, paths, map);
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
                            player.score-=1;
                        }
                    }
                    else
                    {
                        player.money+=50;
                        player.score+=1;
                        player.kills++;
                        level_number = player.kills/10;
                    }
                }

                int range = (rand() % 2) ;
                while(--range >= 0)
                    map->add_sprite(sprite_generator(100 + level_number * 50));
        }

        void game_loop(int number_of_times = 1)
        {

            while(--number_of_times >= 0)
            {
                BOOST_FOREACH(Sprite * sprite, map->get_sprites())
                {
                    if(sprite->is_dead())
                    {
                        map->remove_sprite(sprite);
                        continue;
                    }

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
    .def_readonly("money", &TDGamecore::get_money)
    .def_readonly("kills", &TDGamecore::get_kills)
    .def_readonly("score", &TDGamecore::get_score);
}