#ifndef TDMAP_H
#define TDMAP_H
#include <boost/multi_array.hpp>
#include "sizes.h"

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
private:
	coordinate_type coordinates;
	Position dimensions;

public:
	TDMap(int width, int height);
	Tile& at(int row, int column);
};

#endif