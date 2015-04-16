#include <boost/python.hpp>
#include <boost/multi_array.hpp>
#include <boost/foreach.hpp>
#include <vector>
#include <iostream>
#include <array>
#include <iostream>
#include <string>
#include "shared/path_creator.cpp"
#include "shared/tdmap.hpp"
#include "gui.cpp"
#include "shared/sizes.h"



TDMap::TDMap(int width, int height, Path * p)
{
	assert(width > 0 && height > 0 && p != nullptr);
	dimensions[0] = width;
	dimensions[1] = height;
	coordinates.resize(dimensions);
	path = p;
}

/*
boost does range checking, so we dont need to do it
we can think about it later.
*/

Tile& TDMap::at(int width, int height)
{
	Position p = {{width, height}};
	return coordinates(p);
}

bool TDMap::set_tower_at(int x, int y, Tower * tower)
{

	Tile &t = this->at(x, y);

	if(t.tower != nullptr || path->in(x, y))
	{	
		return false;
	}
	t.tower = tower;
	return true;
}

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
            map = new TDMap(width, height, path);
            Tower * tower = new Tower();
            tower->set_image_string("tower.png");
            map->set_tower_at(3, 4, tower);
           	gui = new GUI(NUM_ROWS, NUM_COLS, path, map);
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
