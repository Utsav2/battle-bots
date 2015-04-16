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



	//a 2d array

TDMap::TDMap(int width, int height)
{
	assert(width > 0 && height > 0);
	dimensions[0] = width;
	dimensions[1] = height;
	coordinates.resize(dimensions);
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
			//while(number_of_players-- > 0)
            //	generate_new_player();

			Path * path = create_path(NUM_ROWS, NUM_COLS);
            map = new TDMap(width, height);
           	gui = new GUI(NUM_ROWS, NUM_COLS, path, map);

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
