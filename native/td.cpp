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

        Tower *  tower_generator(Coordinate location, int player_money)
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
        TDGamecore(int number_of_players, int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT) : 

        players(number_of_players), zombie("zombie.png", Coordinate(128, 128), 8), projectile ("projectile.png", Coordinate(64, 64), 2), simple_range(6)

        {
            srand(time(NULL));

            Path * path = new Path(NUM_ROWS, NUM_COLS);
            paths.push_back(path);
            map = new TDMap(width, height, paths);

            while(number_of_players-- > 0)
                generate_new_player();

            gui = new GUI(NUM_ROWS, NUM_COLS, paths, map);

            set_up();

            Sprite * sprite = sprite_generator();
            Tower * tower = tower_generator(Coordinate(6,4), 1000);

            map->add_sprite(sprite);
            //
            map->add_tower(tower);
            map->add_tower(tower_generator(Coordinate(3,2), 1000));


           	/*gui->Update();
           	sprite->move_to_next_position();
           	gui->Update();
           	sprite->move_to_next_position();
            Sprite * sprite2 = sprite_generator(0.5);
            map->add_sprite(sprite2);
            sprite2->move_to_next_position();
           	tower->set_attacking(Coordinate(1, 5));
            gui->Update();
            sprite->set_attacked();
            sprite2->move_to_next_position();
            tower->remove_attack_tile(Coordinate(1, 5));
            sprite->move_to_next_position();
            tower->set_attacking(Coordinate(2, 5));
            gui->Update();  
            tower->remove_attack_tile(Coordinate(2, 5));
            sprite->move_to_next_position();
            sprite2->move_to_next_position();
            tower->set_attacking(Coordinate(3, 5));
            gui->Update();  
            tower->remove_attack_tile(Coordinate(3, 5));
            sprite->die();
            sprite2->move_to_next_position();
            gui->Update();  
            sprite2->move_to_next_position();
            map->remove_sprite(sprite);
            gui->Update(); 
            sprite2->move_to_next_position();
            gui->Update();
            sprite2->move_to_next_position();
            gui->Update();
            sprite2->move_to_next_position();
            gui->Update();
            sprite2->move_to_next_position();
            gui->Update();
            sprite2->move_to_next_position();
            gui->Update();
            sprite2->move_to_next_position();
            gui->Update(); */
            game_loop();


        }

        void reduce_player_score()
        {

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
                    }
                }
        }

        void game_loop()
        {

            while(1)
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
