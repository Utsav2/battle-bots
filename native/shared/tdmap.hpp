#ifndef TDMAP_HPP
#define TDMAP_HPP
#include <boost/multi_array.hpp>
#include "tower.cpp"
#include "sizes.h"
#include "sprite.hpp"

class Player
{
	public:
		Player()
		{

		}
};

class Tile
{
	public:
		Tower * tower;
		std::vector<Sprite *> sprites;
};

typedef boost::multi_array<Tile, DIMENSION_COUNT> coordinate_type;
typedef boost::array<coordinate_type::index, DIMENSION_COUNT> Position; 

class TDMap
{
	private:
		coordinate_type coordinates;
		Position dimensions;
		Path * path;

	public:
		TDMap(int width, int height, Path * path);
		Tile& at(int row, int column);
		bool set_tower_at(int row, int column, Tower * tower);
};

#endif