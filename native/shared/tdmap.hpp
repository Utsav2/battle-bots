#ifndef TDMAP_HPP
#define TDMAP_HPP
#include <boost/foreach.hpp>
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

class TDMap
{
	private:
		std::vector<Sprite *> sprites;
		std::vector<Tower *> towers;
		Coordinate dimensions;
		std::vector<Path *> paths;

	public:
		TDMap(Coordinate dimensions, std::vector<Path *> paths);
		TDMap(int width, int height, std::vector<Path *> paths);
		std::vector<Sprite *> get_sprites_at(int row, int column);
		std::vector<Sprite *> get_sprites_at(Coordinate c);
		Tower * get_tower_at(Coordinate c);
		Tower * get_tower_at(int row, int column);
		bool add_tower(Tower * tower);
		bool add_sprite(Sprite * sprite);
};

#include "tdmap.cpp"
#endif
