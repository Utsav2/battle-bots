#include <boost/python.hpp>
#include "boost/multi_array.hpp"
#include <boost/foreach.hpp>
#include <vector>
#include <iostream>
#include <array>
#include <iostream>
#include <string>

#define DIMENSION_COUNT 2
#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

class Player
{

public:
	Player()
	{

	}
};

class Tile
{

};

typedef boost::multi_array<Tile, DIMENSION_COUNT> coordinate_type;
typedef boost::array<coordinate_type::index, DIMENSION_COUNT> Position; 

class TDMap
{

	//a 2d array

	coordinate_type coordinates;
	Position dimensions;

	public:
	TDMap(int width, int height)
	{
		if(width <= 0 || height <= 0){
			throw "nonzero width and height expected";
		}
		dimensions[0] = width;
		dimensions[1] = height;
		coordinates(dimensions);
	}

	/*
	boost does range checking, so we dont need to do it
	we can think about it later.
	*/
	Tile& at(int width, int height)
	{
		Position p = {{width, height}};
		return coordinates(p);
	}

};

class TDGamecore
{

	private:
		std::vector<Player> players;
		TDMap * map;

	public:
		TDGamecore(int number_of_players)
		{
			TDGamecore(number_of_players, DEFAULT_WIDTH, DEFAULT_HEIGHT);
		}

		TDGamecore(int number_of_players, int width, int height)
		{
			while(number_of_players-- > 0)
            	generate_new_player();

            map = new TDMap(width, height);
		}

		void generate_new_player()
        {
            players.push_back(*(new Player()));
        }

};