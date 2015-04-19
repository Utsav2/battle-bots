#ifndef TOWER_H
#define TOWER_H
#include <iostream>
#include <vector>
#include "coordinate.hpp"
#include "spritesheet.hpp"


enum Direction { NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, NORTHWEST };

class Range
{
	private:
		std::vector<int> ranges;
		bool in_vertical(int);
		bool in_horizontal(int);
		bool in_simple(int, int);
		bool in_negative(int, int);
		bool in_positive(int, int);
		bool in_diagonal(int, int);
	public:
		Range();
		Range(int);
		void set_all(int);
		void set_specific(Direction, int);
		bool in(Coordinate, Coordinate);
};

class Tower
{

	private:
		Range * range;
		std::vector<Coordinate> attacking_tiles;
		Coordinate location;
		std::string image_string;
		Spritesheet * spritesheet;
		std::vector<Coordinate> cycles;

	public:
		Tower(Coordinate location, std::string image_string, Spritesheet * spritesheet, std::vector<Coordinate> & cycles);
		Coordinate get_coordinate();
		void set_attacking(Coordinate);
		void remove_attack_tile(Coordinate);
		std::vector<Coordinate>& get_attack_tiles();
        Spritesheet * get_spritesheet();
        std::string get_image_string();
        std::vector<Coordinate>& get_sscords();

};


#endif
